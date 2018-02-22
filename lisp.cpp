#include "lisp.hpp"

#include <cctype>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace utils
{
        bool contains(std::string const& s, char c)
        {
                return s.find(c) != s.npos;
        }
}

namespace lisp
{
        bool displayMExp = true; // If true, show [a ; b ; c] instead of (a b c) for debugging purpose

        std::ostream &operator<<(std::ostream & o, Cell const& c)
        {
                if(displayMExp)
                {
                        switch(c.type)
                        {
                        case Symbol:
                                return o << c.val;
                        case Number:
                                return o << c.num;

                        case List:
                                if (c.list.empty())
                                        return o << "[]";

                                o << "[ ";
                                for(auto it =  c.list.begin() ; it != c.list.end() - 1 ; ++it)
                                {
                                        o << *it << " ; ";
                                }
                                return o << c.list[c.list.size() - 1] << " ]";

                        case Proc:
                                return o << "<proc: " << c.val << " >";
                        case SpeForm:
                                return o << "<speform: " << c.val << " >";
                        case Lambda:
                                return o << "<lambda>";
                        case Macro:
                                return o << "<macro>";
                        }
                }

                return o;
        }

        const Cell trueSym{Symbol, "#t"};
        const Cell falseSym{Symbol, "#f"};
        const Cell nil{Symbol, "nil"};

        Env::Env(std::vector<Cell> const& params, std::vector<Cell> const& args, Env * outer)
                : _outer{outer}
        {
                for(auto it1{params.begin()}, it2{args.begin()} ; it1 != params.end() && it2 != args.end() ; ++it1, ++it2)
                {
                        _map[it1->val] = *it2;
                }
        }

        Cell const& Env::get(std::string const& var)
        {
                try
                {
                        return _map.at(var);
                } catch(std::out_of_range const& e)
                {
                        if(_outer != nullptr)
                        {
                                return (*_outer)[var];
                        }

                        throw;
                }
        }

        Cell & Env::operator[](std::string const& var)
        {
                return _map[var];
        }

        std::list<std::string> tokenize(std::string const& s)
        {
                std::list<std::string> tokens;
                std::string token;

                for(auto c : s)
                {
                        if(utils::contains("()", c))
                        {
                                tokens.push_back(token);
                                tokens.push_back({c});
                                token = "";
                        }
                        else if(isspace(c))
                        {
                                tokens.push_back(token);
                                token = "";
                        }
                        else
                        {
                                token += c;
                        }
                }

                if(!token.empty())
                        tokens.push_back(token);

                tokens.erase(std::remove(tokens.begin(), tokens.end(), ""), tokens.end());

                return tokens;
        }

        std::vector<Cell> parse(std::string const& s)
        {
                auto tokens = tokenize(s);
                std::vector<Cell> cells;
                while(!tokens.empty())
                {
                        cells.push_back(parseFrom(tokens));
                }

                return cells;
        }

        Cell parseFrom(std::list<std::string> &tokens)
        {
                const std::string token{tokens.front()};
                tokens.pop_front();

                if(token == "(")
                {
                        Cell c{List};
                        while(tokens.front() != ")")
                        {
                                c.list.push_back(parseFrom(tokens));
                        }
                        tokens.pop_front(); // Remove the closing parenthesis

                        return c;
                }
                else
                {
                        return parseAtom(token);
                }
        }

        Cell parseAtom(std::string const& s)
        {
                Cell c;
                std::stringstream ss{s};
                Num num;
                std::string val;

                if(ss >> num)
                {
                        c.type = Number;
                        c.num = num;
                        return c;
                }

                c.val = s;
                return c;
        }

        // Evaluation stuff
        Interpreter::Interpreter()
        {
                // We just set a few primitives
                _env["nil"] = nil;
                _env["#t"] = trueSym;
                _env["#f"] = falseSym;
        }

        Cell Interpreter::eval(Cell const& c)
        {
                return eval(c, _env);
        }

        Cell Interpreter::eval(std::string const& s)
        {
                Cell c;
                auto cs{parse(s)};

                std::for_each(cs.begin(), cs.end(), [this](Cell & c){c = eval(c); });

                return cs[cs.size() - 1];
        }

        void Interpreter::replOnce()
        {
                using namespace std;
                string input;
                cout << "? ";
                getline(cin, input);
                cout << "result " << eval(input) << endl;
        }

        void Interpreter::repl()
        {
                while(true) replOnce();
        }

        Cell Interpreter::eval(Cell const& c, Env & env)
        {
                switch(c.type)
                {
                case Symbol:
                        return env.get(c.val);
                case Number:
                        return c;
                case List:
                {
                        // If it is the empty list return nil
                        if (c.list.empty())
                                return nil;

                        // We evaluate the first element
                        auto f = eval(c.list[0], env);

                        if(f.type == Symbol || f.type == Number || f.type == List)
                                throw std::runtime_error{"The first element is not of a function type"};

                        std::vector<Cell> args{c.list.begin() + 1, c.list.end()};
                        // If Proc or Lambda, evaluate the arguments
                        if(f.type == Proc || f.type == Lambda)
                        {
                                std::for_each(args.begin(), args.end(), [&env, this](Cell & c)
                                              {
                                                      c = eval(c, env);
                                              });
                        }

                        // The evaluation methods are different
                        if(f.type == Proc || f.type == SpeForm)
                                return (f.proc)(args);

                        auto const& params{f.list[1].list};
                        std::vector<Cell> const& body{f.list.begin() + 1, f.list.end()};

                        // Make sure there is exactly the same number of arguments given and received
                        if(params.size() != args.size())
                                throw std::runtime_error{"Number of arguments mismatch"};

                        // Now evaluate the body of the lambda / macro
                        // with a new environment given by the arguments
                        Env argEnv{params, args, f.env};

                        for(auto it = body.begin() ; it != body.end() - 1 ; ++it)
                        {
                                eval(*it, argEnv);
                        }

                        return eval(body[body.size() - 1]); // Return the value of the last sexpr in the body
                }
                case Proc:
                case SpeForm:
                case Lambda:
                case Macro:
                        throw std::runtime_error{"A function type is not evaluable"};
                }
        }
}

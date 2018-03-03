#ifndef LISP_HPP
#define LISP_HPP
#include <string>
#include <ostream>
#include <memory>
#include <vector>
#include <map>
#include <utility>
#include <list>
#include <functional>

namespace utils
{
        // Returns true if the string contains the character
        bool contains(std::string const& s, char c);
}

namespace lisp
{
        using Num = double; // For now we support only one numeric type

        enum CellType
        {
                Symbol
                , Number // Of type Num

                , List

                , Proc // Builtin function
                , SpeForm // Builtin special form (the arguments are not evaluated)

                , Lambda // User defined function
                , Macro // User defined macro
        };

        struct Env;

        struct Cell
        {
                using ProcType = std::function<Cell(std::vector<Cell> const&)>;

                CellType type;
                Num num;
                std::string val; // For the symbol, and later the string (also the name of processes and special forms)
                std::vector<Cell> list; // For the lists, the lambdas and the macros
                ProcType proc; // For the processes and special forms
                Env *env; // Closure for lambdas and macros

                Cell(CellType type, Num num, std::string const& s, std::vector<Cell> const& xs, ProcType const& proc, Env *env)
                        : type{type}, num{num}, val{s}, list{xs}, proc{proc}, env{env} {}

                Cell(CellType type = Symbol) : type{type}, env{nullptr} {}
                Cell(CellType type, std::string const& val) : type{type}, val{val}, env{nullptr} {}
                Cell(ProcType proc, CellType type = Proc) : type{type}, proc{proc}, env{nullptr} {}

                static Cell mkList(std::vector<Cell> const& xs) {return Cell{List, 0, "", xs, {}, nullptr};}

                bool operator==(Cell const& c) const;
                bool operator!=(Cell const& c) const {return !(*this == c);}
        };

        std::ostream &operator<<(std::ostream & o, Cell const& c);

        extern const Cell nil;
        extern const Cell trueSym;
        extern const Cell falseSym;


        struct Env
        {
                using Map = std::map<std::string, Cell>;

                Env(Env * outer = nullptr) : _outer{outer} {}
                Env(std::vector<Cell> const& params, std::vector<Cell> const& args, Env * outer = nullptr);

                Cell const& get(std::string const& var);
                Cell& operator[](std::string const& var);
        private:
                Env *_outer;
                Map _map;
        };

        // Parsing stuff
        std::list<std::string> tokenize(std::string const& s);
        std::vector<Cell> parse(std::string const& s);
        Cell parseFrom(std::list<std::string> &tokens);
        Cell parseAtom(std::string const& s);

        // Evaluation stuff
        class Interpreter
        {
        public:
                Interpreter();

                Cell eval(Cell const& c);
                Cell eval(std::string const& s);

                void replOnce();
                void repl();
        private:
                Cell eval(Cell const& c, Env & env);

                Cell evalLabel(std::vector<Cell> const& args, Env & env);
                Cell evalCond(std::vector<Cell> const& args, Env & env);

                static bool toBool(Cell const& c);

                Env _env; // The environment for global variables and functions
        };
}

#endif

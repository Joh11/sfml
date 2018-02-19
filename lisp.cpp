#include "lisp.hpp"

#include <cctype>
#include <algorithm>
#include <iostream>

namespace lisp
{
    Sexpr::Sexpr(Sexpr const& other)
    	: isAtom{other.isAtom}
    	, atom{other.atom}
    	, car{utils::copy_unique(car)}
	, cdr{utils::copy_unique(cdr)}
    {}

    Sexpr::Sexpr(bool isAtom, Atom const& atom, std::unique_ptr<Sexpr> const& car, std::unique_ptr<Sexpr> const& cdr)
    	: isAtom{isAtom}, atom{atom}, car{utils::copy_unique(car)}, cdr{utils::copy_unique(cdr)}
    {}

    Sexpr::Sexpr(bool isAtom, Atom const& atom, Sexpr const& car, Sexpr const& cdr)
	: isAtom{isAtom}
	, atom{atom}
	, car{std::make_unique<Sexpr>(car)}
	, cdr{std::make_unique<Sexpr>(cdr)}
    {}

    std::ostream & operator<<(std::ostream & o, Atom const& a)
    {
	switch(a.type)
	{
	case Atom::NUM:
	    return o << a.num;
	case Atom::SYM:
	    return o << a.sym;
	case Atom::STR:
	    return o << "\"" << a.str << "\"";
	}
    }

    // --------------------------------------------------

    std::ostream & operator<<(std::ostream &o, Sexpr const&s)
    {
	o << "test" << std::endl;
	if(s.isAtom)
	    return o << s.atom;
	if (!s.car || !s.cdr)
	    return o << "INVALD";
	return o << "(" << *(s.car) << " . " << *(s.cdr) << ")";
    }
    
    // --------------------------------------------------
    
    List<std::string> tokenize(std::string const& s)
    {
	List<std::string> tokens;
	std::string token;
	for(auto c : s)
	{
	    if(contains("().", c))
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

	// remove all empty tokens
	tokens.erase(std::remove(tokens.begin(), tokens.end(), ""), tokens.end());
	
	return tokens;
    }
    
    List<Sexpr> parse(std::string const& s)
    {
	auto tokens = tokenize(s);
	List<Sexpr> sexprs;
	
	while(!tokens.empty())
	{
	    for(auto const& s : tokens)
		std::cout << s << "/";
	    std::cout << std::endl;
	    sexprs.push_back(parseOne(tokens));
	}

	return sexprs;
    }

    Sexpr parseOne(List<std::string> & tokens)
    {
	if(tokens.front() == "(")
	{
	    tokens.pop_front();
	    if (tokens.front() == ")") // Then it is the empty list
	    {
		tokens.pop_front();
		return NIL;
	    }
	    
	    List<Sexpr> list;
	    list.push_back(parseOne(tokens));
	    if(tokens.front() == ".") // if dotted pair
	    {
		tokens.pop_front();
		list.push_back(parseOne(tokens));

		if(tokens.front() != ")")
		    throw std::runtime_error{"Expected a ')' to close the dotted pair"};

		// Remove the last ")"
		tokens.pop_front();
		return Sexpr::mkCons(list.front(), list.back());
	    }

	    // Else consume all sexprs until a ')' is seen
	    while(tokens.front() != ")")
		list.push_back(parseOne(tokens));

	    return mkList(list);
	}

	if(tokens.front() == "." || tokens.front() == ")")
	    throw std::runtime_error{"Expected a '(' or atom"};

	auto token = tokens.front();
	tokens.pop_front();
	return Sexpr::mkAtom(Atom::mkSym(token));
    }

    Sexpr mkList(List<Sexpr> list)
    {
	auto car = list.front();
	list.pop_front();
	return Sexpr::mkCons(car, mkList(list));
    }
}

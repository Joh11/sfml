#ifndef LISP_HPP
#define LISP_HPP

#include <string>
#include <ostream>
#include <memory>
#include <vector>
#include <map>
#include <utility>
#include <list>

namespace utils
{
    template <typename T>
    std::unique_ptr<T> copy_unique(const std::unique_ptr<T>& source)
    {
	return source ? std::make_unique<T>(*source) : nullptr;
    }
}

namespace lisp
{
    using Num = double;
    template <typename T> using List = std::list<T>;
    
    struct Atom
    {
	enum {NUM, SYM, STR} type;
	
	Num num;
	std::string sym;
	std::string str;

	static Atom mkNum(Num const& num) {return {NUM, num, "", ""}; }
	static Atom mkSym(std::string const& sym) {return {SYM, 0, sym, ""}; }
	static Atom mkStr(std::string const& str) {return {STR, 0, "", str}; }
    };

    std::ostream & operator<<(std::ostream &, Atom const&);

    const Atom EMPTY_ATOM = {Atom::SYM, 0, "", ""};
    
    struct Sexpr
    {
	bool isAtom;

	Atom atom;
	std::unique_ptr<Sexpr> car, cdr;

	// Copy constructor with deep copy
	Sexpr(Sexpr const&);
	Sexpr(bool, Atom const&, std::unique_ptr<Sexpr> const&, std::unique_ptr<Sexpr> const&);
	Sexpr(bool, Atom const&, Sexpr const& car, Sexpr const& cdr);

	static Sexpr mkAtom(Atom const& atom) {return {true, atom, {nullptr}, {nullptr}}; }
	static Sexpr mkCons(Sexpr const& car, Sexpr const& cdr) {return {false, EMPTY_ATOM, car, cdr}; }
    };

    std::ostream & operator<<(std::ostream &, Sexpr const&);

    const Sexpr NIL = {true, {Atom::NUM, 0, "NIL", ""}, {nullptr}, {nullptr}};

    inline bool contains(std::string const& s, char c) {return s.find(c) != s.npos; }

    List<std::string> tokenize(std::string const& s);

    List<Sexpr> parse(std::string const& s);
    Sexpr parseOne(List<std::string> & tokens);
    Sexpr mkList(List<Sexpr>);
}

#endif

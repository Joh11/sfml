#include <algorithm>

#include "lisp.hpp"
#include "primitives.hpp"

using namespace lisp;


Cell cons(std::vector<Cell> const& xs)
{
        if(xs.empty())
                return nil;

        return Cell::mkList(xs);
}

Cell car(std::vector<Cell> const& xs)
{

        return xs[0].list[0];
}

Cell cdr(std::vector<Cell> const& xs)
{
        auto x = xs[0];

        if(x.type != List || x.list.empty())
                throw std::runtime_error{"You can only cdr a nonempty list"};

        if(x.list.size() == 1)
                return nil;

        return Cell::mkList(std::vector<Cell>{x.list.begin() + 1, x.list.end()});
}

Cell eqQestionMark(std::vector<Cell> const& xs)
{
        auto const& x = xs[0];
        auto const& y = xs[1];

        return (x == y) ? trueSym : falseSym;
}

Cell atomQuestionMark(std::vector<Cell> const& xs)
{
        return (xs[0].type != List) ? trueSym : falseSym;
}

Cell quote(std::vector<Cell> const& xs)
{
        if(xs.empty())
                throw std::runtime_error{"Quoting no argument"};

        return xs[0];
}

// Cell cond(std::vector<Cell> const& xs);
// Cell lambda(std::vector<Cell> const& xs);
// Cell label(std::vector<Cell> const& xs);

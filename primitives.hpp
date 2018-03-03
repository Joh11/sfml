#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include "lisp.hpp"

lisp::Cell cons(std::vector<lisp::Cell> const& xs);
lisp::Cell car(std::vector<lisp::Cell> const& xs);
lisp::Cell cdr(std::vector<lisp::Cell> const& xs);
lisp::Cell eqQuestionMark(std::vector<lisp::Cell> const& xs);
lisp::Cell atomQuestionMark(std::vector<lisp::Cell> const& xs);

lisp::Cell quote(std::vector<lisp::Cell> const& xs);
// lisp::Cell cond(std::vector<lisp::Cell> const& xs);
// lisp::Cell lambda(std::vector<lisp::Cell> const& xs);
// lisp::Cell label(std::vector<lisp::Cell> const& xs);

#endif

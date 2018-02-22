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
        bool contains(std::string const& s, char c);
}

namespace lisp
{
        using Num = double;

        enum CellType
        {
                Symbol, Number, List, Proc, Lambda
        };

        struct Env;

        struct Cell
        {
                using ProcType = std::function<Cell(std::vector<Cell>)>;

                CellType type;
                Num num;
                std::string val;
                std::vector<Cell> list;
                ProcType proc;
                Env *env;

                Cell(CellType type = Symbol) : type{type}, env{nullptr} {}
                Cell(CellType type, std::string const& val) : type{type}, val{val}, env{nullptr} {}
                Cell(ProcType proc) : type{Proc}, proc{proc}, env{nullptr} {}
        };

        std::ostream &operator<<(std::ostream & o, Cell const& c);

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

                Env _env; // The environment for global variables and functions
        };
}

#endif

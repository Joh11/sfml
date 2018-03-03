// A simple program using SFML to plot data

#include <iostream>
#include <stdexcept>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "lisp.hpp"

using namespace sf;

VideoMode getBestVideoMode();

int main(int argc, char *argv[])
{
        std::cout << "SFML version : ";
        std::cout << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

        lisp::Interpreter i;

        if(argc == 2)
        {
                // Load the given file
                std::ifstream file{argv[1]};
                std::string fileStr, lineStr;

                while(std::getline(file, lineStr))
                        fileStr += lineStr;

                std::cout << "FILE LOADED : " << i.eval(fileStr) << std::endl;
        }

        i.repl();

        // {
        // 	using namespace lisp;
        // 	auto sexpr = Sexpr::mkCons(Sexpr::mkAtom(Atom::mkStr("test")), Sexpr::mkAtom(Atom::mkSym("test")));
        // 	std::cout << sexpr;
        // }

        RenderWindow win(getBestVideoMode(), "SFML Works", Style::Fullscreen);

        while (win.isOpen())
        {
                Event e;

                while (win.pollEvent(e))
                {
                        if (e.type == Event::Closed)
                                win.close();
                }

                win.clear();

                win.display();
        }

        return EXIT_SUCCESS;
}

VideoMode getBestVideoMode()
{
        auto fsModes = VideoMode::getFullscreenModes();

        if (fsModes.empty())
                throw std::runtime_error("No fullscreen mode available");

        auto mode = fsModes[0];

        if (!mode.isValid())
                throw std::runtime_error("The fullscreen mode is not valid");

        return mode;
}

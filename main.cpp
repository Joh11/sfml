// A simple program using SFML to plot data

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "lisp.hpp"

using namespace sf;

VideoMode getBestVideoMode();

int main()
{
    std::cout << "SFML version : ";
    std::cout << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    std::string str;
    std::getline(std::cin, str);
    
    for(auto const& s : lisp::tokenize(str))
    {
    	std::cout << "-" << s;
    }

    std::cout << "TEST" << std::endl;

    auto tab = lisp::parse(str);
    for(auto const& s : tab)
    {
    	std::cout << s << std::endl;
    }

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

#include "Menu.hpp"
#include<iostream>
#define SFML_NO_DEPRECATED_WARNINGS

using namespace std;


Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("Fonts/BRITANIC.TTF"))
	{
		cout << "error loading file" << endl;
	}

	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Manual");
	menu[0].setPosition(sf::Vector2f(width / 2, height / (MAX_ITEMS + 1) * 1));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("Solar System");
	menu[1].setPosition(sf::Vector2f(width / 2, height / (MAX_ITEMS + 1) * 2));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Exit");
	menu[2].setPosition(sf::Vector2f(width / 2, height / (MAX_ITEMS + 1) * 3));

	selectedItemIndex = 0;
}

Menu::~Menu()
{

}

void Menu::draw(sf::RenderWindow &window)
{
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		window.draw(menu[i]);
	}

}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < MAX_ITEMS)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}
}
int Menu::menudraw()
{
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "Planetary Simulation");
    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Up:
                    menu.MoveUp();
                    break;
                case sf::Keyboard::Down:
                    menu.MoveDown();
                    break;
                case sf::Keyboard::Return:
                    switch (menu.GetPressedItem())
                    {
                    case 0:
                        cout << "Manual Pressed" << endl;
                        return 1;
                        break;
                    case 1:
                        cout << "Solar system Pressed" << endl;
                        return 2;
                        break;
                    case 2:
                        cout << "Exit Pressed" << endl;
                        exit(1);
                        break;
                    default:
                        break;
                    }
                }
                break;


            case sf::Event::Closed:
                window.close();
                break;

            }
        }
        window.clear();

        menu.draw(window);

        window.display();
    }
}
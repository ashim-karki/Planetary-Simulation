#pragma once
#include "SFML/Graphics.hpp"

#define MAX_ITEMS 3
class Menu
{
public:
	Menu(float width, float height);
	~Menu();

	void draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	int GetPressedItem() { return selectedItemIndex; }
	int menudraw();

    
private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_ITEMS];
};


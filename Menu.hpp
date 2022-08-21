#pragma once
#include "SFML/Graphics.hpp"

#define MAX_ITEMS 3
class Menu //add bg sprite here
{
public:
	Menu(float width, float height);
	~Menu(); // is the destructor working?

	void draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	int GetPressedItem() { return selectedItemIndex; }
	int menudraw();
	sf::RectangleShape rect;
	sf::Texture text;
	sf::Sprite background;

    
private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_ITEMS];
};


#pragma once
#include <SFML/Graphics.hpp>
#include <utility>

using std::pair;
using sf::Color;
using sf::RectangleShape;

struct Tile {
	int danger = 1;
	int priorDanger = 1;
	bool isChecked = false;
	bool userPlaced = false;
	bool visitedA = false;
	bool visitedD = false;

	pair<int, int> parent;
	Color priorColor;
	RectangleShape box;
	Tile(float x = 0.0, float y = 0.0) {
		box.setSize({ 32.0f, 32.0f });
		box.setPosition({ x * 32.0f, y * 32.0f });
		box.setFillColor(Color(179, 226, 255, 125));
		priorColor = Color(179, 226, 255, 125);
	}
};



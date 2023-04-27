#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
using namespace sf;
using namespace std;
// represents a button on the bottom of the window
struct Button {
	RectangleShape button;
	Text text;
	bool isClicked = false;
	Button(unsigned int width, unsigned int height, int posX = 0, int posY = 0, Color fillColor = Color::White, Color outlineColor = Color::Black, float outlineThickness = 0.0f) {
		button.setSize(Vector2f(width, height));
		button.setPosition(posX, posY);
		button.setFillColor(fillColor);
		button.setOutlineColor(outlineColor);
		button.setOutlineThickness(outlineThickness);
	}
	RectangleShape& operator()() {
		return button;
	}
	// handles a buttone being clicked, accepts a callback function to run after, this can be expensive though if you render too often
	void click(RenderWindow& window, std::function<void(RenderWindow& win)> callback = nullptr) {

		if (!isClicked) {
			isClicked = true;

			Color oldColor = button.getFillColor();
			button.setFillColor(Color(210, 210, 210));

			window.draw(button);
			window.draw(text);
			window.display();

			if (!callback) {

				button.setFillColor(oldColor);
				window.draw(button);
				window.draw(text);
				sleep(milliseconds(10));
				window.display();
			}
			else {
				callback(window);
			}

			isClicked = false;
		}

	}

	void draw(RenderWindow& window) {
		window.draw(button);
		window.draw(text);
	}

	void setText(string& buttonText, Font& font, unsigned int fontSize = 16, Color textColor = Color(25, 25, 25), Text::Style style = Text::Bold, int posX = 0, int posY = 0) {
		text.setString(buttonText);
		text.setFont(font);
		text.setCharacterSize(fontSize);
		text.setFillColor(textColor);
		text.setStyle(style);
		text.setPosition(posX, posY);
	}
	void setText(Text textObj) {
		text = textObj;
	}
};

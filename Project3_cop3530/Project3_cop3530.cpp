#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <random>
#include <vector>
#include <sstream>
#include <queue>
#include <climits>
#include <utility>

#include "Random.h"
#include "Tile.h"
#include "TileMap.h"
#include "Button.h"

using namespace std;
using namespace sf;


int main() {

	// load button fonts
	Font font;
	if (!font.loadFromFile("fonts/CASCADIACODE.TTF"))
	{
		cout << "font could not load..." << endl;
	}

	// load texture images
	string gatorPath = "images/gator.png";
	Texture gatorTexture;
	if (!gatorTexture.loadFromFile(gatorPath)) {
		cout << "cannot load gator-lg.png texture" << endl;
	}

	string fishPath = "images/fish.png";
	Texture fishTexture;
	if (!fishTexture.loadFromFile(fishPath)) {
		cout << "cannot load fish texture" << endl;
	}

	// used for styling the cursor
	Cursor pointer, arrow;
	pointer.loadFromSystem(Cursor::Hand);
	arrow.loadFromSystem(Cursor::Arrow);

	vector<Color> blueColors{
			Color(134, 206, 250), // lighter
			Color(115, 185, 238),
			Color(84, 148, 218),
			Color(51, 115, 196),
			Color(23, 80, 172),
			Color(0, 33, 165),

	};

	// window information
	int windowWidth = 1344;
	int windowHeight = 868;
	int i = 0;
	int j = 0;

	// determin number of rows and columns based on 32x32 tiles
	int numRows = (windowHeight - 100) / 32;
	int numCols = windowWidth / 32;

	// main tile map of the game
	TileMap tiles(gatorTexture, fishTexture, numRows, numCols);

	cout << "\nMap Loaded" << endl;
	cout << "Num Rows: " << numRows << endl;
	cout << "Num Cols: " << numCols << endl;
	cout << "Total Cells: " << numCols * numRows << endl;

	// create new render window to draw in
	RenderWindow window(VideoMode(windowWidth, windowHeight),
		"shortest path", Style::Close);

	// create new rectangle shape to serve as background panel
	RectangleShape windowBackground(
		Vector2f(windowWidth, windowHeight - 100));


	unsigned int buttonWidth = 168;
	unsigned int buttonHeight = 100;
	unsigned int fontSize = 16;

	Text::Style bold = Text::Bold;
	Color neutral = Color(84, 115, 135);
	Color grey = Color(192, 192, 192);
	Color gatorBlue = Color(0, 33, 165);

	// hazard button
	string hazardTxt = "  ADD\nHAZARDS";
	int hazardTextX = 1222;
	int hazardTextY = windowHeight - 70;
	int hazardX = 1176;
	int hazardY = windowHeight - 100;

	Button addHazardsBtn(buttonWidth, buttonHeight, hazardX, hazardY, grey, gatorBlue, -1.0f);
	addHazardsBtn.setText(hazardTxt, font, fontSize, gatorBlue, bold, hazardTextX, hazardTextY);

	// clear hazard button
	string clearText = "\tCLEAR\n   HAZARDS";
	int clearTextX = 1024;
	int clearTextY = windowHeight - 70;
	int clearX = 1008;
	int clearY = windowHeight - 100;

	Button clearHazardsBtn(buttonWidth, buttonHeight, clearX, clearY, grey, gatorBlue, -1.0f);
	clearHazardsBtn.setText(clearText, font, fontSize, gatorBlue, bold, clearTextX, clearTextY);

	// RandomizeMap button
	string randomText = " RANDOMIZE\n   TILES";
	int randomTextX = 868;
	int randomTextY = windowHeight - 70;
	int randomX = 840;
	int randomY = windowHeight - 100;

	Button randomizeMapBtn(buttonWidth, buttonHeight, randomX, randomY, grey, gatorBlue, -1.0f);
	randomizeMapBtn.setText(randomText, font, fontSize, gatorBlue, bold, randomTextX, randomTextY);

	// Dijkstra button
	string dText = " PATHFIND \nDIJKSTRA'S";
	int dTextX = 704;
	int dTextY = windowHeight - 70;
	int dX = 672;
	int dY = windowHeight - 100;

	Button dijkstraBtn(buttonWidth, buttonHeight, dX, dY, grey, gatorBlue, -1.0f);
	dijkstraBtn.setText(dText, font, fontSize, gatorBlue, bold, dTextX, dTextY);

	// astar button
	string aText = " PATHFIND\n A STAR'S";
	int aTextX = 535;
	int aTextY = windowHeight - 70;
	int aX = 504;
	int aY = windowHeight - 100;

	Button astarBtn(buttonWidth, buttonHeight, aX, aY, grey, gatorBlue, -1.0f);
	astarBtn.setText(aText, font, fontSize, gatorBlue, bold, aTextX, aTextY);

	// neighbor button
	string neighborText = "  DIRECTION \n MODE TOGGLE";
	int neighborTextX = 354;
	int neighborTextY = windowHeight - 70;
	int neighborX = 336;
	int neighborY = windowHeight - 100;

	Button neighborBtn(buttonWidth, buttonHeight, neighborX, neighborY, grey, gatorBlue, -1.0f);
	neighborBtn.setText(neighborText, font, fontSize, gatorBlue, bold, neighborTextX, neighborTextY);

	// reset and clear paths button
	string pathText = " CLEAR PATHS\n  AND RESET";
	int pathTextX = 187;
	int pathTextY = windowHeight - 70;
	int pathX = 168;
	int pathY = windowHeight - 100;

	Button pathBtn(buttonWidth, buttonHeight, pathX, pathY, grey, gatorBlue, -1.0f);
	pathBtn.setText(pathText, font, fontSize, gatorBlue, bold, pathTextX, pathTextY);

	// reset but don't clear paths
	string resetText = "  RESET SOURCE\n\tLOCATION";
	int resetTextX = 4;
	int resetTextY = windowHeight - 70;
	int resetX = 0;
	int resetY = windowHeight - 100;

	Button resetBtn(buttonWidth, buttonHeight, resetX, resetY, grey, gatorBlue, -1.0f);
	resetBtn.setText(resetText, font, fontSize, gatorBlue, bold, resetTextX, resetTextY);

	// limit frame rate and enable vertical sync
	//window.setVerticalSyncEnabled(true);

	// set the frame rate limit
	//window.setFramerateLimit(60);

	// basic state of board
	int randX;
	int randY;
	bool randomToggle = false;
	bool pressing = false;
	bool toggleLock = false;
	bool show = false;
	bool isHandling = false;


	// while the window is open
	while (window.isOpen()) {
		// event
		Event event;

		// poll for events
		while (window.pollEvent(event)) {

			// disregard clicks when window has no focus
			if (window.hasFocus())

				window.setMouseCursor(arrow);

			switch (event.type) {

			// close window
			case Event::Closed:
				window.close();
				break;

			// mouse pressed
			case Event::MouseButtonPressed:

				// left press
				if (event.mouseButton.button == Mouse::Left) {

					if (!isHandling) {

						isHandling = true;

						// get mouse position
						auto mp = Mouse::getPosition(window);
						int x = mp.x / 32.0f;
						int y = mp.y / 32.0f;

						// inside the tile section of the board
						if (y < numRows && x < numCols) {

							//cout << "\nUser clicked inside the board: (" << y << ", " << x << ")" << endl;

							while (Mouse::isButtonPressed(Mouse::Left)) {

								auto mp = Mouse::getPosition(window);
								x = mp.x / 32.0f;
								y = mp.y / 32.0f;

								if (y < numRows && x < numCols && x >= 0 && y >= 0) {
									auto& t = tiles(y, x);

									// flip user placed condition
									t.userPlaced = true;
									t.danger = 0;

									// color according to userPlaced status
									t.box.setFillColor(Color(186, 170, 135));
									if (t.box.getOutlineColor() == Color(250, 70, 22) || t.box.getOutlineColor() == Color(250, 70, 22)) {
										t.box.setOutlineColor(Color(25, 25, 25));
										t.box.setOutlineThickness(-0.5f);
										tiles.clearDijkstra();
										tiles.clearAStar();
									}
									// immediately render to avoid any lag when clicking and marking
									window.draw(t.box);
									sf::sleep(milliseconds(2));
									window.display();

								}
							}
						}

						// the bottom tool bar of the board
						else {

							// swap to the pointer cursor 
							window.setMouseCursor(pointer);

							// check for button presses on all bottom buttons
							if (!pressing && event.type != sf::Event::MouseButtonReleased) {
								pressing = true;
								if (!toggleLock && addHazardsBtn().getGlobalBounds().contains(x * 32.0f, y * 32.0f)) {
									cout << "\nUser clicked Add Hazards button:\nHazards randomly generating until user clicks again. (" << y << ", " << x << ")" << endl;
									addHazardsBtn.click(window);
									toggleLock = true;
									randomToggle = !randomToggle;
								}
								else if (clearHazardsBtn().getGlobalBounds().contains(x * 32.0f, y * 32.0f)) {
									cout << "\nUser clicked Clear Hazards button:\n(" << y << ", " << x << ")" << endl;
									clearHazardsBtn.click(window);
									tiles.clearHazards();
									tiles.clearDijkstra();
									tiles.clearAStar();

								}
								else if (randomizeMapBtn().getGlobalBounds().contains(x * 32.0f, y * 32.0f)) {
									cout << "\nUser clicked RandomizeColorMap() button:\nNew random map generated. (" << y << ", " << x << ")" << endl;
									randomizeMapBtn.click(window);
									tiles.randomizeColorMap();
								}
								else if (dijkstraBtn().getGlobalBounds().contains(x * 32.0f, y * 32.0f)) {
									cout << "\nUser clicked Solve Dijkstra() button:\nPath Finding in progress. (" << y << ", " << x << ")" << endl;
									auto callback = [&tiles, &window](RenderWindow& rw) {tiles.solveDijkstra(rw); };
									if (!dijkstraBtn.isClicked && !tiles.dSolved) {
										Color oldColor = dijkstraBtn.button.getFillColor();
										dijkstraBtn.click(window, callback);
										dijkstraBtn.button.setFillColor(oldColor);
									}
								}
								else if (astarBtn().getGlobalBounds().contains(x * 32.0f, y * 32.0f)) {
									cout << "\nUser clicked Solve A*() button:\nPath Finding in progress. (" << y << ", " << x << ")" << endl;
									auto callback = [&tiles, &window](RenderWindow& rw) {tiles.solveAStar(rw); };
									if (!astarBtn.isClicked && !tiles.aSolved) {
										Color oldColor = astarBtn.button.getFillColor();
										astarBtn.click(window, callback);
										astarBtn.button.setFillColor(oldColor);
									}
								}
								else if (neighborBtn().getGlobalBounds().contains(x * 32.0f, y * 32.0f)) {
									cout << "\nUser clicked Neighbor Change button:";
									neighborBtn.click(window);
									tiles.toggleNumNeighbors();
									int num = tiles.fourNeighbors ? 4 : 8;
									cout << "\nDiagonal movement " << ((num == 4) ? "disabled: " : "enabled: ");
									cout << num << " neighbors will be considered. (" << y << ", " << x << ")" << endl;
								}
								else if (pathBtn().getGlobalBounds().contains(x * 32.0f, y * 32.0f)) {
									cout << "\nUser clicked ClearPath() button:\nPath results cleared. (" << y << ", " << x << ")" << endl;
									pathBtn.click(window);
									tiles.resetPaths();
									tiles.resetSourceAndDestination();
								}
								else if (resetBtn().getGlobalBounds().contains(x * 32.0f, y * 32.0f)) {
									cout << "\nUser clicked ResetSourceAndDestination() button:\nStarting positions reset. (" << y << ", " << x << ")" << endl;
									resetBtn.click(window);
									tiles.resetSourceAndDestination();
								}
							}
							pressing = false;
						}

						isHandling = false;

					}
				}
				else if (event.mouseButton.button == Mouse::Right) {

					while (Mouse::isButtonPressed(Mouse::Right)) {

						// get mouse position
						auto mp = Mouse::getPosition(window);
						int x = mp.x / 32.0f;
						int y = mp.y / 32.0f;
						//cout << "\nUser right clicked inside the board: (" << y << ", " << x << ")" << endl;

						// inside the tile section of the board
						if (y < numRows && x < numCols && y >= 0 && x >= 0) {
							auto& t = tiles(y, x);

							// flip user placed condition
							if (t.userPlaced) {
								t.userPlaced = false;
								t.danger = t.priorDanger;

								// path could now be available
								tiles.clearDijkstra();
								tiles.clearAStar();


							}
							pair<int, int> p{ y, x };
							if (tiles.source != p && tiles.destination != p) {
								t.box.setFillColor(t.priorColor);
							}
							else {
								cout << "source or dest" << endl;

							}

							// immediately render to avoid any lag when clicking and marking
							window.draw(t.box);
							window.display();
						}
					}
				}
				break;

			case Event::MouseMoved:
				// get mouse position
				auto mp = Mouse::getPosition(window);
				int x = mp.x / 32.0f;
				int y = mp.y / 32.0f;

				if (y >= numRows) {
					window.setMouseCursor(pointer);

				}

				break;

			}

			// things to do at end of event loop
			toggleLock = false;
		}


		window.clear();

		// draw background and controls
		window.draw(windowBackground);

		// randomly place hazards
		if (randomToggle) {

			while (true) {

				// find random locations to place the hazards
				randX = Random::Int(0, numCols - 1);
				randY = Random::Int(0, numRows - 1);

				if (!tiles(randY, randX).userPlaced) {
					break;
				}
			}

			if (tiles.source.first != randX
				&& tiles.source.second != randX
				&& tiles.destination.first != randX
				&& tiles.destination.second != randX) {
				tiles(randY, randX).box.setFillColor(Color(186, 170, 135));
				tiles(randY, randX).danger = 0;
				tiles(randY, randX).userPlaced = true;
			}
		}


		// draw tile map (grid)
		tiles.draw(window);

		// draw buttons
		addHazardsBtn.draw(window);
		clearHazardsBtn.draw(window);
		randomizeMapBtn.draw(window);
		dijkstraBtn.draw(window);
		astarBtn.draw(window);
		neighborBtn.draw(window);
		pathBtn.draw(window);
		resetBtn.draw(window);

		// display window
		window.display();

	}

	return 0;
}

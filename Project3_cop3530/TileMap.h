#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <random>
#include <sstream>
#include <queue>
#include <climits>
#include <vector>
#include <utility>
#include "Random.h"
#include "Tile.h"
using namespace sf;
using namespace std;

struct TileMap {

	// grid of tiles
	vector<vector<Tile>> grid;

	// destination and source tiles
	pair<int, int> source = { 0, 0 };
	pair<int, int> destination = { 0, 0 };

	// used to track state of path finding
	bool dSolved = false;
	bool aSolved = false;

	// used to check if there are 4 or 8 directions being considered for movement
	bool fourNeighbors = true;

	// sprites for gator and fish img
	Sprite gatorImg;
	Sprite fishImg;

	// used to track dijkstra distances, parents, and path
	vector<vector<int>> dDist;
	vector<vector<pair<int, int>>> dParent;
	vector <pair<int, int>> dPath;

	// used to track a star distance, parents, and path
	vector<vector<int>> aDist;
	vector<vector<pair<int, int>>> aParent;
	vector <pair<int, int>> aPath;

	// constructor
	TileMap(Texture& gatorTexture, Texture& fishTexture, float numRows = 0.0f, float numCols = 0.0f) {
		for (auto i = 0; i < numRows; i++) {
			vector<Tile> row;
			for (auto j = 0; j < numCols; j++) {
				Tile newTile(j, i);
				row.push_back(newTile);
			}
			grid.push_back(row);
		}

		destination = { numRows - 1 , numCols - 1 };

		gatorImg.setPosition({ 0, 0 });
		gatorImg.setTexture(gatorTexture);

		fishImg.setPosition({ (numCols - 1) * 32, (numRows - 1) * 32 });
		fishImg.setTexture(fishTexture);

		setColorMap();
		colorSrcAndDest();
	}
	// functor to return tile at given index
	Tile& operator()(int x, int y) {
		return grid[x][y];
	}
	RectangleShape& boxAt(int x, int y) {
		return grid[x][y].box;
	}
	const unsigned int numRows() {
		return grid.size();
	}
	const unsigned int numCols() {
		return grid.at(0).size();
	}
	// draws all the tiles to the board
	void draw(RenderWindow& window) {
		for (auto i = 0; i < numRows(); i++) {
			for (auto j = 0; j < numCols(); j++) {
				//if (grid[i][j].visited) {
				//	//grid[i][j].box.setFillColor(Color(0, 0, 235));
				//	grid[i][j].box.setOutlineColor(Color(255, 20, 20));
				//	grid[i][j].box.setOutlineThickness(-1.0f);
				//}
				window.draw(grid[i][j].box);
			}
		}
		window.draw(grid[source.first][source.second].box);
		window.draw(grid[destination.first][destination.second].box);
		if (gatorImg.getPosition() != fishImg.getPosition()) {
			window.draw(fishImg);
		}
		window.draw(gatorImg);

	}

	// clears any user set or randomly generated obstacles on the board
	void clearHazards() {
		for (auto i = 0; i < numRows(); i++) {
			for (auto j = 0; j < numCols(); j++) {
				if (grid[i][j].userPlaced) {
					grid[i][j].box.setFillColor(grid[i][j].priorColor);
					grid[i][j].danger = grid[i][j].priorDanger;
					grid[i][j].userPlaced = false;
				}
			}
		}
	}
	void setColorMap() {

		// blues
		vector<Color> colors{
			Color(0, 48, 135), // darker
			Color(23, 80, 172),
			Color(51, 115, 196),
			Color(84, 148, 218),
			Color(115, 185, 238), // lighter is worse since gator is more easily seen
		};

		int rand = 0;

		for (auto& row : grid) {
			for (auto& tile : row) {
				rand = Random::Int(0, 4);

				tile.danger = rand + 1;
				tile.priorDanger = tile.danger;

				tile.box.setOutlineColor(Color(25, 25, 25));
				tile.box.setOutlineThickness(-0.5f);

				tile.visitedD = false;
				tile.visitedA = false;
				tile.box.setFillColor(colors[rand]);
				tile.priorColor = colors[rand];
			}
		}
	}
	void randomizeSrcAndDest() {
		if (grid.size()) {

			source.first = Random::Int(0, grid.size() - 1);
			source.second = Random::Int(0, grid[0].size() - 1);
			destination.first = Random::Int(0, grid.size() - 1);
			destination.second = Random::Int(0, grid[0].size() - 1);
			gatorImg.setPosition(source.second * 32.0f, source.first * 32.0f);
			fishImg.setPosition(destination.second * 32.0f, destination.first * 32.0f);
			colorSrcAndDest();
		}

	}
	// colors the source and destination vertices properly
	void colorSrcAndDest() {
		if (source.first < grid.size() && source.second < grid[0].size()
			&& destination.first < grid.size() && destination.second < grid[0].size()) {
			grid[source.first][source.second].box.setFillColor(Color(250, 70, 22));
			grid[destination.first][destination.second].box.setFillColor(Color(250, 70, 22));
		}
	}
	// clear any visited tile
	void clearVisited() {
		for (auto& row : grid) {
			for (auto& tile : row) {
				tile.visitedA = false;
				tile.visitedD = false;
				tile.box.setOutlineColor(Color(25, 25, 25));
				tile.box.setOutlineThickness(-0.5f);
			}
		}
	}

	// clears the visited list of tiles vistied during astar
	void clearVisitedA() {
		for (auto& row : grid) {
			for (auto& tile : row) {
				if (!tile.visitedD) {
					tile.box.setOutlineColor(Color(25, 25, 25));
					tile.box.setOutlineThickness(-0.5f);
				}
				tile.visitedA = false;
			}
		}
	}

	// clears the visited list of tiles visited during dijkstras
	void clearVisitedD() {
		for (auto& row : grid) {
			for (auto& tile : row) {
				tile.visitedD = false;
				tile.box.setOutlineColor(Color(25, 25, 25));
				tile.box.setOutlineThickness(-0.5f);
			}
		}
	}

	// gets a new random map with random source and destination vertices
	void randomizeColorMap() {
		clearDijkstra();
		clearAStar();
		clearHazards();
		setColorMap();
		randomizeSrcAndDest();
	}

	// resets the position of the gator sprite back to the source
	void resetSourceAndDestination() {
		gatorImg.setPosition(source.second * 32.0f, source.first * 32.0f);
		clearDijkstra();
		clearAStar();
	}

	// makes sure a row and col are in bounds and not obstacles with no danger
	bool isValid(int row, int col) {
		return row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size() && grid[row][col].danger != 0;
	}

	// changed the number of neighboring tiles considered when moving
	void toggleNumNeighbors() {
		fourNeighbors = !fourNeighbors;
	}

	// solve the dijkstra algorithm
	void solveDijkstra(RenderWindow& window) {
		if (!dSolved) {
			dPath.clear();
			dijkstra();
			calcDPath(window);
			dSolved = true;
		}
	}

	// solve the a star algorithm
	void solveAStar(RenderWindow& window) {
		if (!aSolved) {
			aPath.clear();
			clearVisitedA();
			aStar();
			calcAPath(window);
			aSolved = true;
		}
	}

	// resets the paths that are found when solving pathfind algos
	void resetPaths() {
		for (auto& row : grid) {
			for (auto& tile : row) {
				if (!tile.userPlaced) {
					tile.box.setFillColor(tile.priorColor);

				}
				tile.box.setOutlineColor(Color(25, 25, 25));
				tile.box.setOutlineThickness(-0.5f);
				tile.visitedA = false;
				tile.visitedD = false;
			}
		}
		//preserve source and destination colors
		grid[source.first][source.second].box.setFillColor(Color(250, 70, 22));
		grid[destination.first][destination.second].box.setFillColor(Color(250, 70, 22));
	}

	// clear solved status and path for dijkstras
	void clearDijkstra() {
		dSolved = false;
		dPath.clear();
	}

	// clear solved status and path for astar
	void clearAStar() {
		aSolved = false;
		clearVisitedA();
		aPath.clear();
		aParent.clear();
	}

	// dijkstras with priority queue min heap to get O(VlogV) E proportional to V here.
	void dijkstra() {

		// horrendous name, but what else are we going to call this beast?
		using pairIntPairInts = pair<int, pair<int, int>>;

		// min heap priority queue used to store the cells to check
		priority_queue<pairIntPairInts, vector<pairIntPairInts>, greater<pairIntPairInts>> pq;

		// initialize the distance as max and parent vectors
		dDist = { grid.size(), vector<int>(grid[0].size(), INT_MAX) };
		dParent = { grid.size(), vector<pair<int, int>>(grid[0].size(), {-1, -1}) };

		// set initial distance to 0
		dDist[source.first][source.second] = 0;

		// add source to queue
		pq.push({ 0, source });

		// keep looping until nothing left in queue
		while (!pq.empty()) {

			// pop the distances off after getting info
			int d = pq.top().first;
			int row = pq.top().second.first;
			int col = pq.top().second.second;

			pq.pop();

			if (row == destination.first && col == destination.second)
				break;

			if (d > dDist[row][col])
				continue;

			// directional neighbors lists
			vector<int>dr;
			vector<int>dc;

			// check which neighbor mode we're in
			if (fourNeighbors) {
				dr = { -1, 0, 1, 0 };
				dc = { 0, 1, 0, -1 };
			}
			else {
				dr = { -1, -1, -1, 0, 0, 1, 1, 1 };
				dc = { -1, 0, 1, -1, 1, -1, 0, 1 };
			}

			int numNeighbors = fourNeighbors ? 4 : 8;

			for (int i = 0; i < numNeighbors; i++) {

				int r = row + dr[i];
				int c = col + dc[i];

				// check if the row and col are valid positions
				if (isValid(r, c)) {
					grid[r][c].visitedD = true;

					// check if the box isn't a destination box marked orange
					if (grid[r][c].box.getFillColor() != Color(250, 70, 22))
						grid[r][c].box.setFillColor(Color(34, 136, 76));

					// set the exposure level (the danger of gabby being seen)
					int exposure = grid[r][c].danger;

					// less exposed route
					if (dDist[row][col] + exposure < dDist[r][c]) {

						// add distance list
						dDist[r][c] = dDist[row][col] + exposure;

						// add to parent list
						dParent[r][c] = { row, col };

						// add to queue
						pq.push({ dDist[r][c], {r, c} });
					}
				}
			}
		}
	}

	// calculates the path of dijkstras solution
	void calcDPath(RenderWindow& window) {
		//sleep(milliseconds(1));

		//vector<pair<int, int>> path;
		if (dParent[destination.first][destination.second] == make_pair(-1, -1)) {
			cout << "\n--------------------------------" << endl;
			cout << "Dijkstras Algorithm Results:" << endl;
			cout << "No valid path found." << endl;
			cout << "--------------------------------" << endl;
			return;
		}

		// add items to the path
		for (auto curr = destination; curr != source; curr = dParent[curr.first][curr.second]) {
			dPath.push_back(curr);
		}

		// add the source
		dPath.push_back(source);

		// reverse order so path is traversed
		reverse(dPath.begin(), dPath.end());

		unsigned int total = 0;
		unsigned int count = 0;

		for (auto& p : dPath) {
			count++;

			grid[p.first][p.second].box.setOutlineColor(Color(242, 169, 0));
			grid[p.first][p.second].box.setOutlineThickness(-3.0f);
			grid[p.first][p.second].visitedD = true;
			gatorImg.setPosition(p.second * 32.0f, p.first * 32.0f);
			window.draw(grid[p.first][p.second].box);
			window.draw(gatorImg);
			window.display();
			total += grid[p.first][p.second].danger;

		}

		unsigned int countVisitedD = 0;
		unsigned int countVisitedA = 0;
		for (auto& row : grid) {
			for (auto& tile : row) {
				if (tile.visitedD) {
					countVisitedD++;
				}
				if (tile.visitedA) {
					countVisitedA++;
				}
			}
		}

		displayDResults(total, countVisitedA, countVisitedD, dPath);
	}

	// displays the result of dijkstras
	void displayDResults(unsigned int total, unsigned int countVisA, unsigned int countVisD, vector<pair<int, int>>& path) {
		cout << "\n--------------------------------" << endl;
		cout << "Dijkstras Algorithm Results" << endl;
		if (path.size() > 0) {
			cout << "Total danger cost (including source and destination): " << total << endl;
			cout << "Total steps taken (including source and destination): " << path.size() << endl;
			cout << "Total tiles visited (including source and destination): " << countVisD << endl;
			if (countVisA > 0) {
				cout << fixed << setprecision(3);
				cout << "a-star visited / dijkstras visited = " << (float)countVisA / (float)(countVisD) << endl;
			}
			cout << "\nPath Taken: " << endl;
			for (auto& p : path) {
				cout << "(" << p.first << ", " << p.second << "), ";
			}
			/*for (auto& p : dPath) {
				cout << p.first << ", " << p.second << "\n";
			}*/
			cout << endl;
			cout << endl;
		}
		else {
			cout << "No path found." << endl;
		}
		cout << "--------------------------------" << endl;
	}

	// manhattan distance heuristic that considers danger of tiles as well as distance
	int manhattanHeuristic(pair<int, int> a, pair<int, int> b) {
		return  abs(a.first - b.first) + abs(a.second - b.second) + max(grid[a.first][a.second].danger, grid[b.first][b.second].danger);
	}

	/*int diagonalHeuristic(pair<int, int> a, pair<int, int> b) {
		return max(abs(a.first - b.first), abs(a.second - b.second)) + max(grid[a.first][a.second].danger, grid[b.first][b.second].danger);
	}*/

	// returns neighbors as pairs
	vector<pair<int, int>> getNeighbors(pair<int, int> current) {
		vector<pair<int, int>> neighbors;
		int numRows = grid.size();
		int numCols = grid[0].size();

		int numNeighbors = fourNeighbors ? 4 : 8;

		vector<pair<int, int>> directions(numNeighbors);
		if (numNeighbors == 4) {
			directions = { {-1, 0}, { 1, 0 }, { 0, -1 }, { 0, 1 } };
		}
		else {
			directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
		}

		for (auto dir : directions) {
			int x = current.first + dir.first;
			int y = current.second + dir.second;
			if (x >= 0 && x < numRows && y >= 0 && y < numCols && grid[x][y].danger > 0) {
				neighbors.push_back({ x, y });
			}
		}
		return neighbors;
	}

	// a* algorithm
	void aStar() {

		auto numRows = grid.size();
		auto numCols = grid[0].size();

		// min heap priority queue to store danger
		priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
		
		// initialize all distances to max
		aDist = { numRows, vector<int>(numCols, INT_MAX) };

		// initalize parents
		aParent = { numRows, vector<pair<int, int>>(numCols, {-1, -1}) };

		pq.push({ 0, source });
		aDist[source.first][source.second] = 0;

		while (!pq.empty()) {
			auto current = pq.top().second;
			pq.pop();
			if (current == destination) {
				break;
			}
			if (grid[current.first][current.second].visitedA) {
				continue;
			}

			// color visited
			grid[current.first][current.second].visitedA = true;
			grid[current.first][current.second].box.setFillColor(Color(106, 42, 96)); 
			
			// find neighboring tiles
			auto neighbors = getNeighbors(current);
			for (auto& neighbor : neighbors) {

				// find the possible distance
				int possibleDistance = aDist[current.first][current.second] + grid[neighbor.first][neighbor.second].danger;
				if (possibleDistance < aDist[neighbor.first][neighbor.second]) {
					aDist[neighbor.first][neighbor.second] = possibleDistance;
					aParent[neighbor.first][neighbor.second] = current;

					// use modified manhattanHeuristic that accounts for danger of tiles
					int priority = possibleDistance + manhattanHeuristic(neighbor, destination);
					pq.push({ priority, neighbor });
				}
			}
		}

	}

	void calcAPath(RenderWindow& window) {
		//sleep(milliseconds(1));

		//vector<pair<int, int>> path;
		if (aParent[destination.first][destination.second] == make_pair(-1, -1)) {
			cout << "\n--------------------------------" << endl;
			cout << "A* Algorithm Results:" << endl;
			cout << "No valid path found." << endl;
			cout << "--------------------------------" << endl;
			return;
		}
		unsigned int total = 0;
		unsigned int count = 0;
		unsigned int countVisitedA = 0;
		unsigned int countVisitedD = 0;

		// add to the path to display
		for (auto curr = destination; curr != source; curr = aParent[curr.first][curr.second]) {
			aPath.push_back(curr);
		}

		// add source to path
		aPath.push_back(source);

		// reverse order so path is traversed
		reverse(aPath.begin(), aPath.end());

		for (auto& p : aPath) {
			count++;
			//grid[p.first][p.second].box.setOutlineColor(Color(212, 0, 219));
			grid[p.first][p.second].box.setOutlineColor(Color(211, 39, 55));
			grid[p.first][p.second].box.setOutlineThickness(-3.0f);
			grid[p.first][p.second].visitedA = true;
			gatorImg.setPosition(p.second * 32.0f, p.first * 32.0f);
			window.draw(grid[p.first][p.second].box);
			window.draw(gatorImg);
			window.display();
			total += grid[p.first][p.second].danger;

		}

		for (auto& row : grid) {
			for (auto& tile : row) {
				if (tile.visitedA) {
					countVisitedA++;
				}
				if (tile.visitedD) {
					countVisitedD++;
				}
			}
		}
		displayAResults(total, countVisitedA, countVisitedD, aPath);

	}
	// displays the result of dijkstras
	void displayAResults(unsigned int total, unsigned int countVisA, unsigned int countVisD, vector<pair<int, int>>& path) {
		cout << "\n--------------------------------" << endl;
		cout << "A-Star Algorithm Results" << endl;
		if (path.size() > 0) {
			cout << "Total danger cost (including source and destination): " << total << endl;
			cout << "Total steps taken (including source and destination): " << path.size() << endl;
			cout << "Total tiles visited (including source and destination): " << countVisA << endl;
			if (countVisD > 0) {
				cout << fixed << setprecision(3);
				cout << "a-star visited / dijkstras visited = " << (float)countVisA / (float)(countVisD) << endl;
			}
			cout << "\nPath Taken: " << endl;
			for (auto& p : path) {
				cout << "(" << p.first << ", " << p.second << "), ";
			}
			/*for (auto& p : dPath) {
				cout << p.first << ", " << p.second << "\n";
			}*/
			cout << endl;
			cout << endl;
		}
		else {
			cout << "No path found." << endl;
		}
		cout << "--------------------------------" << endl;
	}


};

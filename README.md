# Project3_cop3530

## About
This project is a path finding visualizer that shows the differences in approaches and efficiencies between Dijkstra's algorithm and A-Star. 

The premise is that Gabby the gator is hungry and wants to catch fish, but if she doesn't stay in the deep water, they will see her coming.

Gabby has devised two algorithms (Dijkstras and A-star) to help her find the optimal route to the fish. 
Here the optimal route is not necessarily the shortest distance.
Instead each tile on the grid has a depth associated with it, or a danger level (risk of being seen by the fish) and is colored to match. The optimal path is one that 
prioritizes taking the darkest tiles while still reaching the goal.

This program allows the user to randomly generate endless tilemaps with unique source and destination tiles as well as various danger levels. 
Users can add hazards and see how it changes the path finding by blocking routes or creating ellaborate paths.

## Dependencies
This program relies on the SFML 2.5 library. 32 or 64 bit should work fine, but you'll need to configure visual studio to use the static or dynamic libraries
and adjust any paths required for SFML. More information can be found at the SFML site. Once that is installed properly you should have no trouble running the code.
We compiled to C++14, but it shouldn't matter.

SFML Setup Info
- [SFML DOWNLOAD](https://www.sfml-dev.org/download.php)
- [SFML SETUP TUTORIAL](https://www.sfml-dev.org/tutorials/2.5/start-vc.php)

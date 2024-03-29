# Path Finding Visualizer

## About
This project is a path finding visualizer that shows the differences in approaches and efficiencies between Dijkstra's algorithm and A-Star. 

The premise is that Gabby the gator is hungry and wants to catch fish, but if she doesn't stay in the deep water, they will see her coming.

![gator](https://user-images.githubusercontent.com/109101463/235012862-6f90875f-9c09-4ea4-a07f-48394df213c0.png)	![fish](https://user-images.githubusercontent.com/109101463/235012881-75af20df-a5a7-442c-bca9-a41b3bd35714.png)


Gabby has devised two algorithms (Dijkstras and A-star) to help her find the optimal route to the fish. 
Here the optimal route is not necessarily the shortest distance.
Instead each tile on the grid has a depth associated with it, or a danger level (risk of being seen by the fish) and is colored to match. The optimal path is one that 
prioritizes taking the darkest tiles while still reaching the goal.

This program allows the user to randomly generate endless tilemaps with unique source and destination tiles as well as various danger levels. 
Users can add hazards and see how it changes the path finding by blocking routes or creating ellaborate paths.


## Dependencies
This program relies on the SFML 2.5 library. 32 or 64 bit should work fine, but you'll need to configure visual studio to use the static or dynamic libraries
and adjust any paths required for SFML. More information can be found at the SFML site or using the links below. Once that is installed properly you should have no trouble running the code.
We compiled to C++14, but it shouldn't matter.

## SFML Setup Info
- [SFML DOWNLOAD](https://www.sfml-dev.org/download.php)
- [SFML SETUP TUTORIAL](https://www.sfml-dev.org/tutorials/2.5/start-vc.php)
- [SFML FRAMERATE ISSUE](https://www.sfml-dev.org/tutorials/2.5/window-window.php)
- [SFML VISUAL STUDIO SETUP VIDEO](https://www.youtube.com/watch?v=_VgRjlvGpPk)

## Application Size
The application is hard coded to be 1344 x 868. The tile grid section it 1344 x 768

## Framerate
If you notice any weirdness with choppy movement on the pathfinding, it could be due to the graphics card or framerate issue. The following lines can be adjusted to fit your needs and will result in different smoothness depending on individual monitor, refresh rate, and gpu/cpu load.
	
```
// limit frame rate and enable vertical sync(tries to match framerate to refreshrate of monitor)
// window.setVerticalSyncEnabled(true);

// set the frame rate limit
// window.setFramerateLimit(60);
 ```

## Screencast
[Project Video Walkthrough](https://youtu.be/DAgrOO41-c0)

## Screenshots
![image](https://user-images.githubusercontent.com/109101463/234973252-1c28ae3f-53dc-4e8d-99b3-d94d97f16c2f.png)
![image](https://user-images.githubusercontent.com/109101463/234973742-5532f3c7-eb3d-41ad-a69e-1098b9de7641.png)




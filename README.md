#About the game:  
This is a simple game for a Computer Graphics project.  
You are a space traveler equiped with a powerful star gun and currently under alien attack.  
You start with 3 lives and your objective is to survive 5 minutes until your rescue arrives.  
Enemies will get faster and numerous as time goes on. Therefore, you should try going for power-ups whenever the opportunity arrives.  
Good luck!  

#Controls:  
W - Move up.  
A - Move left.  
S - Move down  
D - Move right.  
J - Zoom-in.  
K - Zoom-Out.  

Mouse left button - Shoots  
ESC - leaves the game  

#About the engine:  
SDL3 is the graphic motor that controls the window and the I/O inputs. However, this project does NOT use any functions that draws points, lines, polygons or textures on the screen.   
Everything is draw into a framebuffer via a custom setPixel function and SDL3 copies the entire framebuffer to the screen.   
The engine has all the basic rasterization algorithms: Bresenham's line algorithm, Flood fill, Circle rasterization, Ellipse rasterization, scanline with interpolation of colors between 2 points and   scanlineNearestNeighbor for sprites with uv mapping.  
It also has a lazy cache system for textures and sprites and respective drawSprite functions for static and animated sprites.  
All polygons can be translated, scaled and rotated.  
Sprites can be scaled and rotated.  
The camera position and clipping follows the player's positions and everything is drawn via interpolation of previous position and current position in between logic "steps".  
The game's logic updates 60 times per second and the remaing time between steps is used to interpolate entities positions.  
SDL3_Image is responsable for getting .png images and transforming them into a vector<uint32_t> to be used as textures and transformed into sprites. All sprites were personaly made.  
SDL3_Mixer is responsable for getting a .mp3 music file and playing on repeat. The song was Not made by me. Song name: Spider Dance - TobyFox   
link: https://www.youtube.com/watch?v=NH-GAwLAO30.  

#How to Build:  
1.Install cmake, SDL3, SDL3_Image and SDL3_Mixer (Make sure you add them to your PATH and that there is no version compatibility issues with SDL3 by updating all of them).  
2.Download/clone this repository and delete /build if there is one.  
3.Open a terminal in the main project folder and run "cmake -S . -B build" followed by "cmake --build build"  
4.You can now run MeteorFrenzy.exe!  

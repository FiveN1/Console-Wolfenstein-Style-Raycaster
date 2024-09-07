# Console-Wolfenstein-Style-Raycaster
A Simple Wolfenstein style raycaster game in a conosle window!



## Controls:
- movement: `WSAD`
- camera rotation: `Right-click`

## Map Editing:
to create your own map you will need theese files in your `assets/map` folder:
- **map.png** - contains map tile data
- **texture_atlas.png** - stores all of the textures of the map in one image
- **map_data.txt** - stores data associated with the map. like: tile colors, empty tile color, player tile color, player angle. _tile colors use the RGB color format in this file. (0-255)_

## How to compile:
1. download the repository.
2. run the `build.bat` file.
3. generate the project files you want by **calling any of the premake commands** listed in the `help` message.
or if you want to compile outside your project editor you can just call the `compile` or `c` command.
4. **compile your project in the project editor** you generated your files for!
> [!NOTE]
> by calling `compile` command you will run the **MSBuild** compiler with the `vs2022` solution by default. **this might not work if you dont have Visual Studio 2022 installed!**

- if you want to run the executable in the dev enviroment outside your project editor you can just type `run` or `r` for short.
- type `help` for list of commands in the `build.bat` file.

## Troubleshooting:
- if you don't have **Visual Studio 2022** installed the `compile` command won't work by default.
- you can fix this by changing the premake command call under `:Compile` in the `build.bat` file from `vs2022` to somthing else. _yeah it kinda sucks_
- if this doesn't fix work you will heve to change `"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"` to some other **Dev Enviroment Directory**.

## Devolepement process
I decided to give myself a challenge... to write my own Wolfenstein clone by myself. So figuring out how would you go about rendering a 2D map as "3D".
So I read some info about how Wolfenstein renderer works and i tried to implement it in C++.

### Beautiful code
The biggest challange for me was writing the **grid traversal algorithm**. More specifically finding the nearest tile edge intersection when you cast a ray from within the tile.
This doesn't seem as a too big of a problem, you can solve it by having a lot of if-else statements. Like i did last year ago in my older project:
```cpp
if (sin(rayAngl) > 0) {
    tilStpX = 1;
    Xstep = -1 / tan(rayAngl);
    xr = ceil(Player.x) - Player.x;
    rayX = -((ceil(Player.x) - Player.x) / tan(rayAngl));
}
else {
    tilStpX = -1;
    Xstep = 1 / tan(rayAngl);
    xr = -(Player.x - floor(Player.x));
    rayX = (Player.x - floor(Player.x)) / tan(rayAngl);
}
// (Y)
if (cos(rayAngl) > 0) {
    tilStpY = -1;
    Ystep = tan(rayAngl);
    yr = -(Player.y - floor(Player.y));
    rayY = (Player.y - floor(Player.y)) * tan(rayAngl);
}
else {
    tilStpY = 1;
    Ystep = -tan(rayAngl);
    yr = ceil(Player.y) - Player.y;
    rayY = -((ceil(Player.y) - Player.y) * tan(rayAngl));
}
```
But i wanted to make it clean and readable, without all of the nesting.
So I came up with this:
```cpp
float y = (float)(cos(ray_angle) < 0.0f); // coordinates of the Y value for the square. can only be 0.0 or 1.0 based on the cos of angle. imagine it as a value that determines on wich side of the circle you are.
float yn = y * 2.0f - 1.0f; // scale y so that it will be (-1) when false and (1) when true (used as variable so it is more readable! although it will be faster to calculate it probably)
vec2 ray_x = vec2(ray_position.x - tan(ray_angle) * yn * abs(ray_position.y - y), y);
// first ray intersection on the Y axis
float x = (float)(sin(ray_angle) > 0.0f); // coordinates of the X value for the square. can only be 0.0 or 1.0 based on sin of angle. imagine it as a value that determines on wich side of the circle you are.
float xn = x * 2.0f - 1.0f; // scale x so that it will be (-1) when false and (1) when true (used as variable so it is more readable! although it will be faster to calculate it probably)
vec2 ray_y = vec2(x , ray_position.y - (1.0f / tan(ray_angle)) * xn * abs(ray_position.x - x));
```
The `ray_x` vector is the nearest intersection on X axis and the `ray_y` on the Y axis. This code doesn't use any nesting and looks super clean and readable.

I don't know why I'm so obsessed by this, but the biggest lesson I learned from this was when I came back to my old Wolfenstein clone game from a year ago for inspiration, i just couldn't read it, it was a mess.
This is why you shoud write readable and clean code. especially if you're working on a large project where you will allways iterate over your code.

So my idea of beautiful code is:
1. It shoud be readable.
2. It shoud feel well built, robust.
3. Math will make it more beautiful.

This is why good mathematicians are good coders. They can build beautiful systems.

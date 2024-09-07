# Console-Wolfenstein-Style-Raycaster
A Simple Wolfenstein style raycaster game in a conosle window!

# Controls:
- movement: 'WSAD'
- camera rotation: 'Right-click'

# Map Editing:
to create your own map you will need theese files in your 'assets/map' folder:
- **map.png** - contains map tile data
- **texture_atlas.png** - stores all of the textures of the map in one image
- **map_data.txt** - stores data associated with the map. like: tile colors, empty tile color, player tile color, player angle. _tile colors use the RGB color format in this file_

# How to compile:
- (1) download the repository.
- (2) run the 'build.bat' file.
- (3) generate the project files you want by **calling any of the premake commands** listed in the 'help' message.
    or if you want to compile outside your project editor **you can just call 'compile' or 'c'**.
    **NOTE:** by calling 'compile' command you will run the MSBuild compiler with the vs2022 solution by default. _this might not work if you dont have Visual Studio 2022 installed!_
- (4) **compile your project in the project editor** you generated your files for!

- if you want to run the executable in the dev enviroment outside your project editor you can just type 'run' or 'r' for short.
- **type 'help' for list of commands in the 'build.bat' file.**

# Troubleshooting:
- if you don't have Visual Studio 2022 installed the 'compile' command won't work by default.
- you can fix this by changing the premake command call under ':Compile' in the 'build.bat' file from 'vs2022' to somthing else. (yeah it kinda sucks)
- if this doesn't fix work you will heve to change "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" to some other Dev Enviroment Directory.

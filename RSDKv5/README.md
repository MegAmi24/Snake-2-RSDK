# Snake 2 RSDKv5
The RSDKv5 port of Snake 2. Created as an exercise for creating games with GameAPI.

Can be loaded in the decomp as a mod with the included `mod.ini` file or via Data Folder Mode.

# How to build
This building guide assumes you are already familiar with [the building process for the RSDKv5(U) Decompilation](https://github.com/RSDKModding/RSDKv5-Decompilation#how-to-build).

## Get the source code
Clone the repo **recursively**, using:
`git clone https://github.com/MegAmi24/Snake-2-RSDK --recursive`

If you've already cloned the repo, run this command inside of the repository:
```git submodule update --init --recursive```

## Building

### Building with CMake
No external dependencies are required; simply type the following commands in the repository's root directory:
```
cmake -B build
cmake --build build --config release
```

The resulting build will be located somewhere in `build/` depending on your system.

The following CMake arguments are available when compiling:
- Use these on the `cmake -B build` step like so: `cmake -B build -DRETRO_REVISION=2`

#### CMake flags
- `RETRO_REVISION`: What RSDKv5 revision to compile for. Takes an integer, defaults to `3` (RSDKv5U).
- `RETRO_MOD_LOADER_VER`: Manually sets the mod loader version. Takes an integer, defaults to the current latest version.
- `GAME_INCLUDE_EDITOR`: Whether or not to include functions for use in certain RSDKv5 scene editors. Takes a boolean, defaults to `on`.

### Building in Visual Studio
Simply open the solution and build the project. Make sure that the architecture, engine revision, and mod loader version match with the target engine binary.

### Building for Android
After [setting up the RSDKv5(U) decompilation for building](https://github.com/RSDKModding/RSDKv5-Decompilation#how-to-build), you will need to add a symbolic link for the directory to the Snake 2 repository so that the game will be built as part of the APK.
To add the symlink, write the appropriate command in `[RSDKv5-decompilation-root]/android/app/jni`:
  * Windows: `mklink /d Game "[path-to-snake2]"`
  * Linux: `ln -s "[path-to-snake2]" Game`
  * If you'd like to build this as a mod rather than the main game, replace `Game` with `Snake2`.

After adding the symlink, build the RSDKv5(U) decompilation in Android Studio.

# Credits
* Original Game by DougDoug
* Using code generated with ChatGPT
* Retro Engine v5 port by MegAmi
* Some assets taken from Sonic CD (2011) by SEGA and Christian Whitehead
* Some assets taken from Sonic Mania by SEGA, Christian Whitehead, Headcannon, and PagodaWest Games
* Some code taken from the Sonic Mania decompilation and the ManiaTouchControls mod by Rubberduckycooly and st×tic

All rights go to their respective owners. This is very legal.

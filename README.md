# Cobalt Core Windowed
Allows a resizable window in Cobalt Core.

## Installation
1. Download [XInput1_4.dll](https://github.com/ComplexRobot/CobaltCoreWindowed/releases/download/release/XInput1_4.dll) and place it into your Cobalt Core installation directory.
   - `C:\Program Files (x86)\Steam\steamapps\common\Cobalt Core` is the default.
   - It must be placed in the `Cobalt Core` main directory. It won't be recognized in a subfolder.
3. Launch Cobalt Core.
4. Set the in-game setting to fullscreen (if it's not already).

## Usage
Press the tilde key (\`~) to toggle between crop-to-fit and letterboxing display modes.
### Additional Info.
A log file `Windowed-Hook.log` and config file `Windowed-Hook.ini` will be generated on first run.

All window positional information is saved and restored between relaunches of the game app.

## How To Uninstall
Remove or rename `XInput1_4.dll`.

## Information
The code uses a [minimalist hook-injection pattern](https://github.com/TsudaKageyu/minhook) to add functionality to the game.

 It adds a workaround to display the window correctly and be resizable without modifying the base game.

There's a rendering cutoff if the window is larger than the fullscreen resolution.

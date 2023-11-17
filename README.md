# Cobalt Core Windowed
Allows a resizable window in Cobalt Core.

## Installation
1. Download [XInput1_4.dll](https://github.com/ComplexRobot/CobaltCoreWindowed/releases/download/release/XInput1_4.dll) and place it into your Cobalt Core installation directory root.
   - `C:\Program Files (x86)\Steam\steamapps\common\Cobalt Core\` is the default.
3. Launch Cobalt Core.
4. Set the in-game setting to fullscreen (if it's not already).

## Usage
Press the tilde key (\`~) to toggle between crop-to-fit and letterboxing display modes.
### Additional Info.
A log file `Windowed-Hook.log` and config file `Windowed-Hook.ini` will be generated on first run.

All window positional information is saved and restored between relaunches of the game app.

## Uninstall
Remove or rename `XInput1_4.dll`.

## Information
The code uses a [minimalist hook-injection pattern](https://github.com/TsudaKageyu/minhook) to add functionality to the game.

 It adds a workaround to display the window and be resizable without modifying the base game. It uses a minimalistic and modular design that should continue to work, but it is still possible it may break in future releases of the game.

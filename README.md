# Cobalt Core Windowed
Allows a resizable window in Cobalt Core.

## Installation
1. Download [XInput1_4.dll](https://github.com/ComplexRobot/CobaltCoreWindowed/releases/download/release/XInput1_4.dll) and place it into your Cobalt Core installation directory.
2. Launch Cobalt Core.
3. Set the in-game setting to fullscreen (it is by default).

## Usage
Press the tilde key (\`~) to toggle between crop-to-fit and letterboxing display modes.

A log file `Windowed-Hook.log` and config file `Windowed-Hook.ini` will be generated on first run.

All window positional information is saved and restored between relaunches of the game app.

## Information
Uses a [minimalist injection-hook pattern](https://github.com/TsudaKageyu/minhook) to add functionality to the base code of the game.

This adds a workaround to display the window and be resizable. It uses a minimalistic and modular design that should continue to work, but it is still possible it may break in future releases of the game.

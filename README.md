# Chip-8-Emulator-and-Dissasembler
Contains a Chip-8 dissasembler for .ch8 files and and a basic emulator. Both written in C++.

# Purpose
The purposes of this project include:
- To learn emulator/system development in an interesting way.
- Get comfortable with basic C and C++ functions and libraries.
- Further develop and understand bitwise masking through memory storage.

# Goals
- Get a basic emulator running for almost any .ch8 ROM file.
- Create a dissasembler which runs any .ch8 file and decodes the opcodes.

# Requirements
- Windows
- MSYS2
- GCC C++ Compiler with SDL2 Libraries
- Visual Studio Code

# Setup
1. Download the source code
2. Open the folder in VS Code or IDE of choice.
3. Create a folder named "roms" and store the rom files you will provide in there.
4. If you have MSYS2 and other dependencies installed, open the VS Code Terminal.

# Build
Make sure you are in the chip8 folder directory. If not, then type `cd chip8` in the terminal and press enter

Compile the emulator with: 

`C:\msys64\ucrt64\bin\g++.exe emulator.cpp cpu.cpp -o chip8.exe -IC:\msys64\ucrt64\include\SDL2 -LC:\msys64\ucrt64\lib -DSDL_MAIN_HANDLED -lSDL2`

Then, run ` .\chip8.exe "roms/IBM Logo.ch8" ` in the terminal and replace the .ch8 name with the rom file name provided. I used the IBM Logo rom for example.

The keypad is a 4 by 4 key pad from 1-4 all the way down to z-v.

# Dissasembler
Similar steps as the emulator.

Compile with: 

`C:\msys64\ucrt64\bin\g++.exe disassembler.cpp -o disassembler.exe`

Then run with: `.\disassembler.exe "roms/IBM Logo.ch8"` with your provided file name again.

# Notes
You need to provide your own ROM to run this emulator and dissasembler


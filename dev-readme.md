# Tic-Tac-Pong
## Project Info

> Developed by Mitchell Philipp

This project is compiled using [ARM GCC Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm). To install it on Ubuntu use:
```
sudo apt install gcc-arm-none-eabi
```
The project's makefile contains a target `med` that builds the project and launches an emulator to run it. It uses the [Mednafen GBA emulator](https://mednafen.github.io/), and `make med` won't work without having Mednafen installed.
```
sudo apt install mednafen
```
Images used in the game were converted to C source code using [nin10kit](https://github.com/TricksterGuy/nin10kit)
```
sudo apt install nin10kit
```

###### Contact the developer
> Mitchell Philipp (he/they): [mgpalpha1@gmail.com](mailto:mgpalpha1@gmail.com)
> 
> Github: [MGPAlpha](https://github.com/MGPAlpha)
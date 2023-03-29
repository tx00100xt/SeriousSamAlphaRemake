## Serious Sam Alpha Remake
![Build status](https://github.com/tx00100xt/SeriousSamAlphaRemake/actions/workflows/cibuild.yml/badge.svg)
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/tx00100xt/SeriousSamAlphaRemake)

What is Serious Sam Alpha Remake?  
This is a modification for Serious Sam Classic The First Encounter. 
This mod required https://github.com/tx00100xt/SeriousSamClassic or https://github.com/tx00100xt/SeriousSamClassic-VK to run.  
Serious Sam Alpha Remake was created by fans of the game Serious Sam TFE and is distributed for free.    

Description: (From the author of mod)  
This is remake based on all code leftovers and original assets from alpha build. They are fixed and ported to game.    

Author:  
Zdzichu is author this mod for windows.

![Lava Planet](https://raw.githubusercontent.com/tx00100xt/SeriousSamAlphaRemake/main/Images/alpharemake_1.png)

![Ice Planet](https://raw.githubusercontent.com/tx00100xt/SeriousSamAlphaRemake/main/Images/alpharemake_2.png)

![Ice Planet End](https://raw.githubusercontent.com/tx00100xt/SeriousSamAlphaRemake/main/Images/alpharemake_3.png)

Download [SeriousSamAlphaRemake_v1.5.7z] archive and unpack to  SeriousSamClassic/SamTFE/ directory.
To start the modification, use the game menu - item Modification.

Building Serious Sam Alpha Remake.
----------------------------------

### Linux

Type this in your terminal:

```
git clone https://github.com/tx00100xt/SeriousSamAlphaRemake.git SeriousSamAlphaRemake
cd SeriousSamAlphaRemake/Sources
./build-linux64.sh -DTFE=TRUE	# use build-linux32.sh for 32-bits
```
After that , libraries will be collected in the x32 or x64 directory . Copy them to SeriousSamClassic/SamTFE/Mods/SSA/Bin folder.

### Gentoo

To build a game for gentoo, use a https://github.com/tx00100xt/serioussam-overlay containing ready-made ebuilds for building the game and add-ons.

### Arch Linux

To build a game under Arch Linux you can use the package from AUR: https://aur.archlinux.org/packages/serioussam

### Raspberry Pi

The build for raspberry pi is similar to the build for Linux, you just need to add an additional build key.

```
cd SeriousSamAlphaRemake/Sources
./build-linux64.sh -DTFE=TRUE -DRPI4=TRUE	# use build-linux32.sh for 32-bits
```
### FreeBSD

Install bash. 
Type this in your terminal:

```
git clone https://github.com/tx00100xt/SeriousSamAlphaRemake.git SeriousSamAlphaRemake
cd SeriousSamAlphaRemake/Sources
bash build-linux64.sh -DTFE=TRUE	# use build-linux32.sh for 32-bits
```
After that , libraries will be collected in the x32 or x64 directory . Copy them to SeriousSamClassic/SamTFE/Mods/SSA/Bin folder.

Windows
-------
* This project can be compiled starting from Windows 7 and higher.

1. Download and Install [Visual Studio 2015 Community Edition] or higher.
2. Download and Install [Windows 10 SDK 10.0.14393.795] or other.
3. Open the solution in the Sources folder, select Release x64 or Release Win32 and compile it.

Supported Architectures
----------------------
* `x86`
* `aarch64`
* `e2k` (elbrus)

Supported OS
-----------
* `Linux`
* `FreeBSD`
* `Windows`
* `Raspberry PI OS`

### Build status
|CI|Platform|Compiler|Configurations|Platforms|Status|
|---|---|---|---|---|---|
|GitHub Actions|Windows, Ubuntu|MSVC, GCC|Release|x64|![GitHub Actions Build Status](https://github.com/tx00100xt/SeriousSamAlphaRemake/actions/workflows/cibuild.yml/badge.svg)

License
-------

* Serious Engine is licensed under the GNU GPL v2 (see LICENSE file).

Some of the code included with the engine sources is not licensed under the GNU GPL v2:

* zlib (located in `Sources/Engine/zlib`) by Jean-loup Gailly and Mark Adler


[SeriousSamAlphaRemake_v1.5.7z]: https://drive.google.com/file/d/1JZouza6PCpqGbucFYLaMh1oDXmGbb7_6/view?usp=share_link "Serious Sam Classic SSA Mod"
[Visual Studio 2015 Community Edition]: https://go.microsoft.com/fwlink/?LinkId=615448&clcid=0x409 "Visual Studio 2015 Community Edition"
[Windows 10 SDK 10.0.14393.795]: https://go.microsoft.com/fwlink/p/?LinkId=838916 "Windows 10 SDK 10.0.14393.795"

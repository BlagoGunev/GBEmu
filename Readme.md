# Gameboy Emulator
Gameboy Emulator written in C, based on [Low Level Devel's tutorials](https://youtube.com/playlist?list=PLVxiWMqQvhg_yk4qy2cSC3457wZJga_e5) on YouTube. Tested and working on Fedora Linux, but should also be compatible with most other distros.

## Progress
- [X] CPU Instruction set
- [X] CPU Interrupts
- [X] Timers
- [X] DMA
- [ ] LCD
- [ ] PPU
- [ ] Gamepad

![Rendered output](/assets/progress_tiles.png)

## Building instructions
You will need the following build packages:
- `gcc`
- `cmake`
- `check`
- `sdl2-compat`
- `SDL2_ttf`

To install all the necessary packages use the following command on Fedora Linux:
```bash
sudo dnf install -y gcc cmake sdl2-compat-devel SDL2_ttf-devel check-devel
```

To build the project, go into the `code` directory. From there
```bash
mkdir build
cd build
cmake ..
make
```

## Using the emulator
To run the emulator from the `build` directory, use the following command:
```bash
gbemu/gbemu <path-to-rom-file>
```

In the `roms` directory are provided ROMs used for testing.

## Folder structure
```
.
├── code
│   ├── cmake
│   │   ├── config.h.in
│   │   ├── COPYING-CMAKE-SCRIPTS.txt
│   │   ├── FindCheck.cmake
│   │   └── sdl2
│   │       ├── Copyright.txt
│   │       ├── FindSDL2.cmake
│   │       ├── FindSDL2_gfx.cmake
│   │       ├── FindSDL2_image.cmake
│   │       ├── FindSDL2_mixer.cmake
│   │       ├── FindSDL2_net.cmake
│   │       ├── FindSDL2_ttf.cmake
│   │       └── README.md
│   ├── CMakeLists.txt
│   ├── gbemu
│   │   ├── CMakeLists.txt
│   │   └── main.c
│   ├── include
│   │   ├── bus.h
│   │   ├── cart.h
│   │   ├── common.h
│   │   ├── cpu.h
│   │   ├── dbg.h
│   │   ├── dma.h
│   │   ├── emu.h
│   │   ├── instructions.h
│   │   ├── interrupts.h
│   │   ├── io.h
│   │   ├── ppu.h
│   │   ├── ram.h
│   │   ├── stack.h
│   │   ├── timer.h
│   │   └── ui.h
│   ├── lib
│   │   ├── bus.c
│   │   ├── cart.c
│   │   ├── CMakeLists.txt
│   │   ├── cpu.c
│   │   ├── cpu_fetch.c
│   │   ├── cpu_proc.c
│   │   ├── cpu_util.c
│   │   ├── dbg.c
│   │   ├── dma.c
│   │   ├── emu.c
│   │   ├── instructions.c
│   │   ├── interrupts.c
│   │   ├── io.c
│   │   ├── ppu.c
│   │   ├── ram.c
│   │   ├── stack.c
│   │   ├── timer.c
│   │   └── ui.c
│   ├── NotoSansMono-Medium.ttf
│   └── tests
│       ├── check_gbe.c
│       └── CMakeLists.txt
├── docs
│   ├── gbctr.pdf
│   └── The Cycle-Accurate Game Boy Docs.pdf
├── .gitignore
├── Readme.md
└── roms
    ├── 01-special.gb
    ├── 02-interrupts.gb
    ├── 03-op sp,hl.gb
    ├── 04-op r,imm.gb
    ├── 05-op rp.gb
    ├── 06-ld r,r.gb
    ├── 07-jr,jp,call,ret,rst.gb
    ├── 08-misc instrs.gb
    ├── 09-op r,r.gb
    ├── 10-bit ops.gb
    ├── 11-op a,(hl).gb
    ├── cpu_instrs.gb
    ├── dmg-acid2.gb
    └── mem_timing.gb
```
10 directories, 68 files

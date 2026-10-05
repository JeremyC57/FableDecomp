# Fable: The Lost Chapters — original Xbox, recompiled

The Xbox `default.xbe` statically recompiled to C (`../lifter`), with a host that replaces
the console: the kernel, the NV2A GPU (rendered through Vulkan), DirectSound, XInput and
the hard disk. It runs on Linux (x86-64) and Android (arm64). No game data is included:
you need your own copy of the Xbox disc.

## What you need
- The disc image, extracted to a folder (for example with `extract-xiso -x Fable.iso`).
  The folder contains `default.xbe` and `Data/`.
- Linux: clang, cmake, ninja, SDL2 (dev package), a Vulkan driver.
- Android: a 64-bit ARM phone with Vulkan 1.1. A custom Vulkan driver (for example Mesa
  Turnip for Adreno GPUs, in the adrenotools `.zip` format) can be selected in the launcher.

## Building (Linux)
```sh
cmake -S rebuild/recomp -B build/lifter -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build/lifter
rebuild/recomp/xbox/lift.sh build/lifter/fable_recomp <disc>/default.xbe build/gen_xbox
cmake -S rebuild/recomp/xbox -B build/xbox -G Ninja -DCMAKE_BUILD_TYPE=Release -DFABLE_GEN_DIR=build/gen_xbox \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build build/xbox
build/xbox/FableXbox --game <disc> --hdd <saves folder>/hdd
```

## Building (Android)
```sh
# NDK r27c, SDK (platforms;android-34, build-tools;35.0.0), JDK 17+, cmake, ninja, git
rebuild/recomp/xbox/android/build_android.sh <work dir> build/gen_xbox   # -> <work dir>/FableXbox.apk
```
Install the APK, extract the disc to the phone's storage, and choose that folder in the
launcher. Saves, settings and `FableXbox.log` go to `Android/data/org.fablexbox/files/`.

## Settings
The Android launcher writes them; on Linux, put them in `<hdd folder>/../fable_xbox.ini`
(or pass `--config <file>`):

| key | values | |
|---|---|---|
| `resolution_scale` | 1–4 | internal render resolution, × 640x480 |
| `aspect` | `4:3`, `16:9` | 16:9 widens the 3D view (the game only renders 4:3; the HUD is stretched) |
| `anisotropy` | 1, 2, 4, 8, 16 | anisotropic texture filtering |
| `fps` | 30, 60 | 60 is experimental: the game is a fixed-step 30 Hz game (see HANDOFF.md) |
| `pc_textures` | folder | PC install folder for higher-quality textures (not implemented yet) |
| `vulkan_driver` | path | a Vulkan driver `.so` to load instead of the system loader (Linux) |
| `volume` | 0–100 | master volume |
| `fullscreen`, `vsync` | 0, 1 | |

## Controls
An Xbox-style controller works as is (A/B/X/Y, LB = White, RB = Black, triggers, sticks,
Back, Start). Keyboard: WASD move, arrows camera, Space A, Left Shift B, E X, Q Y, R Black,
F White, Z/C triggers, Enter Start, Esc/Tab Back, I/J/K/L d-pad. On Android there are
on-screen controls (drag on the empty screen to turn the camera).

## Status
Boots, plays the intro movie and the opening cutscene, and reaches gameplay. See
`HANDOFF.md` for what works, what is missing and how to debug.

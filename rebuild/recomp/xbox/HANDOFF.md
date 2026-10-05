# Xbox Fable TLC recompilation — handoff

Goal: the original Xbox `default.xbe` (Fable: The Lost Chapters, USA, title 4D5300D1)
statically recompiled and running natively on Linux and Android, rendering through Vulkan
(custom driver option), with launcher settings (resolution scale, 16:9/4:3, 30/60 fps,
PC HD textures, anisotropic filtering).

## Inputs (never committed)
- Disc extracted to `work/xbox/iso/` (gitignored). It was fetched with HTTP range reads
  from the user's Google Drive xiso.
- Symbols for statically linked XDK functions: build Cxbx-Reloaded's XbSymbolDatabase CLI
  and run it on `default.xbe`. Kernel signatures come from Cxbx (reference only, GPL: no
  code copied). xemu's nv2a sources are used as a hardware reference only.

## Build and run (Linux)
```sh
# lifter
cmake -S rebuild/recomp -B lb -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build lb
rebuild/recomp/xbox/lift.sh lb/fable_recomp work/xbox/iso/default.xbe gen_xbox   # mmio, safepoints, HLE hooks
cmake -S rebuild/recomp/xbox -B bx -G Ninja -DCMAKE_BUILD_TYPE=Release -DFABLE_GEN_DIR=gen_xbox \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build bx        # full rebuild ~25 min on 4 cores (recomp_0102.c is the long pole)
FABLE_HEADLESS=1 FABLE_AUTOPRESS=1 XBOX_LOG=1 bx/FableXbox --game work/xbox/iso --hdd run/hdd
```
- `XBOX_LOG`: 0 errors, 1 info, 2 kernel calls and NV2A register writes, 3 more, 4 every Kelvin method.
- `FABLE_DUMP_FRAMES=N` (software renderer) writes every Nth scanout frame to a PNG.
- Settings file: `<hdd>/../fable_xbox.ini` (see `settings.hpp`).

## State (2026-10-05, evening)
**The game boots, plays the intro movie and opening cutscene, and reaches gameplay in
Oakvale**, rendered through Vulkan (tested on lavapipe under Xvfb, ~7 fps in software).
- Lifter: XBE input, ring-0 instructions, `--mmio`, `--safepoints`, `--hook`/`--wrap`,
  `--trace`, `--bucket 14` (files by address range: a lifter change recompiles a few files,
  seconds instead of ~30 min). Lazy flags carry into fall-through targets; unaligned and
  vtable-neighbour code pointers become entries (XBE has no relocations).
- Host: kernel HLE, threads under a fair GIL, coroutines on host stacks (`coro.cpp`, the
  game's stack-copying script threads), 733 MHz RDTSC, video field toggle (port 0x80C0, the
  movie clock needs it), zero-filling operator new (0x1FBF0, see hle.cpp).
- Audio: DSOUND HLE (`audio.cpp`), SDL mixer, Xbox ADPCM, streams (movies, music).
- Input: XInput HLE; `FABLE_AUTOPRESS=1` taps A (and Start for 50 s) for unattended runs.
- NV2A + Vulkan: pusher (wrap fix), PATT_COLOR0 fence, vertex programs, register combiners,
  fog unit, surfaces alias by address, frame readback (`FABLE_DUMP_FRAMES=N`).
- Debugging: `XBOX_LOG=0..4`, `FABLE_DRAWLOG=<flip>`, `FABLE_DUMP_SHADERS=1`,
  `FABLE_WATCH=<guest addr>` (logs writers of a word), `lift.sh ... --trace ADDR`.
- Run unattended: `Xvfb :98 &` then `DISPLAY=:98 FABLE_AUTOPRESS=1 FABLE_NO_AUDIO=1
  FABLE_DUMP_FRAMES=1500 timeout -s KILL 300 bx/FableXbox --game ... --hdd run/hdd`, with
  `run/fable_xbox.ini` holding `vulkan_driver = /usr/lib/x86_64-linux-gnu/libvulkan_lvp.so`.

## Next
1. Renderer: PROJECT3D on a 2D depth texture is a shadow-map compare (now a dummy 3D
   lookup); dot-product texture modes; fixed-function T&L; check hero/NPC rendering in play.
2. Launcher settings: 60 fps (presentation interval), widescreen, PC textures, anisotropy
   (sampler path exists), resolution scale (surfaces support `scale_`).
3. Android: build script for this target (SDL2, adrenotools -> `vk::Settings::driverHandle`),
   reuse `rebuild/recomp/posix/android` (launcher activity, touch controls).
4. Movies: frame presentation through the renderer works; check A/V sync.

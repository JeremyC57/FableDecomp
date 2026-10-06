# Xbox Fable TLC recompilation — handoff

Goal: the original Xbox `default.xbe` (Fable: The Lost Chapters, USA, title 4D5300D1)
statically recompiled and running natively on Linux and Android, rendering through Vulkan
(custom driver option), with launcher settings (resolution scale, 16:9/4:3, 30/60 fps,
PC HD textures, anisotropic filtering).

## Inputs (never committed)
- The game takes the disc image itself: `--game Fable.iso` extracts it once (`disc.cpp`,
  XDVDFS; Android: `DiscExtractor.java` in the launcher) and runs from the folder.
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
  fog unit, surfaces alias by address, shadow-map compares, frame readback
  (`FABLE_DUMP_FRAMES=N`). Occlusion queries are stubbed: GET_REPORT returns 0x10000 pixels
  (`gpu.hpp` zpassCount). With 0, Fable culls every house and character.
- Debugging: `XBOX_LOG=0..4`, `FABLE_DRAWLOG=<flip>`, `FABLE_DUMP_SHADERS=1`,
  `FABLE_WATCH=<guest addr>` (logs writers of a word), `lift.sh ... --trace ADDR`.
- Run unattended: `Xvfb :98 &` then `DISPLAY=:98 FABLE_AUTOPRESS=1 FABLE_NO_AUDIO=1
  FABLE_DUMP_FRAMES=1500 timeout -s KILL 300 bx/FableXbox --game ... --hdd run/hdd`, with
  `run/fable_xbox.ini` holding `vulkan_driver = /usr/lib/x86_64-linux-gnu/libvulkan_lvp.so`.

## Next
1. Renderer: real occlusion queries (count passed fragments per CLEAR/GET_REPORT pair with
   Vulkan queries); dot-product texture modes; fixed-function T&L.
2. Launcher settings. Done: resolution scale, anisotropy, widescreen (anamorphic 3D
   squeeze + 16:9 present; `vk_pipeline.cpp`), custom Vulkan driver, volume. To do:
   - 60 fps: the game already presents with ONE_OR_IMMEDIATE (0x80000001; the wrap of
     Direct3D_CreateDevice forces ONE), yet runs a measured 30.0 flips/s: Fable is a 30 Hz
     game (a fixed 1/30 s step at 0x910EC4/0x9190DC, ~10 users, plus its own limiter).
     60 fps means halving that step consistently and finding the limiter.
   - PC textures: replace textures at upload by name (needs the game's texture/bank
     name at load time).
3. Android: `android/build_android.sh` builds the APK (launcher, touch controls, custom
   drivers). Not yet tested on a device.
4. Movies: frame presentation through the renderer works; check A/V sync.

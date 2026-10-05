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

## State (2026-10-05)
- The lifter reads XBEs. It handles the ring-0 instructions, memmove's jump tables,
  `--mmio` (checked device-register accessors for the D3D section), `--safepoints`
  (preemption checks on loop back edges) and `--hook` (HLE).
- Host runtime (`core/memory/kernel/files/xc/main`): XAPI and CRT start-up, the game's
  engine init and def loading, Direct3D_CreateDevice, the display mode set, and the GPU
  context switch and callbacks all work.
- Fable's own demand paging is disabled through a `T:\xboot.ini` override.
- NV2A (`nv2a.cpp`) covers the registers, the DMA pusher, RAMHT, interrupts and vblank,
  semaphores, flips, NOP callbacks, the clear-value mirrors and the 2D blit.
- Input HLE (`hle.cpp`, `input.cpp`) is written. The rebuild with the hooks was running
  when this note was written; next is to see how far the game gets.
- Vulkan: `vk.cpp` (loader, custom driver, swapchain, present) and `video.cpp` (window,
  scanout upload) are in the build.
- The renderer `vk_renderer.cpp` and `vk_pipeline.cpp` are written but **not yet in
  CMake**. To add them: append the files plus `nv2a_vsh.cpp`, `nv2a_psh.cpp` and
  `nv2a_texture.cpp` to HOST_SOURCES, add glslang (FetchContent tag 15.4.0, ENABLE_OPT
  OFF, link `glslang SPIRV glslang-default-resource-limits`), set
  `gpu::g_renderer = new VkRenderer` after `vk::init` in video.cpp, present through
  `scanoutImage()`, and fix the compile errors.

## Next
1. Get the renderer compiling, then see the first frames (Xvfb + lavapipe:
   `vulkan_driver = /usr/lib/x86_64-linux-gnu/libvulkan_lvp.so`).
2. Fixed-function T&L (lighting, texgen) if Fable uses it; check which execution mode
   its draws use.
3. Audio: DSOUND HLE (151 symbols found). Movies: XMV.
4. Launcher settings: 60 fps (presentation interval), widescreen, PC textures, anisotropy.
5. Android: reuse `rebuild/recomp/posix/android` (SDL, adrenotools for custom drivers).

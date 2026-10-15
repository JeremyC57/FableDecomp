# Fable TLC (recompiled) on the Anbernic RG34XX SP

Target: Allwinner H700 (4x Cortex-A53 @ 1.5 GHz), Mali-G31 MP2, 2 GB LPDDR4, 720x480 (3:2)
screen, the stock (fan-modified) Linux OS that launches `.sh` files.

## What the package is
An aarch64 Linux build of the recompiled game (`rebuild/recomp/posix`), cross-compiled with clang
(`linux-a64.cmake`, target aarch64-linux-gnu) against Ubuntu's arm64 packages, bundled with everything it loads:
glibc and its dynamic loader, libstdc++, SDL2 (KMSDRM video, ALSA audio, evdev pads), FFmpeg
(movies), plus DXVK Native and the Vulkan loader for devices that do have Vulkan. `Fable.sh`
starts the game through the bundled loader (`lib/ld-linux-aarch64.so.1 --library-path lib`),
so the device's own glibc version does not matter.

Rendering is Direct3D 9 on **OpenGL ES 3.0** (`rebuild/recomp/posix/gles/`, selected by
`FABLE_RENDERER=gles` in `Fable.sh`): the Mali-G31 has no Vulkan driver on the stock OS. The
GL entry points are resolved at run time through SDL from the OS's own EGL/GLES libraries
(the Mali blob); the package does not bring a GPU driver. Mesa's `libgbm`/`libEGL`/`libGLESv2`
sit in `lib/fallback` and are used only when the OS has no `libEGL.so.1` at all (software
rendering at best; `FABLE_GL_FALLBACK=1` forces them).

## Layout on the device
```
Fable.sh          the launcher (anywhere the OS launches scripts from, e.g. the ports folder)
Fable/            its files, in the folder of the same name next to it
  FableRecomp     the game (aarch64)
  lib/            bundled libraries
  game/           copy the PC game's folder here (Fable.exe, data/, ...)
  save/           saves, profile, settings (created on first run)
  log.txt         the last run's log
```

## Known requirements and limits
- **GPU driver.** The stock OS has no KMS/DRM, so SDL's KMSDRM video cannot start
  (`SDL_Init(video) failed: KMSDRM not available` in the log). The game then runs SDL on
  its dummy video driver (window bookkeeping and the pad) and opens EGL on `/dev/fb0`
  itself (`gles/egl_fbdev.cpp`): `libEGL.so.1` + `libGLESv2.so.2` (or `libmali.so`) from
  the OS, Arm's fbdev native window (a width/height struct), an ES 3.x context. The
  framebuffer size is the display size (`FABLE_SCREEN=WxH` overrides, `FABLE_FBDEV` names
  another device). Candidate libraries are probed in turn (`libEGL.so.1`, `libmali.so`,
  the usual Mali directories; `FABLE_EGL_LIB=<libEGL>:<libGLESv2>` names one), and when no
  window surface can be made on the framebuffer the renderer falls back to a pbuffer whose
  frames are copied into `/dev/fb0` (`FABLE_GL_WINDOW=window|copy|pbuffer` forces one).
  On an OS with KMS/DRM the bundled SDL2's KMSDRM path is used instead.
- **Diagnostics.** `Fable-diag.sh` (next to `Fable.sh`) writes `Fable/diag.txt`: the
  device's framebuffer, GPU libraries and kernel modules, then runs `FableRecomp --gl-probe`,
  which opens EGL the way the game does and flashes red, green and blue on the screen.
  `FABLE_RENDERER=dxvk` switches back to DXVK on Vulkan where a Vulkan ICD exists. The
  log (`log.txt`) names the EGL and GL driver and version on start. Two renderer
  shortcuts can be switched off to rule them out when something renders wrongly:
  `FABLE_GLES_NOVAO=1` (one vertex array object, attributes re-specified every draw) and
  `FABLE_GLES_NOSHADOW=1` (every render state re-issued whenever one changes). The GL
  calls are replayed by a render thread (`FABLE_GLES_THREAD=0` runs them on the game
  thread; `FABLE_GLES_QUEUE_FRAMES=N` allows N frames in flight, default 1). The periodic
  `gles: frame` log line counts that frame's buffer uploads (in-place ones stall a tiler) and
  reads the back buffer's centre pixel only with `FABLE_GLES_PROBE=1` (the readback drains
  the GPU). The graphics
  page's "Frame rate cap" (`fps_cap`, 30 by default here) holds the game to that rate: a
  steadier pace and less heat and battery than running the menus at 60.
- **GLES scope.** What the game uses on the PC, per a Direct3D call census: vs_1_1 and
  ps_1_1/1_4 shaders (translated to GLSL ES 3.00), the fixed-function pixel pipeline,
  pre-transformed UI quads, DXT1/3/5 textures (decoded on the CPU; Mali has no S3TC),
  render-target and depth textures, occlusion queries. No state blocks, cube/volume
  textures or multisampling.
- **Saves and the shader cache.** The game flushes a file on every seek and close; on the SD
  card each flush took tens of milliseconds on the game's thread, so a file that was written
  is flushed by a background thread instead (one flush outstanding per file). A save is
  therefore on the card a moment after the game reports it (typically well under a second, a
  few seconds on a slow card). Quitting through the game, the host menu's exit or Back + Start
  waits for those flushes, and SIGTERM (a firmware's quit) reaches the game as a window close,
  which quits the same way; a SIGKILL or a power cut in that moment can lose the save's last
  writes. `FABLE_SYNC_FLUSH=1` flushes on the game's thread as before. Shader programs are
  kept as driver binaries in `save/gles_programs.bin` (the 1024 most recently used; another
  GPU driver starts it over; `FABLE_GLES_PROGRAM_CACHE=0` turns it off): a program the game
  used in an earlier session loads instead of compiling (~40-50 ms each on the Mali-G31), and
  those of the last three sessions are created a slice per frame (~2 ms of the render
  thread, `FABLE_GLES_PROGRAM_WARM_MS`) after start. A frame over 100 ms logs what the
  renderer did in it (`gles: long frame`; `FABLE_GLES_LONG_FRAME_MS`).
- **Speed.** The x86 game runs as lifted C on four A53 cores at 1.5 GHz; expect a fraction
  of the Android frame rate. 720x480 keeps the GPU load low. The first device log showed
  gameplay at 7-8 fps with ~1300 draws a frame and menus at the 60 fps cap, so the cost is
  per draw: the renderer keeps GL state and uniform traffic to what changed (each program is
  sent only the constants and mirrored states that differ from what it last received;
  sampler parameters likewise; cached vertex layouts and texture bindings), builds a new
  shader program in one round trip to its render thread, never waits for occlusion query
  results, orphans dynamic buffers rather than writing them in place (a Mali stall), clears
  whole targets without the scissor (no tile load), compiles the alpha test (a `discard`)
  only into the programs of alpha-tested draws (a shader that can discard loses Mali's early
  depth update and forward pixel kill), and discards the depth buffer at the end of each
  frame. The host menu's statistics line
  shows the frame time and the part spent in GL, which tells the game's own CPU time from
  the renderer's. The lifted code is built at -O2 for the Cortex-A53 (`FABLE_GEN_OPT`,
  `linux-a64.cmake`), and `Fable.sh` puts the CPU and GPU governors into performance mode
  for the session (`FABLE_NO_PERF=1` leaves them).
- **Code layout.** The lifted code is ~56 MB of machine code in guest-address order, and a
  frame's work is spread thin over it (the hottest lifted function is under 2% of the
  samples), so the A53's 32 KiB instruction cache and its TLBs (10 L1 instruction entries, 512
  shared) miss on most calls. The build therefore links with lld and places the functions in
  `hot_symbols.txt` (written by `gen_hot_order.py` from a profile) first and together, between
  the markers `fable_hot_text_begin`/`_end`; at start the game copies that range onto
  transparent huge pages when the kernel has them (`huge text:` in the log says what it did;
  `FABLE_HUGE_TEXT=0` turns it off). `FABLE_PMU=60` (opt-in: off in `Fable.sh`, since the
  PMU support of Allwinner kernels varies; `Fable-diag.sh` runs `FableRecomp --pmu-probe` to
  test it) logs the busiest thread's hardware counters every minute (`pmu:`): IPC,
  instruction-cache and branch misses, and on the A53 the share of cycles lost to
  instruction-cache and instruction-TLB misses, the numbers that say whether the layout work
  pays. The A53 errata 835769/843419 workarounds are on (`FABLE_A53_ERRATA`).
- **Vsync.** Off in the first-run preset: a frame shows as soon as it is done instead of
  waiting for the next 60 Hz refresh. "Half" locks to 30 fps and is the smoothest choice
  once frames fit in 33 ms.
- **Memory.** 2 GB is tight; keep other apps closed.
- **Screen.** 720x480 is offered as a resolution; a new profile defaults to it. The UI
  scale is automatic (1x at 480 lines, so the HUD is laid out as on the Xbox).
- **Controls.** The built-in pad is an SDL game controller: the Xbox control scheme with the
  Xbox menus and camera (settings in `game/FableRecomp_controller.ini`). The stock pad has
  no mapping in SDL's database, so `gamecontrollerdb.txt` next to `Fable.sh` supplies one
  (`FABLE_PAD_MAPPINGS`); its raw button and axis numbers come from `FableRecomp --pad-probe`,
  which `Fable-diag.sh` runs (press the buttons in the order it names). Back + Start held
  for two seconds exits (`FABLE_PAD_QUIT=1`).
  Left trigger locks a target while held (`HOLD_TARGET`); right trigger held with X, B or A
  casts the assigned spells and Y cycles them; the d-pad is the Xbox quick-access and context
  item slots (the pad's slot bindings replace the keyboard's 1..9, which the device cannot press).
- **Host menu** (`FABLE_OVERLAY=1`, `posix/overlay/`): the pad's menu/function key (SDL's
  guide button; Back + Start held 1.5 s also works) opens a menu over the game: resume,
  graphics settings, volume, FPS counter, renderer statistics, log level, vsync, screenshot
  (`save/screenshot_NNN.bmp`), exit (closes the game's window; a hard exit follows after
  four seconds). The GLES renderer draws it in Present with an embedded 8x16 font
  (`overlay/gen_font.py`).
- **Graphics settings** (`FABLE_HOST_GFX=1`): the game's own Video Options entry is inert;
  the host menu's Graphics page holds what it had: resolution (fractions of the 720x480
  panel, scaled up on present), refresh rate, anti-aliasing (2x/4x multisampling), screen
  aspect ratio (auto = 3:2 here), landscape / mesh / texture / shadow / effects detail
  (each Low..Highest; shadow Low = no shadows; texture Low also stores the decoded DXT
  textures in 16-bit formats, RGB565 / RGB5_A1, half the memory and texture bandwidth:
  `FABLE_GLES_TEX16=1` forces that at any detail, `2` also turns smooth alpha into RGBA4,
  `0` keeps RGBA8), brightness, vertical sync (off / on / half
  = a 30 fps lock), plus draw distance, glow, water reflections and weather. Changes apply at once and are saved in `save/graphics.txt`; `Fable.sh`'s
  `FABLE_GFX_DEFAULTS` is the first-run preset, used only while `save/graphics.txt` does not
  exist (an upgrade keeps the player's settings; delete the file to get a newer preset). The
  preset is the PC's Low detail with an 80% draw distance, because the game's own CPU time
  per frame is the limit on this device: mesh detail Low draws objects to 0.75x their
  distance, at most 250 m, without the repeated ground meshes and z-sprite billboards (a town
  went from ~760 to ~545 draws a frame and lost two render-target clears); draw distance
  culls the meshes further out together with their bone and cloth work; effects Low has no
  weather, colour filter, glow, radial blur or 3D distortion; shadows and water reflections
  are whole extra scene passes. Landscape detail has no reader in the retail engine that we
  found (the PC menu fixes it at Normal; the device log shows no change). Raise draw
  distance and mesh detail first when there is CPU time to spare; lower the render
  resolution if the GPU is the limit (the statistics line shows the split).
- **Volume.** The OS's rocker does not reach the game's mixer, so with `FABLE_VOLUME_KEYS=1`
  a thread reads KEY_VOLUMEUP/DOWN from the evdev devices that have them and steps the host's
  master volume (`save/volume.txt`; also in the host menu).

## Building
```sh
# host tools: clang, lld (ld.lld: the hot-function order needs it; without it CMake warns and
# links with GNU ld, unordered), cmake, ninja, meson
# arm64 packages (dpkg --add-architecture arm64; libsdl2-dev:arm64 libavcodec-dev:arm64 ...)
meson setup build/dxvk-a64 <dxvk 2.x> --cross-file android/linux-aarch64.ini --buildtype release \
  -Denable_dxgi=false -Denable_d3d8=false -Denable_d3d10=false -Denable_d3d11=false -Dnative_sdl2=enabled
ninja -C build/dxvk-a64
cmake -S rebuild/recomp/posix -B build/linux-a64 -G Ninja -DCMAKE_TOOLCHAIN_FILE=rebuild/recomp/posix/rg34xx/linux-a64.cmake \
  -DFABLE_GEN_DIR=gen_win "-DFABLE_GUEST_DLLS=cfgdetect|ConfigDetect.dll|gen_cfg" "-DFABLE_GEN_OPT=-O2" \
  -DMINGW_HEADERS=<llvm-mingw>/generic-w64-mingw32/include -DDXVK_D3D9=build/dxvk-a64/src/d3d9/libdxvk_d3d9.so
ninja -C build/linux-a64
# a minimal FFmpeg (movie codecs only) keeps the package small; Ubuntu's would pull in ~150 libraries
(cd <ffmpeg 6.1 source> && ./configure --prefix=build/ffmpeg-a64 --enable-cross-compile --arch=aarch64 \
  --target-os=linux --cross-prefix=aarch64-linux-gnu- --cc=aarch64-linux-gnu-gcc --enable-shared --disable-static \
  --disable-programs --disable-doc --disable-everything --disable-avdevice --disable-avfilter --disable-network \
  --disable-postproc --enable-decoder=wmv1,wmv2,wmv3,vc1,msmpeg4v1,msmpeg4v2,msmpeg4v3,wmav1,wmav2,wmapro,pcm_s16le \
  --enable-demuxer=asf,avi --enable-parser=vc1,mpegaudio --enable-protocol=file --enable-swscale --enable-swresample \
  && make -j4 install)
# configure the game build with PKG_CONFIG_LIBDIR=build/ffmpeg-a64/lib/pkgconfig:/usr/lib/aarch64-linux-gnu/pkgconfig
# (the GLES renderer needs only the GLES3/gl3.h header; it links no GL library)
FFMPEG_PREFIX=build/ffmpeg-a64 rebuild/recomp/posix/rg34xx/package_rg34xx.sh build/linux-a64 build/dxvk-a64 out/rg34xx
```
Hot-function order: `hot_symbols.txt` is regenerated from a profile with
`gen_hot_order.py --gen-dir gen_win --log <device log.txt> -o rebuild/recomp/posix/rg34xx/hot_symbols.txt`
(the profiler's top-60 lines seed it, and the direct callees and callers of those functions fill it to
4 MiB), or better from a complete profile: `--counts` takes `<count> <symbol>` lines (a full
sample histogram, e.g. `perf report` of the x86-64 build), `--profdata` an instrumented run (below).
A new list needs only a relink. `-DFABLE_SYMBOL_ORDER=` (empty) builds without it.

Profile-guided optimisation (clang PGO): the instrumentation profile is keyed by the source, so a
desktop session trains the handheld build. `pgo_train.sh build/linux /tmp/pgo -- <play command>`
(a harness script that starts `$FABLE_BUILD/FableRecomp`, loads a save and plays in a world scene
for 10+ minutes) builds an instrumented x86-64 copy (`FABLE_PGO=gen`; needs `libclang-rt-18-dev`, or
`FABLE_PGO_RUNTIME=<libclang_rt.profile.a>`), runs the command, and writes `/tmp/pgo/fable.profdata`
and a complete `/tmp/pgo/hot_symbols.txt`; configure the aarch64 build with
`-DFABLE_PGO=/tmp/pgo/fable.profdata` and copy the ordering file over this directory's.

Smoke test on x86-64 (qemu-user, Mesa llvmpipe through `lib/fallback`, SDL on X11):
`SDL_VIDEODRIVER=x11 FABLE_RENDERER=gles LIBGL_ALWAYS_SOFTWARE=1 qemu-aarch64 Fable/lib/ld-linux-aarch64.so.1 --library-path Fable/lib:Fable/lib/fallback Fable/FableRecomp --game <game>`.

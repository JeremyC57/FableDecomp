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

## 2026-10-06: targeting void / invisible NPCs
- Root cause: the CRT `_trandisp` helpers (0x5855AD, 0x5855F0, behind acos/asin/exp/pow/log)
  return with ZF set and the caller branches on it right away. The lifter dropped flags across
  `ret`, so those functions took random branches and sometimes returned NaN (acos from
  0x85070, quaternion → angle), which tainted camera and bone matrices. The lifter now hands
  flags back through `recomp_lf_*` where the continuation reads them (8 call sites).
- Check: `FABLE_HEAVY_DRAWS=3000 FABLE_EXIT_FLIP=4450` + the LT input script (scratch
  `trun.sh`). Old lifter: ~16k NaN-constant draws/s from flip 2070, 21 void frames.
  New: 0 and 0 in 3 complete runs.
- The intro NaN at 0x73579 (normalising a zero vector in 0x3D70A0) is benign: same on x86.
- Known: the intro movie sometimes stalls under FABLE_NO_AUDIO in parallel test runs (pre-existing).
- Highlight outline: FIXED. The pass draws the target into 071E0080 256x256 with a stage-1
  PROJECT3D depth compare against the main depth buffer 072C0000 (texture format 0x2E), marks a
  shell in stencil bit 2, blurs to 07220080 128x128, and composites where bit 2 is set. A stale
  colour surface at 072C0000 won the address lookup, so the compare never ran; depth texture
  formats (0x2A-0x31) now pick the depth surface. Verified with `FABLE_CAPTURE_HIGHLIGHT=1`
  (green glow around an Oakvale NPC at flip ~2335).
- Shadow compares now use the texture's depth range: Fable's shadow maps are Z16 (format 0x30,
  clip max 65535) and were scaled as Z24, so character shadow tests passed almost everywhere.

## 2026-10-07: 16:9, resolutions, PC files
- Resolution = output height (480..2160); 640x480 surfaces render at outW_ x outH_ (16:9:
  1280x720, 1920x1080, ...), others scale by outH_/480. 16:9 widens the game camera
  (`hle_SetupGamut` 0x132D00, `hle_SkyBand` 0x161830); `FABLE_DISABLE=hor+` is the old squeeze.
- Interface at 16:9: screen-space draws (CPU-evaluated vertex program, w = 1, stage 0 not a
  render target) keep 4:3 proportions. A frame with a full-screen panel (menus, movies,
  loading screens) stays in the centred 4:3 area; otherwise touching HUD pieces are clustered
  per frame (`uiEndFrame`) and pinned to the left/right screen edge or the centre.
  `FABLE_DISABLE=uifix|uicorners|textsharp`.
- PC files (`pc_textures = <TLC folder>`, launcher switch): textures by mip-0 hash -> Xbox
  bank entry name -> larger PC entry (`pc_textures.cpp`); music: the WMA music source
  (0x5E1D70 ctor, 0x5E19B0 read, 0x5E17E0 seek, 0x5E1B20 dtor) reads `data/Sound/<name>.ogg`
  instead (stb_vorbis, resampled 48 -> 44.1 kHz; `music.cpp`, `FABLE_DISABLE=pcmusic`,
  `FABLE_MUSIC_DUMP=<file>`). Speech/SFX banks are identical on PC.
- Lifter quirk: a relift with a changed `--wrap` list shifts junk entries; add new wraps to an
  existing generated tree by hand (rename `F_x` -> `F_x_orig`, append the forwarder).
- Shadow maps: Z16 clears decode as Z16, AA shadow targets are not split; A/B against
  `FABLE_SHADOW_Z24=1` shows no brightness change.

- 60 fps: the game sets its own present interval from the region's LockToFrameRate via
  SetTargetFrameRate (0x3F700, n = ceil(refresh / fps), D3D state 0x862494, frame period
  +0x1C4); 0x3F820 forces n in some modes. Both wrapped (hle.cpp). The game loop
  (CMainGameComponent::Run 0x1F4F90) renders every pass, simulation = 15 server turns/s on real
  time with render interpolation, so 60 fps keeps game speed. The Sleep(1) loop at 0x3C420 is
  the XMV movie player (30 fps content). `FABLE_WAIT_LOG=1` profiles kernel waits by call site.
- Android quick menu (`QuickMenu.java`, swipe from the left edge): FPS counter (`perfStats`),
  runtime toggles (vibration, `hud_corners`, `text_sharpen`), 15 s profile to `<data>/profile.txt`
  (offsets into libmain.so; resolve with `llvm-nm -n` on the unstripped build-xbox/libmain.so).

- Occlusion: real Vulkan queries per draw while ZPASS counting is on (vk_renderer.cpp report()).
  Fable counts both test boxes (colour writes off) and real objects (temporal culling: an object
  counted 0 is skipped next frame until its box test passes). A count can span submissions (the
  presenter submits at every vblank): ranges are summed per count (ReportCount). Quick-menu /
  ini switch `occlusion`. Open: user video shows a wall section missing for seconds near the
  Guild (panorama shows through) - reproduce with their save.
- 16:9 side bars: a full-screen 4:3 panel fills the bars (opaque: its top-corner colour, so
  white beside the intro and black beside movies; blended over the game: black).
  FABLE_DISABLE=sidebars. HUD pieces are learned (texture + place) and keep their edge.
- Render passes: texture lookups no longer end the pass (was ~950 passes per frame).

## Next
1. Renderer: real occlusion queries (count passed fragments per CLEAR/GET_REPORT pair with
   Vulkan queries); dot-product texture modes; fixed-function T&L.
2. Launcher settings. Done: resolution scale, anisotropy, widescreen (anamorphic 3D
   squeeze + 16:9 present; `vk_pipeline.cpp`), custom Vulkan driver, volume. To do:
3. Android: `android/build_android.sh` builds the APK (launcher, touch controls, custom
   drivers). Not yet tested on a device.
4. Movies: frame presentation through the renderer works; check A/V sync.

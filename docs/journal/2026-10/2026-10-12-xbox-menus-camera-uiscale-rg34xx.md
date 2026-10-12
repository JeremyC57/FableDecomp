# 2026-10-12 — Xbox menus fixed, Xbox camera, automatic UI scale, RG34XX SP package

Recompiled game (`rebuild/recomp`), branch `port/modern-assets`. Android is the primary target.

## Done
- **Xbox chase camera** (`win/camera.cpp`, `XBOX_CAMERA`): `SetCameraManagerSetDef` 0x69AC90
  reads the region set def's PCMain at retail +0x44 (fields sit 4 bytes before the Ego_r PDB);
  patched to the Xbox pair (+0x3C/+0x40). The right stick keeps its angle on release and the
  camera turns to follow the hero.
- **UI scale** restored from 51fe109b with `0 = automatic` (the game lays out about 720 lines);
  Android launcher slider has an "automatic" position. Verified at 1280x720 x1.5.
- **Xbox pause list** showed one entry: `CList`'s view test (vfunc 185) uses
  `CComponent::GetSize`, and a `CMenuEntry` carries its sub-screen as children (600-770
  wide). `menu_nav.cpp` wraps GetSize (0x533D30) for that caller: a menu entry's extent is
  its label, and rows beyond the list's Up/DownLimit are outside (their wheel states get
  alpha 0, since the wheel's alpha bounces back up past the limit). The inventory list's
  jump on the last entry went with it. Pause list now matches the console reference.
- **RG34XX SP** (`posix/rg34xx/`): self-contained aarch64 Linux package (bundled glibc
  loader, libstdc++, SDL2, DXVK, minimal FFmpeg, Vulkan loader), `Fable.sh` + `Fable/`.
  Smoke-tested under qemu-aarch64 (device created at 1280x720). Needs a Vulkan ICD on the
  device, which the stock Anbernic OS does not have.

## Learned
- The profile's `Resolution N` could not be driven from the file for testing (every index
  fell back to 1024x768); `FABLE_RECOMP_MODES=1920x1080` (d3d9.cpp, POSIX) makes the
  synthesized mode list a single mode for tests instead.
- Front-end clicks after the capturing click reach `CManager::ProcessEvent` (26/28 from
  0x42E5B1) but did not press "Apply" in Video Options in the harness; untested with a real
  mouse. Pad focus: `hovered()` returns the first widget with +0x160 set, so a stale pad
  hover beats the mouse-hovered one.

## Open
- Verify the Xbox menus and HUD at 1920x1080 (auto scale 1.5) in the harness.
- Android APK and Windows build with all of the above; user tests on the phone and RG34XX SP.

## Direct3D 9 on OpenGL ES (later the same day)
The device test failed twice: `libpulsecommon-16.1.so` missing (Ubuntu's SDL2 links libpulse,
whose private half lives in `pulseaudio/`; the packager now searches that directory) and no
Vulkan on the RG34XX SP. The user asked for a Direct3D 9 → OpenGL ES renderer, for that device
only (Android and PC stay on DXVK).

- **Census first** (`posix/gles/census.cpp`, `FABLE_D3D_CENSUS=<dir>`): a proxy around the
  DXVK device counts every call and value and dumps the shader bytecode. 8000 frames of
  menus and gameplay: vs_1_1 only (always bound), ps_1_1/ps_1_4 (SetPixelShader(NULL) for
  the 2D layer), FVF XYZRHW UP draws, A8R8G8B8/X8R8G8B8/R5G6B5/A4R4G4B4/A8/V8U8/D16/DXT1-5
  textures, render-target textures, 256x256 D24S8 depth-stencil surfaces, 16-bit dynamic
  index buffers, occlusion queries. No lighting, state blocks, cube or volume textures.
- **Translator** (`dx9shader.cpp`): vs_1_1/ps_1_x bytecode → GLSL ES 3.00, incl. relative
  `c[a0.x]`, `_dw` projective texld, texbem, texm3x2pad/tex, cnd/cmp, phase. All 635
  census shaders translate and compile under Mesa.
- **Renderer** (`gl_renderer.cpp`, `FABLE_RENDERER=gles`): the COM interfaces implemented
  directly; the guest proxies and Lock staging in `win/d3d9.cpp` work unchanged on top.
  Everything renders into an FBO in Direct3D row order (the vertex epilogue negates clip y,
  `z' = 2z - w`), Present blits with a vertical flip. GL entry points come from
  `SDL_GL_GetProcAddress` so the binary links no GL library and uses the device's driver.
- Verified under Xvfb with Mesa llvmpipe: intro movies, front end, Load Game, the opening
  cutscene, gameplay with the HUD, and the Xbox inventory menu all match the DXVK output.
- Bugs on the way: the device creation took the native SDL_Window* for a guest HWND (black
  screen); cull winding was backwards (the mirrored clip y flips it: D3DCULL_CCW keeps GL's
  CCW), which culled every front face — the whole frame was black until fixed.
- `FABLE_GLES_TRACE=N` logs the first N draws (state, declaration, vertex data, GLSL);
  `FABLE_GLES_DUMP=<dir>` writes every frame as PPM; `gles: frame` stats every 120 frames.
- The RG34XX package now runs the GLES backend (`Fable.sh` sets `FABLE_RENDERER=gles`),
  bundles the pulse libraries, and keeps Mesa's GBM/EGL/GLES only as `lib/fallback`
  (the device's Mali blob must supply the real ones). Smoke-tested under qemu-aarch64 with
  the fallback libraries: intro movies render.

### Device test 2: no KMS/DRM
The GLES package started on the RG34XX SP but SDL's KMSDRM video was "not available" (the
stock OS is framebuffer + fbdev Mali blob, no DRM), so there was no window for the context.
Added `gles/egl_fbdev.cpp`: when SDL video fails under `FABLE_RENDERER=gles`, SDL is
re-initialised on its dummy driver (the pad and the window machinery still work) and the
renderer opens EGL directly on `/dev/fb0` with Arm's fbdev native window, loading
libEGL/libGLESv2 (or libmali) with dlopen. `w32::displayBounds` reports the framebuffer size
in that mode, so the game offers and defaults to 720x480. Tested on the PC with
`SDL_VIDEODRIVER=nonexistent EGL_PLATFORM=surfaceless FABLE_GL_WINDOW=pbuffer`: the whole
route to gameplay renders at 720x480 (frames via `FABLE_GLES_DUMP`).

### Device test 3: eglInitialize fails
`libEGL.so.1`/`libGLESv2.so.2` load on the device but `eglInitialize` returns
EGL_NOT_INITIALIZED (0x3001), and the framebuffer size read 720x960 (sysfs `virtual_size` is
the double-buffered height). Changes: `egl_fbdev.cpp` now probes candidate libraries in turn
(soname, then the Mali directories, `FABLE_EGL_LIB` override), tries the platform displays the
driver advertises (surfaceless, device) after the default one, and falls back from a window
surface to a pbuffer whose frames are copied into `/dev/fb0` (`swap()` handles 32/24/16 bpp).
`Fable.sh` no longer puts Mesa's fallback libraries on the path unless asked (they could have
shadowed the OS's EGL). `FableRecomp --gl-probe` and `Fable-diag.sh` collect what the device
has. Next step depends on `diag.txt` from the device.

### Device tests 4-6: it runs; pad, volume, overlay, glow
The GLES build ran on the RG34XX SP (menus ~28 fps). Follow-ups from the user's report:
- **Pad**: the stock pad is "ANBERNIC-keys" (GUID 19002cb4…); SDL's automatic mapping had
  Y on L1, the guide on R3 and the sticks as triggers. `FableRecomp --pad-probe` (run by
  `Fable-diag.sh`) prints raw events; the mapping in `rg34xx/gamecontrollerdb.txt` is from
  the user's probe (A b0, B b1, X b3, Y b2, L1 b4, R1 b5, L2 b10, R2 b11, Select b6, Start b7,
  hat, L3 b9, R3 b12, menu b8, sticks a0-a3). Loaded through `FABLE_PAD_MAPPINGS`.
- **Host overlay** (`posix/overlay/`): guide key or Back+Start held opens a menu drawn by
  the GLES renderer (8x16 font from `gen_font.py`): resume, volume, FPS, renderer stats, log
  level, vsync, screenshot (BMP), exit. `FABLE_PAD_QUIT=1`: Back+Start 2 s exits.
- **Volume**: the OS rocker never reached the game's mixer; `volkeys.cpp` reads
  KEY_VOLUMEUP/DOWN from evdev and steps a master volume in `audio.cpp` (`save/volume.txt`).
- **Hold-to-target** (`HOLD_TARGET`, controller.cpp): the profile's CControlsDef sits at
  CUserProfileManager+0x20, `ToggleZTarget` at +0x68; cleared in the GetPrimaryInputVector
  hook so LT locks only while held. (Only the patch is verified: no NPC was in range in the
  harness runs to see the lock release.)
- **NPC glow offset**: `FABLE_GLES_DRAWDUMP=purple:<frame>` dumps every draw of the frame
  after a purple patch appears. The glow is drawn into a 256x256 RT (draws 483-490: correct
  position), downsampled to 128x128, blurred twice (vs 269: texcoords + c[92..95]) and
  composited full-screen (alpha blend). Each pass shifted it half a source texel: the game's
  offsets assume Direct3D 9's integer pixel centres. The vertex epilogue now adds half a
  viewport pixel (`uHalfPixel`), as the RHW path did; DXVK does the same.
- Translator: `mov a0.x` rounds to nearest (Microsoft's vs_1_1 docs), not floor.
- **A in the Xbox pause menu did nothing** (user report). `CInputProcessInGameMenu`
  (0x690480) sends the Xbox list the select event (4) itself on action 33, but the pause
  screen leaves the live-GUI flag clear, so the front-end navigator took the event and,
  finding no clickables, turned it into a mouse click. The navigator now acts only on events
  whose return address lies in `CNewFrontendGameComponent::Input` (0x42E3EE..0x42E98F).
  The inventory sub-screen processors (map 0x68AEA5, experience 0x68B355, trade 0x68B83A,
  quests 0x68BF88) send A as a mouse press like the top-level list; converted to select too.
  Verified: Audio Options opens from the pause list, Current Quests from Quests.
  `FABLE_MENU_TRACE=1` logs the UI events with their return addresses at log level 1.

### Performance pass (device log: gameplay 7-8 fps at ~1300 draws, menus 60 fps)
Per-draw cost dominates. Renderer (`gl_renderer.cpp`): constants upload as the dirty range
since the same program's last draw (full set on a program change; element locations
queried at link), `glUseProgram`/vertex layout/texture binds only when they change, sampler
objects bound once, the state-mirroring uniforms (fog, alpha test, bump, viewport, half
pixel) only when a mirrored state changed, occlusion `GetData` returns the previous answer
instead of waiting for the GPU (the sun query polled with FLUSH serialised CPU and GPU),
dynamic buffers orphan + full upload unless NOOVERWRITE (in-place writes stall on Mali),
depth/stencil invalidated before Present. `frame X ms, gl Y ms` in the overlay statistics
splits the game's CPU time from the renderer's. Build: `FABLE_GEN_OPT=-O2` for the lifted
code, `-mcpu=cortex-a53 -fomit-frame-pointer` in the aarch64 toolchain. `Fable.sh` sets the
cpufreq/devfreq governors to performance for the session. Research: Arm's Mali guidance
(discard framebuffer contents, avoid mid-frame FBO switches) and Panfrost's dirty-state
tracking gains back the direction; the H700 governor paths are unverified, `Fable-diag.sh`
now lists them.

### Host-owned graphics settings + profile of the v9 device log
- User: graphics settings belong in the handheld overlay / Android launcher, with what the
  game's Video Options had (model, texture, effects detail, resolution). `win/gfx_options.cpp`
  wraps `CEngine::SetVideoOptions` (0xB24690): with `FABLE_HOST_GFX=1` the quality dwords
  (CEngineVideoOptions +28..+44: landscape, mesh, texture, shadow, effects; +48 AA forced 0)
  and the resolution (+0/+4, a 50-100 % scale of the window; the GLES renderer blits the
  back buffer up) are replaced by the host's; the Video Options button in the front end is
  swallowed (`menu_nav.cpp`, action 13). Draw distance %, glow, water reflection and weather
  act on the four `CEngineVideoOptionsDef`s at CEngine+0x120..0x12C.
- **Def layout**: a raw dump showed retail is Ego_r's layout minus four bytes
  (HiresTextureMemory +0x3C = 0/6/10/16 MB, LODErrorTolerance +0x40 = 3/2/1/0,
  DrawDistanceMultiplier +0x48 = 0.75/1/1.25/1.5, DrawDistanceMaximum +0x50 = 250/1000,
  ShadowBufferSize +0x64 = 0/1024/2048/4096, bools from +0x6C). CEngine's quality indices
  are its CEngineVideoOptions copy at +0xC0 (texture +0xE4 -> ResizeHiresTextureMemory,
  effects +0xEC -> CShaderBankStateBlock::Set), also Ego_r minus 8, like the def slots.
- **Profile (v9, `FABLE_PROFILE=60`)**: the profiler never saw the main table
  (`recomp_table_entries` only listed the DLLs), so lifted code appeared as thousands of
  `FableRecomp+0x...` lines; resolved with the unstripped binary: `recomp_lookup` alone was
  5017 of 58954 samples (8.5 %, ~14 % in gameplay windows) - a binary search per indirect
  call. Replaced by an open-addressing hash of every table (`runtime.c`), rebuilt when a
  DLL registers (old table kept for readers). Other visible host costs: D3DX texture
  resample/encode (~3 %), Mali driver threads (ioctl, cond/rwlock), log writes.
- **Hero save crash** (device and local, under gdb): `CTCInGameMenu::SaveGame` (0x47E196)
  dereferences the PC menu object at this+0xEC after writing the files (re-registers its
  observer part with the UI manager, then its virtual 3 with the slot number). Only
  `OpenInGameMenuPC` sets that member (it is the UI_MOUSE_POINTER component the PC menu
  creates); the Xbox pause menu leaves it null. Wrapped (`host_save_game`, menu_nav.cpp): a
  stand-in observer takes the calls and is removed from the manager afterwards. Its virtuals
  need the real CObserver argument counts (vtable 0x1245DBC: notify(CObservable*, EEvent) is
  slot 0 with two arguments; the manager notifies the stand-in while the save runs): with
  plain `ret` stubs the stack was corrupted and the game crashed after "saved successfully".
  Verified: Hero Save from the Xbox pause list writes the .hs and shows the success box.
- Verified: with `FABLE_HOST_GFX=1` the front end's Options list skips Video Options for the
  pad (Gameplay -> Audio) and Audio Options still opens.
- **Front-end text sits low in its highlight bars** (user report, main menu and Options):
  identical under DXVK and GLES, and the UI scale is 1.0 at 480 lines, so it is neither
  renderer nor host_rtdims; the game lays text out from its own fonts.big metrics (no GDI
  metrics are used). The Xbox lists centre their text. Next: find the PC front-end button's
  text placement (CNewFrontendGameComponent) and compare with retail.
- Harness gotcha: a single entry in `FABLE_RECOMP_MODES` (640x480 or 1280x720 alone)
  crashes `CUserProfileManager::SetDefaultValues` (0x40C790, a mode-table index) at start;
  always offer two modes. Stale manual saves in the test data dir change the load list and
  the route's timing (delete `Manual - Save*` before a run).
- Known cosmetic: the Xbox save confirm screen ("Are you sure?") shows two blank list rows
  under the question (user report).

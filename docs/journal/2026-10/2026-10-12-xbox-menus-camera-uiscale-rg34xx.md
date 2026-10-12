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

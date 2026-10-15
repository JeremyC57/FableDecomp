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
- Render resolution verified in the harness (83 %, 75 %: the engine resets the device, the
  GLES back buffer shrinks and is blitted up); the first version scaled the already scaled
  copy on every re-apply (640x480 -> 320x224), now the game's own request is the base.
- Harness gotcha: a single entry in `FABLE_RECOMP_MODES` (640x480 or 1280x720 alone)
  crashes `CUserProfileManager::SetDefaultValues` (0x40C790, a mode-table index) at start;
  always offer two modes. Stale manual saves in the test data dir change the load list and
  the route's timing (delete `Manual - Save*` before a run).
- Known cosmetic: the Xbox save confirm screen ("Are you sure?") shows two blank list rows
  under the question (user report).

### Second device profile (v10: lifted functions named) and the next performance round
Gameplay at everything Low: frame 55-100 ms, 20-30 ms in GL, 500-1000 draws. The guest
half is flat (the top game function, CGuiComponent::Update, is 2.5 %; scene traversal,
LOD, animation, text, particles each 1-2 %), so there is no single function to replace
natively. What the profile did single out, and what changed:
- **COM/D3D call layer** (the bucket after the last ConfigDetect function: host_camera_set,
  host::com, the ovr_* D3D methods, 2.5-3.8 %): every D3D call from the guest went through
  `onUnknownTarget` with an SRW lock acquire/release (`__pthread_rwlock_*` were 700 samples
  on their own). The trap table is a fixed array with a published count: no lock.
- **Indirect calls** (the bucket after the last Fable function: recomp_lookup/dispatch,
  3.3 % even with the hash): the lifter now emits a per-site inline cache
  (`RECOMP_ICALL`: a 64-bit (index, address) word per call site, `recomp_fns[]` indexed);
  the hash maps to indices and is half the size.
- **Vsync**: the game presented with interval 1, so a 45 ms frame waited for the next
  refresh. `FABLE_VSYNC=0` is the handheld default; the host menu cycles off / on / half
  (interval 2 = a 30 fps lock once frames fit in 33 ms). The statistics line and the
  `gles: frame` log line now split GL time into swap and texture uploads.
- **Texture uploads**: the HUD re-locks small textures every frame (text); an identical
  level is not re-uploaded (FNV hash, levels up to 64 KB).
- **D3DX fills**: resample/decode/encode were ~2 % (runtime texture streaming through the
  generic conversion path); the first 20 converting fills are logged with formats and
  sizes to see why the raw copy is not taken. DXT is still decoded on the CPU to RGBA8 (no
  S3TC on the Mali blob): an ETC2/ASTC transcode cache is the next big GPU-side lever.
- **Hero save left the Xbox save screen dead** (user report; Start still left): the PC
  flow leaves through the mouse-pointer notification. `menuAfterSaveTick` closes the menu
  with `CloseInGameMenu` (0x47D78B, what Start calls) at the next input update.
- **Native resolution = the window**: the device profile asked 640x480 (an older profile)
  and was stretched over the 3:2 panel; with host-owned graphics the scale's base is the
  window (`w32::screenSize`).

### The full Video Options set in the host menu (user request)
Resolution (as pixel sizes), refresh rate, anti-aliasing, screen aspect ratio, landscape /
mesh / texture / shadow / effects detail, brightness, vertical sync, plus the direct levers.
Each one acts: resolution, refresh rate, aspect ratio (CEngineVideoOptions +8/+12 as the
float pair the engine keeps, auto = the window's reduced ratio, 720x480 -> 3:2), vsync (+24)
and anti-aliasing (+48, the D3DMULTISAMPLE value) are written into the options at
SetVideoOptions; the GLES renderer now has a multisampled back buffer (ES 3.0 renderbuffers,
resolved into the single-sample colour texture whenever the back buffer is read:
StretchRect, GetRenderTargetData, Present, screenshots; a StretchRect into the back buffer
lands in the resolved buffer since a blit cannot write a multisampled one) and reports 2x
and 4x in CheckDeviceMultiSampleType; brightness is a gamma exponent (2^((50-b)/50)):
the GLES present pass draws the frame through it (a plain blit at neutral), the Direct3D
path sets a gamma ramp on the native device (DXVK applies it in its presenter). The
overlay's old VSync item moved onto the page. The Android launcher has the same controls.
Fixed on the way: the overlay (and now the present pass) left the default sampler on
texture unit 0 and the sampler tracking never rebound the game's.

### Cosmetic fixes: front-end text in its bars, blank rows on the save confirm lists
- **Text low in the highlight bars** (main menu, Options, the header bars). Measured with
  the game's own pointer hover (so not the navigator): 720x480: bar 257..286 (29 px), caps
  267..283 (3.5 px low); 1024x768: bar 411..458 (47 px), caps 420..441 (4 px high). The
  front end's layout is 640x480 design units scaled by the screen (positions x1.6 at
  1024x768), the bars scale with it, but the glyphs are drawn at the font's pixel size
  (20 px lines at both sizes; fonts.big holds fixed sizes) and the label is anchored a
  fixed 9-10 px below the bar's top edge, so it is centred at no size. `NUISystem::CText`'s
  draw (0x54EF00, wrapped as `host_text_draw`) takes its anchor from this+0xF8/+0xFC (the
  component's screen position; +0xCC is the glyph scale, not an alignment factor); the wrap
  moves the anchor by (29 sy - 16) / 2 - 10 px, sy = the front end's screen height / 480
  (-3.5 at 480 lines, +5.2 at 768): verified centred at both sizes. Front end only
  (0x13B8790, the in-game GUI, is null there); `FABLE_TEXT_CENTER=0` keeps the game's
  placement; `FABLE_UI_TRACE=1` dumps the text components and the list labels.
- **Blank rows around "Are you sure?" / "Overwrite?"**: still open. The confirm lists reuse
  the five-row list; its spare rows are entries of the same class as the labelled ones.
  Hiding them through the view-area test crashes when the list then touches their (absent)
  wheel states, and no field found so far tells a blank label from a labelled one (CText
  +0x15C is null for labelled rows too: the pause list lost Load / Audio Options when it was
  used). Next: find where SetToSave builds the confirm list and what it puts in the spare
  rows.
- **The pause panel's picture sat up-left of its ring** (user report). Aligned at 640x480
  (the Xbox zoom is exactly 1 there), far off at 1024x768 (zoom 1.6): a component tree
  dump (`FABLE_UI_DUMP=<s>`, from the pause list's root) showed every component zoomed
  1.125 x 1.0 at 720x480 but the picture sprites (vtable 0x122F5D4, children of the list
  entries) with render position == position, 442,165 unscaled. `CComponent::UpdatePosition`
  (0x52FFD0) ends by computing the render position and draw anchor: a component whose
  virtual +0x198 says it is absolutely placed keeps its position as given, the others get
  parent position + position x parent zoom; on the Xbox at zoom 1 both agree. Wrapped
  (`host_update_position`): in the Xbox screens an absolute component's render position
  and anchor are multiplied by its own render zoom (`FABLE_XBOX_ABSOLUTE_ZOOM=0` restores).
  The GetUIScale call sites in ChangePositionDelta, CSprite's size and the list's own
  layout were given the 1.6 as well (no visible effect, kept for consistency;
  `FABLE_XBOX_ZOOM_SITES=0/1/2`).

### Third device log (v13) and the 30 fps round
Gameplay at everything Low: ~48 ms frames, ~21 ms in GL (swap 0, uploads 0: pure draw
submission in the Mali driver), ~27 ms game. The pause menu itself costs ~22 ms of CPU
(2000+ components updated per frame). Changes:
- **Hero save left the game paused** (user report): `CloseInGameMenu` only closes the
  screen; Start's path (0x636270) also removes the menu's input process. The after-save
  close now goes through `CloseInGameMenuViaGameEvent` (0x47D21C), the game event that
  closes the menu and resumes.
- **Profiler markers**: the samples after the last lifted function (the runtime's dispatch,
  the COM layer) were charged to that function (8.6 % as "CFontBankStateBlock::CreateCRC").
  The resolver now carries the addresses of recomp_dispatch / dispatch_ic / lookup / rdtsc
  and the COM entry points as named markers.
- **`_ftol2` (0xBFEA70) hooked** (`host_ftol2`): the CRT's truncating double -> int64, 2004
  call sites, 1.9 % of samples through the lifted x87 control-word dance; the host reads the
  stack top and truncates.
- **Vertex array objects**: a draw whose buffers changed re-issued 16 attribute disables,
  a buffer bind, an attribute pointer and an enable per element (~20 GL calls per mesh).
  The renderer caches one VAO per layout key (declaration, shader, program, buffers,
  offsets, strides) and binds it; the element buffer binding is tracked per object; objects
  are dropped when a buffer, declaration or shader they reference is destroyed, and the
  overlay and present passes use their own object.
- **GL state shadow**: a render-state change between draws re-applied every state
  (viewport, depth, blend, colour mask, cull, stencil, polygon offset: ~20 calls); the
  renderer now keeps the values last issued and sends only what differs; passes that set
  state directly (Clear, ColorFill, StretchRect, the overlay, Present) invalidate it. With
  the VAOs, GL time under llvmpipe fell from 62 to 55 ms per gameplay frame (the
  software rasteriser dominates there; the per-call share is what the Mali driver pays).
- **ThinLTO option** for the lifted code (`FABLE_GEN_LTO`, lld): direct calls between
  small lifted functions inline, which keeps the guest registers in host registers across
  them instead of spilling to the Ctx at every call. Measured on the x86 harness (same
  gameplay route, frame time minus GL time = game + runtime, average of the in-game frames):
  -O1 26.9 ms, -O2 25.0 ms, -O2 + ThinLTO 25.3 ms. -O2 is what the handheld build already
  uses; LTO buys nothing measurable (the hot paths are the hash dispatch and the GUI
  component walks, not call overhead between small functions), so the option stays off.
- **White terrain regression** (user report on the harness screenshots): the landscape
  rendered flat white since the build that added the VAO cache and the state shadow; trees,
  grass and meshes were fine. Bisected on the harness with each mechanism off: the cache was
  the cause (`FABLE_GLES_NOVAO=1` brought the grass back with the shadow on). Root cause:
  `Buffer::unlock()` uploaded an index buffer by binding it to GL_ELEMENT_ARRAY_BUFFER,
  which in GL is state *of the bound vertex array object*: every Lock/Unlock of an index
  buffer silently replaced the element buffer of whichever cached object was bound, while
  the object's recorded binding (and `boundIbo`) still named the old one, so a later cache
  hit skipped the bind and drew the terrain with someone else's indices. Buffer uploads
  (create and unlock) now go through GL_COPY_WRITE_BUFFER, which is plain context state.
  A second bug found on reading the shadow: after a pass that sets GL state directly it
  re-issued only the enables, while the dependent values (blend function, depth function,
  stencil parameters, scissor box, polygon offset) kept their "known" values although the
  overlay had changed them (SRC_ALPHA blending) -> a draw with the same values as the
  shadow skipped the call. The shadow now resets every field to "unknown". The FVF
  declaration is cached per FVF value instead of being rebuilt (and its VAOs dropped) on
  every SetFVF. Diagnostics: `FABLE_GLES_NOVAO=1`, `FABLE_GLES_NOSHADOW=1`. Verified on
  the harness with both on: the Oakvale grass is back (v14 package and APK).

### Fourth device log (v14) and the render thread
Gameplay in Oakvale: 680-820 draws per frame at 47-70 ms, 1220 draws at 115 ms. GL time
is linear in the draw count at ~35 us per draw whatever the scene (the Mali blob's CPU
cost per draw call on the A53, not the GPU), the game's own share 30-70 ms. The profile is
flat: half the samples in the guest spread over hundreds of functions (animation, the GUI,
the landscape, the audio DSP's FFT and convolution run on the main thread), the hash
lookup 4.8-7.5 %, the COM trap layer 2-3 %, memcpy ~3 %, Mali's own code appears only as
a per-address tail (the profiler now prints per-module totals so a stripped library's cost
is visible). Levers taken, all visually neutral:
- **Render thread** (`gles/gl_queue.*`, `gen_gl_queue.py`): every gl* the renderer calls is
  a generated wrapper that records the call (arguments, and a copy of any data it reads)
  into a ring the render thread replays on the context it owns. Calls whose result the
  game thread needs (glGet*, glCreate*, glCheckFramebufferStatus, glReadPixels) round-trip
  with a Sync; glGen* names come from a pool refilled 64 at a time; host work that must
  run on the context (the swap, the frame readback and log line, eglSwapInterval, the
  occlusion-query check) is queued as a function. Present records a frame boundary and
  the game thread waits until at most one frame is unreplayed, so a frame costs
  max(game, driver) instead of their sum (4 cores on the H700; one was busy before).
  `FABLE_GLES_THREAD=0` turns it off, `FABLE_GLES_QUEUE_FRAMES` sets the frames in flight,
  the frame log line shows "render thread N ms, waited M ms".
- **Hash lookups**: `callImpl` (host -> guest calls, the menu wraps) caches the lookup per
  target; the lifter gives the 522 direct calls to unlifted targets and the unresolved
  indirect jumps an inline cache (`RECOMP_ICALL`) instead of `recomp_dispatch`.
- **Profile creation error** (user report, screenshot): the game only probes
  `Documents\My Games\Fable\Saves\Default` (the installer creates it on Windows) and
  reports "error creating the profile, check your disk space" when it is missing; the host
  creates the tree when the Documents folder is resolved, and Fable.sh too.
- **`restrict` on the lifted functions' context pointer**: lets the compiler keep the
  emulated x87 stack and control word in registers across guest memory writes (a guest
  store can alias nothing in the Ctx). Measured on the harness; see below.
- Verified on the harness (v15 package and APK): the threaded renderer draws the front end
  and Oakvale exactly as before; under llvmpipe the render thread is the bottleneck
  (80-90 ms per gameplay frame of software rasterising) and the game thread's own share
  is 14 ms, so the overlap is visible there as the game thread waiting. The first build
  crashed at exit (exit() with the thread alive destroyed its statics under it; now an
  exit hook stops it).
- **`restrict` adopted**: back-to-back on the harness (same route, render thread on, the
  game thread's own share of a gameplay frame = frame time minus the wait for the render
  thread): 16.2 ms plain, 8.4 ms with `Ctx* restrict c` on every lifted function. The lifter
  emits it now (v16 is a full re-lift and rebuild).
- **v16** = the re-lift with `restrict` (158 of 161 lifted files change) plus the inline
  caches on the formerly uncached dispatch sites; verified on the harness (gameplay,
  clean exit; the game thread's own share 11.6 ms per gameplay frame). Open: one run in
  six stalled after the dream-sequence movie completed (the completion event was logged
  but the game never destroyed the graph, no further frames, and SIGTERM did not end the
  process). Not reproduced since; `stall.sh` in the scratch area attaches gdb to a stalled
  run to capture every thread's backtrace next time.

### Fifth device log (v15) and the user's Guild test
Gameplay in the Guild: 41 ms frames, the render thread 27 ms of it hidden (waited 0): the
game thread is now the whole frame. Module totals: guest 36-40 %, Mali 23-30 % (the render
thread), libc 18-25 % (15 % of that `sched_yield`: the render thread's idle spin and the
sync waits yielded in a loop; both now spin for 50-100 us on the clock and then sleep),
runtime 7.5-9.6 % (`recomp_lookup` 5.9 %: v16's inline caches). Fixes from the report:
- **Inventory crash on A over a category** (crash log: `CTCInventory::ConstructScreenPC`
  -> `GetItemsListUIComponentPC` -> `CDataBankGroupEntry::BuildGroup` -> a bad string).
  The UI manager's select handler (0x4200AB) picks the PC virtual (+0x15C) when the player
  GUI's byte +0x2BE is set and the Xbox one (+0x134 -> `CTCInventory::GetItemsListUIComponent`,
  0x590235, still present) otherwise; with the Xbox screens up the PC builder got the Xbox
  category component. `host_inventory_screen` (wrap 0x591E5E) calls the Xbox builder when
  the Xbox inventory is open.
- **Target lock toggling with HOLD_TARGET**: the game constructs both input processes
  (hold: vtable 0x12375DC, on-press: 0x1237604) and registers the on-press one from the
  profile's ToggleZTarget, which a loaded profile sets again after host_primary_inputs
  cleared it. `host_ztarget_press` (wrap 0x6882F0) runs the hold variant's ProcessInput
  (0x688260, same object layout) on the on-press object when HOLD_TARGET is on.
- **Frame rate cap** (`fps_cap` in graphics.txt, "Frame rate cap" on the graphics page,
  off/20/25/30/40/60; the handheld defaults to 30): the game thread sleeps out the rest of
  the period after Present, so idle cores sleep instead of running 45-60 fps menus, for
  steadier pacing, less heat and battery.
- **Quest start / completion screens in PC format**: not fixable the way the inventory
  was. `CTCQuestCompletionUI::ActivateReal` (0x5E5F30) builds the screen from PC_ components
  only (PC_QUEST_COMPLETION_MENU_START/COMPLETE/FAIL/DEATH, PC_UI_QUEST_*_FRAME, ...); the
  Xbox code for those screens is not in the PC executable, so an Xbox-style version would
  be a new screen written in host code against the UI component system.
- **Open: a harness crash at the first stick input after loading a save** (twice in ~12
  runs, not reported from the device): `CInputManager::Update` -> `EventContainer_Erase` ->
  `CChangingStateComponent::ForceState` reading 0xBF800000 (a float where a pointer is
  expected). The pad menu code's hover handling (`setHovered`: OnHoverEnter/Leave and the
  state refresh +0x214) on the front end's last pressed button leaves a deferred state
  event behind; the button is destroyed by the level load and the event is processed by
  the next input update in game. Not fixed yet: the fix is to leave no deferred GUI event
  behind when a press changes the screen (drain or avoid the refresh on that press).
- **Water** (flat blue moat at the Guild, flat grey sea at Oakvale): a draw trace of an
  Oakvale frame (`FABLE_GLES_DRAWDUMP=<file>`, touched from outside at the right second)
  shows the sea drawn by ps 561/562: `texm3x2pad`/`texm3x2tex` of a 128x128 dynamic bump
  map (the engine renders the wave normals into it each frame) into the environment map,
  then `mix(env, pc[0], pc[0].w)`. The translator dropped the source modifier of the bump
  operand in texbem/texm3x2 (`_bx2`: 2x - 1 for an unsigned normal map), so the lookup
  used unsigned normals; now applied. The fixed-function pixel emulation also grew to four
  stages with BUMPENVMAP, MULTIPLYADD and LERP. v18 carries both; the moat is the user's
  test (the harness cannot be steered out of the Guild hall).
- **Device crash in v17** after ~20 minutes, entering Lookout Point: `strlen` of 0xA96 from
  `C3DAnimationInfo::GetAnimation` <- `C3DAnimationManager::BlendAnimations` (a bad
  animation name pointer). First seen with v17 (the restrict re-lift, the inline caches on
  all dispatch sites, the hold-target handler on the on-press object). Open; the next log
  decides whether it is the target-lock handler (lock a target, then wait) or streaming.
- **Target lock still a toggle (user report, v17-v19)**: both activation handlers
  (0x688260 hold, 0x6882F0 on-press) only ever queue the activate action; the lock ends in
  `CInputProcessZTarget::Update` (0x688C40), which every frame reads the controls def's
  ToggleZTarget byte through `CGamePlayerInterface::GetToggleZTarget` (0x445B80: byte +0x48
  of the def at this+0x7A0) and, when it is clear, deactivates as soon as the activate
  action is no longer held. The earlier override wrote the profile manager's +0x68 (the def
  taken as embedded), the wrong object. `host_ztarget_toggle` (wrap 0x445B80) now returns
  clear with HOLD_TARGET; the on-press wrap is gone.
- **Open (user report)**: Guild guards present during the childhood and teenage sections;
  Will mode (right trigger held plus a face button) does not cast. The Xbox scheme's
  binding records are now logged once at load to see how the scheme binds the spell
  actions. The dialogue-scene crash did not recur in this run.
- **Will mode root cause (2026-10-13, from the scheme dump)**: `FABLE_XBOX_CONTROL_SCHEME`
  binds ACTIVATE_SPELL_MODE (86) to the right trigger and ASSIGNABLE_SPECIAL_MOVE (54) to
  X, B and A; QUICK_ACCESS_ITEM (55) and CONTEXT_SENSITIVE_ITEM (56) to all four d-pad
  directions; CYCLE_THROUGH_SPELLS (57) to Y; CHARGE_GUILD_SEAL (78) has no pad binding
  (the seal is a context item). `CGamePlayerInterface::SetControlScheme` (0x446EF0) walks
  the bindings once and pushes one slot object per record of 54, 55 and 56 onto the lists
  at +0x7B8/+0x7C4/+0x7D0, in record order and whatever the record's controller type;
  `GetAssignableSpecialMoveIndexForButton` (0x445EE0 and friends) returns the position of
  the first slot object whose button matches. The merged vector put the profile's keyboard
  records first, so the pad's X/B/A landed at spell-slot indices 3+ and the d-pad items
  past the hero's item slots: nothing cast, nothing used. `mergedBindings` now drops the
  profile's 54/55/56 records when it appends the pad's (logged once: "N keyboard slot
  bindings ... replaced by the pad's"). With a pad in native mode the keyboard's 1..9 quick
  slots are gone, which the RG34XX cannot press anyway.
- The hold-to-target build (`host_ztarget_toggle`) first failed to compile (`retStd` without
  its namespace); fixed, x86 and aarch64 rebuilt.
- **D-pad items crashed the PC HUD (harness, after the slot fix)**: `CPlayerGui::CPlayerGui`
  (0x43B570, at 0x43C2A4) always constructs `CDrawQuickAccessIconsPC`; there is no Xbox
  drawer in the PC exe, only the shared base `CDrawQuickAccessIcons::Draw` (0x652FA4). The
  draw indexes the hero's item ids with the PC GUI def's icon counts (nine quick slots plus
  the context icons) through `CTCInventory::GetQuickAccessItem` (0x581E33), which falls
  through from the quick-access vector (this+0x154) into the context-item vector (this+0x148)
  with no upper bound; with the Xbox scheme's four quick slots the context lookups ran off
  the end and `GetDef` crashed on the garbage id (also through
  `GetContextSensitiveItemGraphicIndex` 0x581FA4 from the PC drawer's own loop). Two wraps
  bound both lookups (`host_quick_access_item`, `host_csi_graphic`, 0 past the end) and the
  second logs the slot, count and id once per change.
- **Black adult head (user report, v20; reproduced with the post-graduation save)**: the
  hero's face is composed on a render target (CSurfaceCompositor, fixed-function, three
  layers) and then copied into a DXT1 texture through `D3DXLoadSurfaceFromSurface`. The
  GLES renderer's `Surface::LockRect` succeeded on every surface, handing back a zeroed CPU
  buffer for a render target, so the copy read black (D3D9 refuses that lock, which is what
  sends D3DX through `GetRenderTargetData`). The lock now reads 32-bit render targets back
  from GL first. Found with `FABLE_GLES_DUMP_DRAWS=280-300`: the head is draw 297 of the
  Guild frame, a skinned draw sampling a 256x256 DXT1 texture, and the composites captured
  with `FABLE_GLES_DUMP_RT=all` were correct.
- **D-pad (user report)**: with the Xbox scheme, actions 55 and 56 both sit on the four
  directions; `CWorld::EAUseQuickAccessItem` (0x62ABA0) routes slots below the context count
  to `UseContextSensitiveItem` (0x583A92, the dynamic ids at inventory+0x154) and the rest to
  `UseQuickAccessItem` (0x5840A6, the fixed assignments at +0x148, pre-filled by the PC exe
  with a potion and three expressions), and -2 charges OBJECT_GUILD_SEAL_1 (the PC's G key).
  The wraps give the Xbox layout (up: `OpenQuickAccessMenu` 0x583BD5, down: the seal,
  left/right: context items) and, with the right trigger held, the favourite slots (the
  controller-help screen's "Access Favourites (D-pad)"). The quick-access menu opens with
  "Items x N / Expressions x N" rows but no panel frame (open).
- **Quest table / quest start / quest completion screens oversized (user screenshots, v20)**:
  `host_ui_scale` multiplies `CManager::GetUIScale` (window / 1024x768) by 1.6 at the zoom
  sites for the Xbox screens (laid out for 640x480); the PC-layout quest screens got the
  same 1.6 and ran off a 720x480 screen. The wrap now skips the 1.6 when the largest box
  up the component's ancestor chain exceeds 640x480 (NUISystem::CComponent per the PDB:
  Name +0x24, PParent +0xD8, Size +0xDC, DefIndex +0x158). Open: the component is taken
  from esi, which proved not to be `this` at every site (the first version read garbage
  and crashed once in the harness, shipped as v21; v21b makes every read fault-safe and
  trusts only a candidate with a vtable in .rdata), and the harness logs still show
  0x0 boxes, so the rule has not yet fired: the quest table in the harness still clips on
  the right. Next: take the component from the call site's own frame (each site's `this`
  register verified in the lifted code) or flag the PC screens from CPlayer's
  InitModeQuestCompletionUI (0x633B10) / UninitModeQuestCompletionUI (0x633B40) and the
  quest table's opener. The harness reaches the table from the post-graduation save with a
  short left nudge of the stick and A (adult14.sh).
- **v21/v21b (RG34XX)**: the d-pad layout and favourites, the adult head fix, the quest
  scaling attempt above. Open besides it: the quick-access menu's panel frame, the
  save/load list on the device, the PC key labels on the quick-access icons.
- **v22 (2026-10-14)**: PC screens fit and take the pad.
  - D-pad down: CHARGE_GUILD_SEAL (78) bound to it in `mergedBindings`; the wrap no longer
    uses OBJECT_GUILD_SEAL_1 directly (that opened the teleport menu at once). Left/right:
    only the context item of the moment, nothing otherwise (the fixed assignment under them
    was the PC's default expression).
  - Oversized PC screens: `host_create_component` wraps `CManager::CreateComponent`
    (0x41DB1D; the def name is a CCharString -> CCharStringData {char*, length}); a PC_ name
    turns the Xbox 1.6 in `host_ui_scale` off, a UI_ name back on. The quest table is
    PC_QUESTS_SELECTION_MENU and now fits 720x480 at the PC layout's own scale. The
    component-walk attempts before it failed: esi is not the component at every zoom site.
  - Pad on PC screens (`host::menu::pcScreenPad`, from the joystick hook): while a PC_ screen
    with clickable buttons is up in play, up/down and A go to the manager as UI events (the
    PC lists' entries are not buttons), left/right move between buttons, B presses the Back
    or close button, the right stick presses the description pane's scroll arrows; those
    inputs are kept from the game. Harness: the quest list steps with the d-pad and B closes.
  - Harness: the front-end crash in `EventContainer_Erase` (reading 0xBF800000) hit once more
    before a save loaded; still harness-only.
  - Open: save/load list on the device, quick-access panel frame, Guild river water.
- **Crash entering an inventory category (user report, v22b: Equipment -> Clothing)**: the
  UI event handler (CManager vfunc_14) builds a selected category through its PC constructor
  (vtable +0x15C) when the player GUI's live-GUI byte (+0x2BE) is set and the Xbox one (+0x134)
  otherwise (0x4200B0), and the builders under it pick PC_ or UI_ parts by the same byte
  (`CTCInventoryAbilities::GetMenuEntryFromCategory` 0x5EE288). The host opens the Xbox
  inventory through the live-GUI action, so the byte is set: Clothing built PC_SUB_LIST /
  PC_UI_FRAME / PC_DESCRIPTION_VIEWPORT into the Xbox screen and jumped through a null; the
  Skills -> Magic categories looked up PC_MENU_ENTRY_* children in UI_ screens. Fixes:
  `host_ui_event` runs a select from the inventory processors with the byte clear (a toggle
  made inside carries over); `host_find_child` retries a failed PC_ lookup with the UI_ name
  at the outermost call; `host_is_child` reads a creature with no +0xE0 data as not a child
  (selecting a spell category or an expression crashed in GetPrioritisedAbilityDefs). A
  broader version that cleared the byte for the whole handler stopped Back from opening the
  inventory (the inventory "opens" once at load, so the host's Xbox-inventory flag is always
  set). Harness: Equipment -> Weapons -> Melee, Equipment -> Clothing open, B closes, the hero
  walks after.
- **Crash in sub-menus after v23 (device log)**: the handler built PC_DESCRIPTION_INDEPENDANT
  (vfunc_14 at 0x42380E / 0x423BD3, gated on the same byte) inside an Xbox screen and libc
  faulted. `host_ui_manager_event` now wraps the whole handler (0x41E6D3) with the byte clear,
  but only while `g_xboxScreensUp` (set when the host opens the Xbox inventory with the live
  GUI on, cleared by `xboxScreensTick` when the live GUI goes off), which keeps Back working.
  Harness: Equipment -> Clothing -> Head -> item -> Wear and back out, no crash; the hero
  walks after closing.
- **Guild Seal on d-pad down (user report: "no longer working")**:
  `CInputProcessQuickAccessItems::ProcessInput` (0x68E510) takes the seal's slot (-2) when the
  press is CHARGE_GUILD_SEAL (78), then asks 0x446000 (manifest: IsEventContextSensitiveItemButton;
  it walks the QUICK_ACCESS_ITEM (55) controls at +0x7C4), which overwrote the slot with 1
  because d-pad down is the Xbox scheme's second 55 record; the charge ran on slot 1 and was
  cancelled at once (harness: charge slot 1, then -1). `host_quick_button` (wrap 0x446000)
  answers no for d-pad down unless the right trigger is held (the favourites), so the press
  charges the seal exactly as G does on the PC (0x68E450 cancels the charge when no held 78
  event is queued). The NPC follow/wait commands (OBJECT_EXPRESSION_FOLLOW_DUMMY /
  _WAIT_DUMMY; global def index = compiled_game_entries index + 1) go to d-pad down when they
  are among the context items (`host::menu::npcCommandPress`, from the joystick hook). The
  context ids with no NPC are GUILD_SEAL_1, BELCH, MIDDLE_FINGER, VICTORY_PUMP.
  Harness: holding d-pad down charges the seal (charge slot -2, the seal icon glows) and the
  charge ends when released.
- **Xbox list rows missing / drawn over titles**: CList's view-area pass (vfunc 185, 0x5379F5)
  shows a row (alpha 255) when both corners of its zoomed box lie inside the list's view box
  and hides it (alpha 0) otherwise; the view box's size is multiplied by GetUIScale at
  0x537D74 / 0x537D92, which were not zoomed, so the box was 1/1.6 of its height: a category
  list opened with one row, and rows scrolled above the box still drew over the screen title.
  Both sites are zoomed now; Equipment shows its four rows with the faded edge rows and nothing
  over the title. `FABLE_XBOX_ZOOM_LOG=1` logs every GetUIScale caller seen in the Xbox screens
  (zoomed or not); `FABLE_XBOX_ZOOM_SITES=3` zooms every caller, `-1` none (diagnostics).
  Still unzoomed and seen: CScrollingViewport (0x53EABC.., mouse hit test; 0x53F1E3..),
  CText::CanScroll (0x54ECEB), CTable::Draw (0x5511A5/C3).
- Clothing in the harness: Head, Hands, Torso, Legs, Feet: item -> Wear and the "No ..." item
  to remove it, the hero model changes each time, no crash.
- **Right-stick hint over the description text (user report)**: the description pane's
  UI_THUMB_STICK_UP_DOWN group (up/down arrows, the stick, its "R") sits at x 480..507 of the
  640x480 design and the description text wraps at about 505, so it covered the ends of the
  lines on any screen size. `host_update_position` moves the group 34 design units right
  (times its render zoom); its children follow from its render position. CComponent::Name is
  at +0x20 in the retail layout (PDB/Ego_r +0x24); +0x1C holds a float in some components and
  a first diagnostic that read it as a pointer crashed. `FABLE_UI_POS=1` logs each named
  component of the Xbox screens once (render position, zoom) and each text (name, value).
  The parts do not follow the group's render position (moving only the group left the
  arrows, stick and "R" in place), so every UI_THUMB_STICK* component moves.
- **Green selection marker floating right of weapon / item / clothing rows (user report)**:
  each row carries UI_SELECTION_ARROW_RIGHT at a fixed offset of the Xbox row template (29
  design units right of the bar's right cap); those rows hold the PC button template
  (PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE, PC_TEXT_SLOT_L/M/R; the PC build has no Xbox one) whose
  bar is shorter. `alignRowMarker` (from host_update_position, for a marker or a PC right cap)
  finds the row through the tree (children at +0xB0/+0xB4; the parent taken from +0xC8/+0xD0
  only when its children include the component: +0xD0 held a float and a first version that
  trusted it crashed at 0x420000B0) and puts the marker 29 units past the PC cap. All reads in
  that path are fault-safe.
- **Description text running on under the footer**: CScrollingViewport's window (0x53F1C0),
  scroll range (ScrollChildren 0x53EDA0), pointer hit test (0x53E9A0) and CText::CanScroll
  multiplied by the unzoomed UI scale (window 1/1.6 of the pane); those 13 GetUIScale sites
  are zoomed now with the others.
- **Overlay FPS cap lowered the frame rate (user report)**: the cap in `Device::Present`
  (gles) restarted its schedule at now + period whenever a frame ended past its deadline, so
  every frame slower than the cap slept one more full period (41 ms Guild frames became 74 ms
  at cap 30; one late frame in an 18 ms menu doubled at cap 60). The game has no limiter of
  its own and alternates cheap and expensive frames (the world ticks at 15 Hz), so the
  expensive ones were the ones penalised; the handheld turns cap 30 on by default. Now a late
  frame never sleeps and keeps the schedule (the next frame can catch up), the schedule
  restarts only a whole period behind, and the sleep is retried on EINTR. The 120-frame
  `gles: frame` line adds the mean frame and `cap C slept S/frame, L late`. A change of the cap
  alone no longer re-runs SetVideoOptions (the engine's device reset, a hitch per press).
  With vsync on the cap and the refresh run on separate clocks: for a 30 lock use vsync
  "half"; 25 and 40 pace evenly only with vsync off.
- **"Head" over every clothing item name (user report)**: CTCInventoryClothing's Xbox screen
  (vfunc 77, 0x5B76D1) gives each body-part entry a label ("Head x 1", ...) on the description
  title line, a child of the entry row created from 0x5B83CF. In the category list only the
  highlighted entry's label shows (state +0x14C = 3, alpha 255). In an item screen the rows
  go to alpha 0 but the first entry's label kept state 3 and full alpha, so "Head x 1" drew
  under every item name. `host_text_draw` draws a label only in state 3 with its row
  (the label's parent, UI_WEAPON_MENU_ENTRY_BRANCH...) above alpha 0; the Equipment list's
  clothing entry above the row is at alpha 0 throughout, so only the row counts. The draw
  still runs with the colour's alpha at 0 (a CText keeps its engine primitive between draws;
  skipping the draw left the last text up, the labels piled up).
- **Row bars of the quick-access menu and the weapon / item / clothing lists (user report:
  "the graphic for the quick access seemed a little glitched")**: CreateMenuEntryComponent
  (0x428494) gives each row one shared, name-cached bar table (PC_BUTTON_BASTARD: L, a
  stretched M, R with the orb) that CTable::Draw (0x550DC0) draws per row: the pieces' offsets
  are multiplied by GetUIScale at 0x5511A5 / 0x5511C3, not zoomed, while their sizes come from
  the zoomed render zoom, so the right cap sat at 1/1.6 of its offset with the middle running
  on past the orb. Both sites are zoomed now. `FABLE_XBOX_BAR=1` makes the bar's cache miss
  (CreateComponent from 0x4284C0 / 0x428505) in an Xbox screen build the Xbox bar
  UI_BUTTON_BASTARD (width 180, still in game.bin) that the rows and their markers were laid
  out for; off by default, it faulted entering Clothing (see v24 below).
- B does not close the quick-access menu on the Xbox either (user check): no change.
- **v24** = the re-lift with the CPU round's lifter (155 of 161 lifted files change:
  small displacements folded into the host addressing mode, `rep stos` fills), the trap-range
  dispatch, the GLES renderer round (2026-10-15-gles-renderer-perf.md), the frame rate cap
  fix, and the Xbox screen fixes above (clothing labels, right-stick hint, row markers and
  bars, viewport zoom, pause-menu save texts). Harness: with fps_cap=40 the menus hold 25.0 ms
  (22.5 ms slept per frame, 0 late) and gameplay frames slower than the cap sleep nothing
  (mean = the frame, 120 of 120 late). The Xbox bar swap (`FABLE_XBOX_BAR=1`) faulted entering
  Clothing (CComponent::FindChild 0x530780 called through "STAR" text) and is off by default.
- **Rows onto the picture ring (user report: "the bars in general need to be moved left")**:
  at 720x480 the inventory rows run x 64..400 and the ring's frame starts at about x 355 (the
  640x480 design stretched 1.125). The Xbox rows' own bar (UI_BUTTON) ends at about 400 with
  the green marker on its orb; the CreateMenuEntryComponent rows' PC bar (210 wide) ended at
  423 with the orb past the marker. In CTable::Draw's piece loop (esi the table, edi the
  piece) the GetUIScale call at 0x5511A5 now also narrows PC_BUTTON_BASTARD: offsets times
  `FABLE_PC_BAR_SCALE` (0.94) and the table's render zoom x (+0x7C, recomputed at 0x550E58 per
  draw, so scaled once per draw). Every outermost CList (vt 0x12462E4, no list above it) of
  the inventory screens moves left by `FABLE_XBOX_LIST_SHIFT` design units (20); rows, bars,
  texts and markers follow its render position.
- **A dim line under "A - Select / B - Back" (description pane)**: the pane's only clip is the
  2D viewport's alpha mask (graphic 4818, UI_VIEWPORT_TEXT_AREA_SPRITE; CViewport draw
  0x54E6F0, CEngineInternalPrimitive2DViewport::Render 0xBE1E70), whose opaque band is the
  Window (texels 43..584 x 291..402). The retail mask has a faint smudge below it (rows
  461..479, alpha 17..51), and the third line past the window showed through at y~465.
  `host_viewport_draw` records each CScrollingViewport's window (anchor + Window x
  RelativeRenderZoom), `host_viewport2d_render` scissors the primitive's render to it
  (`host::d3d9ClipPush/Pop` on the native device; the GLES device implements the scissor).
  `FABLE_VIEWPORT_CLIP=0` off, `FABLE_VIEWPORT_CLIP_LOG=1` logs the windows. New lift wraps:
  `--wrap 0x54E6F0=host_viewport_draw --wrap 0xBE1E70=host_viewport2d_render`.
- **Device crash charging the Guild Seal fully (user log, v24)**: the full charge opens the
  teleport map (PC_WORLD_MAP, PC_TELEPORT_LIST; CTCInventoryMap_BuildMapMenu 0x60EEB0).
  `FindChild("PC_TITLE_WORLD_MAP")` fails there, and `host_find_child` retried it as
  UI_TITLE_WORLD_MAP (also absent), then returned a copy of eax taken after its
  CCharString constructor call: `guestCallThis` leaves the guest result in `c->eax`, so that
  was the retry string's own (freed) buffer. BuildMapMenu attached it to PC_UI_MAP_FRAME
  (vfunc +0x1F0 at 0x60FA6B) and the game later crashed releasing its "name" (reading 0xB42,
  ~CCharString 0x99EAE0). A failed retry now returns the original null. The Xbox bar swap's
  destructor call in `host_create_component` had the same fault (the created bar replaced by
  the destructor's eax: the "STAR" crash entering Clothing); it now keeps the result.
  Before the crash the log shows the charge slot alternating -2 / -1 while d-pad down was
  held (open).
- **Perf log line (v25)**: every 120 frames `gles: perf N frames: p50/p90/p99/max ms, frames
  over 33 / 50 ms; per frame: draws, game-thread GL ms (swap), waited (for the render thread),
  render thread ms, QueryInterface calls`. The v24 device profile put 4.1% of samples in
  `runtime!com::QueryInterfac` (a stripped runtime: possibly the nearest symbol of the trap
  path); the per-frame count tells whether the guest really calls it that often.

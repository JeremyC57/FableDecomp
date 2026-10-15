# Recompiler handoff: where the Windows x64 build stopped (2026-10-03, session 2)

Goal: Fable TLC running as a native x64 Windows exe (then Android). The exe needs the
user's original game folder; no retail bytes or generated code are in git.

## Where things stand

- **The recompiled game is playable into the world** (tested under Wine 9 + Xvfb, with
  software rendering at about 6 fps there):
  - intro movies with sound;
  - the frontend and menus;
  - new game: the opening cutscene, then control of the hero in Oakvale;
  - loading a save; autosave.
  - On the user's Windows PC the menus already worked before the in-game fixes below.
- **Fixed this session**, in the order they blocked:
  1. **Lifter: functions started at the wrong block.** Blocks are emitted in address
     order, so a backward jump below the entry made the C function begin there. This
     caused the old "Buffer overrun detected!" blocker.
  2. **ddraw.dll** for ConfigDetect, with video memory clamped to 1 GiB.
  3. **dsound.dll** device enumeration; the real TSC (the CPU speed read 0 MHz).
  4. **DirectSound** proxies, created through `CoCreateInstance`, which now builds real
     64-bit COM objects behind generated proxies.
  5. **Movies:** a host-implemented DirectShow graph (`video.cpp`).
  6. **Thunk argument size:** 8-byte integers (`SIZE_T`, `WPARAM`...) are 4 guest bytes.
     `VirtualFree` popped 4 bytes too many, corrupting registers when starting a game.
  7. **Jump tables with null holes** lost their targets in the lifter.
  8. **Coroutines:** the game's coroutine switch (0x9D8650, used by world loading and
     every quest script) runs on host fibers, via `--hook`.
  9. **C++ exceptions:** landing pads in the 829 EH functions, plus a host
     `_CxxThrowException` (`eh.cpp`).
  10. **Missed function entries:** code right after an indirect `jmp`/`ret` now counts as
      a possible entry (vtable targets).
  11. **Refresh rate 0** (Wine on Xvfb, and some drivers) now reads as 60 Hz. The game
      divides by it for its frame period; an infinite period meant the world ticked
      unthrottled and it never rendered in game.
- **Debug aids:**
  - `CRASH:` log lines give the likely guest call chain for any fault.
  - `FABLE_RECOMP_SAMPLE=<ms>` samples the main thread's guest stack.
  - The lifter's `--trace ADDR` logs calls to chosen functions.
  - `ghidra_out/coverage.tsv` names retail addresses.
  - `Present` logs the frame rate.
- **Session 2b, after the first real-Windows test:** menus ran at 40 fps and the game at
  60 fps on the user's PC.
  - **Settings now persist.** The game writes its video settings to HKLM, which a
    non-admin 64-bit process may not do. `misc.cpp` virtualizes writes to
    `HKCU\Software\Classes\VirtualStore\MACHINE\SOFTWARE\WOW6432Node`, as Windows
    does for the retail exe.
  - **Windowed mode** comes from the host (the game has none). It is set by
    `HKCU\Software\FableRecomp\Windowed`, `--windowed`, `--fullscreen` or Alt+Enter.
    Alt+Enter takes effect through the game's own device-reset path.
  - **UI scale** (`ui_scale.cpp`, `--ui-scale=N` or `HKCU\Software\FableRecomp\UIScale`,
    0 = automatic, else 100-300 %). The game's 2D layer is pixel-sized, so the HUD is tiny at
    1080p and up. The lifter hooks the render-target-size getter (0x9BEDC0) so the game sees
    width/s x height/s; automatic picks s so that it sees about 720 lines. `d3d9.cpp` scales
    back-buffer viewports, scissor/clear/StretchRect rectangles and pre-transformed UP
    vertices by s, so 3D still renders at full resolution. Only the new-profile
    default-resolution code sees the real size.
  - **Xbox chase camera** (`camera.cpp`, `XBOX_CAMERA` in the controller ini): the region's
    camera set def is patched to name the Xbox camera managers (CameraManagerMain/Combat,
    retail def +0x3C/+0x40) instead of the PC pair (+0x44/+0x48), so the right stick looks
    around without snapping back and the camera follows the hero's movement.
  - **Controllers** are supported through XInput (`controller.cpp`): buttons and sticks
    become the game's DirectInput keyboard and mouse input. The mapping is in
    `<game>\FableRecomp_controller.ini`, which defaults to an Xbox-like layout.
  - Native Xbox pad support would be better: the PC exe still has
    `CInputTypeXboxPad*`, data-driven control schemes and `CJoystickDX`. It is not
    reverse-engineered yet.
- **Direct3D 9 on OpenGL ES** (`posix/gles/`, `FABLE_RENDERER=gles`; RG34XX SP only, where
    there is no Vulkan): a census of the game's Direct3D use (`census.cpp`), a vs_1_1/ps_1_x
    → GLSL ES translator (`dx9shader.cpp`) and the device (`gl_renderer.cpp`). Renders
    into an FBO in Direct3D row order; the vertex epilogue negates clip y (so cull winding
    swaps: D3DCULL_CCW → GL front CCW). Verified through gameplay under Mesa llvmpipe.
    `FABLE_GLES_TRACE=N` / `FABLE_GLES_DUMP=<dir>` / `FABLE_GLES_DRAWDUMP=<frame>|<file>|purple[:min]`
    for diagnosis. Runs on the RG34XX SP's stock OS through EGL on /dev/fb0 (`egl_fbdev.cpp`,
    SDL dummy video). The vertex epilogue applies the D3D9 half-pixel shift (`uHalfPixel`).
  - **RG34XX SP performance**: device log had gameplay at 7-8 fps (~1300 draws) and menus at
    60; the renderer now tracks dirty state per draw, never waits on queries, orphans dynamic
    buffers, discards depth per frame; lifted code at -O2 for the A53 (`FABLE_GEN_OPT`,
    build dir `build/linux-a64-o2`); `Fable.sh` sets performance governors. The overlay's
    statistics line gives frame vs in-GL ms: the next device log decides whether the game's
    own CPU time (lifted code) or the renderer is the remaining cost.
  - **Handheld host overlay** (`posix/overlay/`, `FABLE_OVERLAY=1`): guide button menu
    (graphics page, volume, FPS, stats, log level, vsync, screenshot, exit); `volkeys.cpp`
    volume rocker; `--pad-probe` and `rg34xx/gamecontrollerdb.txt` for the stock pad;
    `HOLD_TARGET`.
  - **Host-owned graphics settings** (`win/gfx_options.cpp`, `FABLE_HOST_GFX=1`): the
    game's Video Options entry is inert (`menu_nav.cpp`); the overlay page / Android
    launcher set the game's own Video Options (resolution scale, refresh rate, MSAA,
    aspect ratio, landscape / mesh / texture / shadow / effects detail, brightness, vsync)
    plus draw distance %, glow, water reflection and weather, saved in `<data>/graphics.txt`
    (`FABLE_GFX_DEFAULTS` seeds a first run). They are rewritten into the options at
    `CEngine::SetVideoOptions` (0xB24690, wrapped) and into the four
    `CEngineVideoOptionsDef`s (retail layout = Ego_r minus 4 bytes).
  - **Device profiles (v9, v10 logs)**: indirect calls were ~9 % (binary search), then
    3.3 % (hash): now a per-site inline cache emitted by the lifter (`RECOMP_ICALL`,
    `recomp_fns[]`). The D3D COM call layer took an SRW lock per call: lock-free trap
    table. Vsync off by default on the handheld (`FABLE_VSYNC`, menu: off/on/half).
    Guest time is flat across the engine (no single hot function); GL time is the Mali
    driver's per-draw cost (~20-30 ms at 500-1000 draws) plus DXT decoded to RGBA8 on the
    CPU. **30 fps round (v14)**: `_ftol2` hooked on the host (`host_ftol2`, 2004 call
    sites); the renderer caches one vertex array object per layout (declaration, shader,
    program, buffers, offsets) and shadows the GL state it last issued, so a draw costs a
    VAO bind plus the states that differ instead of ~40 calls. Buffer uploads go through
    GL_COPY_WRITE_BUFFER (binding an index buffer to the element target would replace the
    bound VAO's element buffer: that was the white-terrain regression). Diagnostics
    `FABLE_GLES_NOVAO=1` / `FABLE_GLES_NOSHADOW=1`. ThinLTO for the lifted code
    (`FABLE_GEN_LTO`) measured no gain and stays off. Next levers: ETC2/ASTC transcode
    cache for textures, fewer draws (the game's own LOD/draw distance), the x87 emulation
    (`fprec` per op), the GUI component update cost in menus (~22 ms CPU).
  - **v15/v16**: the renderer's GL calls are replayed by a render thread (`gles/gl_queue.*`,
    wrappers generated by `gen_gl_queue.py`; `FABLE_GLES_THREAD=0` disables it), so the
    Mali driver's per-draw CPU cost overlaps the game's frame; the lifter emits
    `Ctx* restrict c` (the game thread's frame share halved on the harness) and inline
    caches on every dispatch site; host-to-guest calls cache their lookup.
  - **GLES renderer round (2026-10-15, not yet on the device)**: uniforms per program only
    when changed (constant registers versioned), sampler parameters shadowed, program builds
    in one render-thread round trip, whole-target clears unscissored, alpha test as a shader
    variant (no `discard` otherwise), no periodic readback (`FABLE_GLES_PROBE=1`), a stale
    texture binding after mid-frame uploads fixed, 16-bit DXT storage at texture detail Low
    (`FABLE_GLES_TEX16`); the frame log counts buffer uploads. See the 2026-10-15 journal.
  - **CPU round (2026-10-15, needs a re-lift):** memory operands with a register and a
    displacement under 64 KiB are emitted as `rd32p(MA(ecx, 60))` (displacement in the host
    addressing mode: -6.3% A53 instructions on a 2745-function sample; `--no-fold` gives the
    old output byte for byte); `rep stos` with DF clear is a memset / vectorised fill; trap
    (import/COM) calls go to the host hook before any table lookup (`recomp_set_host_range`);
    `recomp_fns` is hidden (no GOT load per inline-cache hit). Not done yet:
    static x87 TOP tracking in the lifter (4-6% of lifted A53 instructions load/store TOP
    or index the emulated stack), freeing the D3D lock staging copies of load-once resources.
- **Next:**
  - Real-Windows testing of gameplay (input, performance, saving, area transitions,
    combat).
  - Audio and video on a machine with real devices.
  - SEH `__try` frames are passed over by the C++ exception dispatch.

## Headless test loop (Linux)

```sh
# Wine 9 (apt wine64), Xvfb, xdotool, imagemagick, llvm-mingw, libzydis-dev
# Kill stray wineservers first. A `wine reg ...` run outside the X server starts a desktop
# with no display driver, and the game then fails to create windows or a GL context.
wine reg delete 'HKCU\Software\Microsoft\Microsoft Games\Fable TLC' /v GFX_RESET /f; wineserver -k; wineserver -w
Xvfb :77 -screen 0 1280x1024x24 &  DISPLAY=:77 FABLE_RECOMP_LOG=1 FABLE_RECOMP_HEADLESS=1 wine FableRecomp.exe --game '<game dir>'
xwd -root -silent | convert xwd:- shot.png      # screenshot
```

- `GFX_RESET=1` is left behind after a crash, and on the next start ConfigDetect asks
  about safe mode.
- Fatal or warning dialogs drawn by ConfigDetect show up in the log as
  `SetWindowTextA`/`SetDlgItemTextA`.

## Files in `win/`

| File | Purpose |
|---|---|
| `host.hpp` | Core API: guest memory helpers, traps/import registry, `Arg<T>` converters, `stdThunk`/`cdeclThunk`, `FWD_STD` / `IMPORT` / `IMPORTN` macros, guest threads, `guestCall` |
| `core.cpp` | Logging, low-memory allocator (guest heap: boundary tags over 64 MB segments below 2 GB), traps, import table, guest threads/TEB/PEB, host→guest calls, module handles |
| `loader.cpp` | Maps PE images at their base, binds imports to traps, loads recompiled DLLs (calls DllMain), export lookup |
| `main.cpp` | Finds the game folder (`--game`, `FABLE_DIR`, the exe's own folder, the remembered `HKCU\Software\FableRecomp\GameDir`, the default Steam library, then a folder picker), verifies the Fable.exe SHA-256, maps it, runs entry 0x401067 on a 256 MB-stack thread |
| `crt.cpp` | msvcr71/msvcp71 (startup, heap, strings, printf family, math incl. `_CI*`, a VC7.1-exact qsort, `std::exception`, VC7.1 `std::string`) |
| `format.cpp/.hpp` | printf/scanf over guest varargs with MSVC semantics |
| `kernel32.cpp` | kernel32: critical sections (host pointer kept in the guest CS), async I/O (`ReadFileEx`), resources, `LoadLibrary`/`GetProcAddress`, TLS, etc. |
| `user32.cpp` | Window classes/procs (guest WndProc called through a host proc), MSG and `lParam` struct conversion, dialogs |
| `misc.cpp` | gdi32, advapi32 (registry in the 32-bit view), winmm, version, shell32, ole32/oleaut32, imm32, wsock32 (network reported as unavailable) |
| `com.hpp/.cpp`, `com_vtables.inc`, `gen_com.py` | COM proxies. Guest vtables of traps for D3D9, DInput8, DirectDraw 7 and DirectSound are generated from mingw headers. `OVERRIDES` in `gen_com.py` lists the hand-written methods. Host-implemented COM classes register with `HostClassReg`; `CoCreateInstance` (misc.cpp) tries those, then the real 64-bit class behind a proxy |
| `d3d9.cpp` | Direct3DCreate9, CreateDevice/Reset (converts D3DPRESENT_PARAMETERS, applies the 24-bit FPU mode), all Lock/Unlock through guest staging buffers. D3DX math is native; D3DX texture helpers load any x64 `d3dx9_XX.dll` at run time |
| `dinput.cpp` | DirectInput8Create, SetDataFormat, GetDeviceData, EnumDevices, EnumObjects |
| `ddraw.cpp` | DirectDrawEnumerateExA, DirectDrawCreateEx; `GetAvailableVidMem` clamped to 1 GiB (ConfigDetect's 32-bit rounding wraps larger values to 0) |
| `dsound.cpp` | DirectSound: CreateSoundBuffer (DSBUFFERDESC), Lock/Unlock through guest staging, SetNotificationPositions; `GetDeviceID`; `DllGetClassObject(CLSID_DirectSoundPrivate)` device enumeration |
| `video.cpp` | Host-implemented DirectShow filter graph for the movies: Media Foundation decode, waveOut audio, frames fed to the game's own `CBaseVideoRenderer` through its x86 interfaces |
| `coroutine.cpp` | `host_coswitch`: the game's stack-switching coroutine routine (0x9D8650, hooked by the lifter) run on host fibers |
| `ui_scale.cpp` | UI scale: `host_rtdims` (0x9BEDC0, hooked by the lifter) reports the screen size divided by the scale; the D3D side is in `d3d9.cpp` |
| `camera.cpp` | `host_camera_set` (wraps `CGameCameraManager::SetCameraManagerSetDef` 0x69AC90): Xbox camera managers on the native pad |
| `controller.cpp/.hpp` | XInput controller -> synthetic DirectInput keyboard/mouse events and state; mapping file `FableRecomp_controller.ini` |
| `eh.cpp` | MSVC C++ exceptions: `_CxxThrowException` walks the guest fs:[0] chain, matches catch types, runs unwind and catch funclets, and resumes through the lifter's landing pads (`recomp_resume_at`) |
| `gamedlls.cpp` | Stand-ins for 32-bit game-folder DLLs (`eula.dll!EBUEula` returns accepted) |
| `CMakeLists.txt`, `cmake/llvm-mingw-x64.cmake` | Cross build. `FABLE_GEN_DIR` = lifter output. `FABLE_GUEST_DLLS="cfgdetect\|ConfigDetect.dll\|<dir>"` |

## How to rebuild

The original session used Linux with llvm-mingw, Wine 11.18 and Xvfb. On Windows you can
use llvm-mingw directly, or clang-cl after small tweaks.

```sh
# Lifter (needs Zydis): build rebuild/recomp, then
fable_recomp Fable.exe rebuild/manifest/functions.tsv gen_win/ --hook 0x9D8650=host_coswitch \
    --hook 0xAB6E40=host_joystick_update --hook 0x4088E0=host_primary_inputs --hook 0x9BEDC0=host_rtdims \
    --wrap 0x55CB10=host_ui_event --wrap 0x58F649=host_open_inventory --wrap 0x41CF47=host_ui_scale \
    --wrap 0x69AC90=host_camera_set --wrap 0x533D30=host_component_size --wrap 0xB24690=host_video_options \
    --wrap 0x47E196=host_save_game --wrap 0x54EF00=host_text_draw \
    --wrap 0x52FFD0=host_update_position --hook 0xBFEA70=host_ftol2 \
    --wrap 0x445B80=host_ztarget_toggle --wrap 0x591E5E=host_inventory_screen --wrap 0x581FA4=host_csi_graphic --wrap 0x581E33=host_quick_access_item --wrap 0x5840A6=host_use_quick_item --wrap 0x583A92=host_use_context_item --wrap 0x41DB1D=host_create_component --wrap 0x530780=host_find_child --wrap 0x661F70=host_is_child --wrap 0x41E6D3=host_ui_manager_event --wrap 0x446000=host_quick_button --wrap 0x54E6F0=host_viewport_draw --wrap 0xBE1E70=host_viewport2d_render
# coroutine switch -> host fibers; native Xbox pad (controller.cpp); UI scale (ui_scale.cpp);
# host graphics settings (gfx_options.cpp, --wrap 0xB24690=host_video_options); the Xbox
# pause menu's Hero Save (menu_nav.cpp, host_save_game: a stand-in for the PC menu object);
# front-end labels centred in their bars (menu_nav.cpp, host_text_draw); the Xbox screens'
# absolute components zoomed with the rest (host_update_position);
# pad menu navigation and the Xbox in-game menus (menu_nav.cpp); the Xbox chase camera
# (camera.cpp). --wrap keeps the original body as F_<addr>_orig
fable_recomp ConfigDetect.dll - gen_cfg/ --prefix cfgdetect
python3 rebuild/recomp/win/gen_com.py <llvm-mingw>/generic-w64-mingw32/include rebuild/recomp/win/com_vtables.inc
cmake -S rebuild/recomp/win -B build/win -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=rebuild/recomp/win/cmake/llvm-mingw-x64.cmake -DLLVM_MINGW=<llvm-mingw> \
  -DFABLE_GEN_DIR=gen_win "-DFABLE_GUEST_DLLS=cfgdetect|ConfigDetect.dll|gen_cfg" -DCMAKE_BUILD_TYPE=Release
cmake --build build/win     # about 5 min; the generated C is ~250 MB
FableRecomp.exe --game "D:\SteamLibrary\steamapps\common\Fable The Lost Chapters"
```

v21b (RG34XX): Xbox d-pad layout and favourites (`host_use_quick_item` / `host_use_context_item`),
render-target locks read back from GL (the adult hero's face), a PC-layout test in `host_ui_scale`
that does not fire yet (see the 2026-10-12 journal, quest screens).
v20 (RG34XX): hold-to-target through `host_ztarget_toggle`; Will mode and d-pad items fixed by
replacing the profile's keyboard slot records (actions 54/55/56) with the Xbox scheme's in
`mergedBindings` (slot lists are positional, see the 2026-10-12 journal). Open: Guild river water,
Guild guards in childhood, quest screens (PC layout), Android launch crash (deferred).

The log goes to `<game>\FableRecomp.log`.

Environment variables:
- `FABLE_RECOMP_LOG=2` logs every import call.
- `FABLE_RECOMP_HEADLESS=1` turns off the fatal-error MessageBox.

Under Wine you must create `Documents\My Games\Fable` in the prefix.

## Known gaps, in rough order

1. Gameplay beyond Oakvale is untested (see "Next" above).
2. **C++ exceptions and SEH are not implemented.** `_CxxThrowException`, `__CxxFrameHandler`
   and `_except_handler3` all end in a fatal error.
   - The game throws `CBBBFileException` when a file is missing, so the full data folder
     is needed. It will also need real unwinding: a longjmp-style resume into the catch
     funclet's continuation, which needs lifter support.
3. Audio:
   - dsound.dll is only partly provided (GetDeviceID and the private device enumeration).
     OpenAL32.dll and wrap_oal.dll are missing, and `CoCreateInstance(CLSID_DirectSound)`
     fails.
   - The game needs a DirectSound proxy (COM, like D3D9) or an OpenAL proxy.
   - WMV video (DirectShow via `CoCreateInstance`) is not supported either.
4. `mgspid.dll` / `PidGen.dll` (product ID) are reported as missing. This has not been
   checked for side effects.
5. The registry key `HKLM\Software\Microsoft\Microsoft Games\Fable\1.0` is missing under
   Wine. Steam creates it on a real install.
6. waveOut/mixer report no devices.
7. Testing: session 2 ran the full Steam install under Wine (2.8 GB, never committed).
   Real testing on the Windows PC with the full install is still the next check.
8. Android: built (arm64 APK, `posix/android/`, see its README) but not yet run on a
   device. The ARM64 code paths are checked on Linux under qemu-aarch64.

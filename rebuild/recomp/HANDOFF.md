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
    100-300 %). The game's 2D layer is pixel-sized, so the HUD is tiny at 1080p and up.
    The lifter hooks the render-target-size getter (0x9BEDC0) so the game sees
    width/s x height/s. `d3d9.cpp` scales back-buffer viewports, scissor/clear/StretchRect
    rectangles and pre-transformed UP vertices by s, so 3D still renders at full
    resolution. Only the new-profile default-resolution code sees the real size.
  - **Controllers** are supported through XInput (`controller.cpp`): buttons and sticks
    become the game's DirectInput keyboard and mouse input. The mapping is in
    `<game>\FableRecomp_controller.ini`, which defaults to an Xbox-like layout.
  - Native Xbox pad support would be better: the PC exe still has
    `CInputTypeXboxPad*`, data-driven control schemes and `CJoystickDX`. It is not
    reverse-engineered yet.
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
| `controller.cpp/.hpp` | XInput controller -> synthetic DirectInput keyboard/mouse events and state; mapping file `FableRecomp_controller.ini` |
| `eh.cpp` | MSVC C++ exceptions: `_CxxThrowException` walks the guest fs:[0] chain, matches catch types, runs unwind and catch funclets, and resumes through the lifter's landing pads (`recomp_resume_at`) |
| `gamedlls.cpp` | Stand-ins for 32-bit game-folder DLLs (`eula.dll!EBUEula` returns accepted) |
| `CMakeLists.txt`, `cmake/llvm-mingw-x64.cmake` | Cross build. `FABLE_GEN_DIR` = lifter output. `FABLE_GUEST_DLLS="cfgdetect\|ConfigDetect.dll\|<dir>"` |

## How to rebuild

The original session used Linux with llvm-mingw, Wine 11.18 and Xvfb. On Windows you can
use llvm-mingw directly, or clang-cl after small tweaks.

```sh
# Lifter (needs Zydis): build rebuild/recomp, then
fable_recomp Fable.exe rebuild/manifest/functions.tsv gen_win/ --hook 0x9D8650=host_coswitch --hook 0x9BEDC0=host_rtdims   # coroutine switch -> host fibers; UI scale
fable_recomp ConfigDetect.dll - gen_cfg/ --prefix cfgdetect
python3 rebuild/recomp/win/gen_com.py <llvm-mingw>/generic-w64-mingw32/include rebuild/recomp/win/com_vtables.inc
cmake -S rebuild/recomp/win -B build/win -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=rebuild/recomp/win/cmake/llvm-mingw-x64.cmake -DLLVM_MINGW=<llvm-mingw> \
  -DFABLE_GEN_DIR=gen_win "-DFABLE_GUEST_DLLS=cfgdetect|ConfigDetect.dll|gen_cfg" -DCMAKE_BUILD_TYPE=Release
cmake --build build/win     # about 5 min; the generated C is ~250 MB
FableRecomp.exe --game "D:\SteamLibrary\steamapps\common\Fable The Lost Chapters"
```

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

# Recompiler handoff: where the Windows x64 build stopped (2026-10-03, session 2)

Goal: Fable TLC running as a native x64 Windows exe (then Android). The exe needs the
user's original game folder; no retail bytes or generated code are in git.

## Where things stand

- **The recompiled game reaches the frontend and renders it** (Wine 9, Xvfb, llvm-mingw):
  - ConfigDetect passes.
  - D3D9 CreateDevice/Reset succeed.
  - `frontend.bin`, `frontend.big` and the shaders load.
  - The animated frontend background draws, with the game's "error during video
    playback" panel over it.
- **Fixed this session:**
  - **Lifter: functions started at the wrong block.** Blocks are emitted in address
    order. When a backward jump pulls in code below the entry, the C function began
    executing that code. This made every `__security_check_cookie` run its failure path,
    which caused the old "Buffer overrun detected!" blocker. 270 functions in Fable.exe and
    142 in ConfigDetect were affected. Any earlier whole-game differential results predate
    this fix.
  - **ddraw.dll:** `DirectDrawEnumerateExA`, `DirectDrawCreateEx` and an `IDirectDraw7`
    proxy, which ConfigDetect uses to size video memory. `GetAvailableVidMem` is clamped to
    1 GiB, because ConfigDetect's 64 MB round-up wraps ~4 GB to 0.
  - **dsound.dll:** `GetDeviceID`, plus `DllGetClassObject(CLSID_DirectSoundPrivate)`
    returning a host-implemented `IClassFactory`/`IKsPropertySet` that answers
    `DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A` from the real dsound. Fable.exe uses these
    without checking the results.
  - **Runtime:** the game host now uses the real TSC; it was a fake counter, so
    ConfigDetect measured 0 MHz. Tests keep the deterministic counter.
- **Current blockers:**
  1. **Intro video.** `CoCreateInstance(CLSID_FilterGraph)` returns 0x80040154 (class not
     registered), and the game shows an in-engine "unable to play video" panel.
     - It needs DirectShow proxies: IGraphBuilder, IMediaControl, IMediaEventEx,
       IVideoWindow, IBasicAudio and friends.
     - Note: `OAHWND` is `LONG_PTR`, so it is 8 bytes on x64.
     - Alternatively, skip the movies.
  2. **Input under headless Xvfb.** Buffered DirectInput delivers Return, space, mouse
     motion and button 0 to the game (logged in `GetDeviceData`). Even so, the panel's OK
     does not trigger, and absolute mouse placement is off.
     - Check whether the frontend wants events with matching `dwTimeStamp`/`timeGetTime`,
       or reads `GetDeviceState`.
     - Then check on a real Windows desktop before going deeper.
  3. **Audio.** No audio devices under this Wine. `OpenAL32.dll`/`wrap_oal.dll` are not in
     the Steam folder, and `CoCreateInstance(CLSID_DirectSound)` fails, so it needs a
     DirectSound proxy.
- **Lifter:** adds entries for code only reached through pointers (relocations, data
  dwords, `push offset` immediates), giving 62,888 functions.
  - Options: `-` in place of `functions.tsv`, `--prefix NAME` (to lift a DLL), and
    `--no-refs`.
- **Runtime:** `RECOMP_IDENTITY_MEMORY` (guest address == host address) and
  `recomp_register_table()` for recompiled DLLs.

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
| `eh.cpp` | MSVC C++ exceptions: `_CxxThrowException` walks the guest fs:[0] chain, matches catch types, runs unwind and catch funclets, and resumes through the lifter's landing pads (`recomp_resume_at`) |
| `gamedlls.cpp` | Stand-ins for 32-bit game-folder DLLs (`eula.dll!EBUEula` returns accepted) |
| `CMakeLists.txt`, `cmake/llvm-mingw-x64.cmake` | Cross build. `FABLE_GEN_DIR` = lifter output. `FABLE_GUEST_DLLS="cfgdetect\|ConfigDetect.dll\|<dir>"` |

## How to rebuild

The original session used Linux with llvm-mingw, Wine 11.18 and Xvfb. On Windows you can
use llvm-mingw directly, or clang-cl after small tweaks.

```sh
# Lifter (needs Zydis): build rebuild/recomp, then
fable_recomp Fable.exe rebuild/manifest/functions.tsv gen_win/ --hook 0x9D8650=host_coswitch   # coroutine switch -> host fibers
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

1. DirectShow video and headless input (above).
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
8. Android:
   - Needs a non-identity memory layout, or a guest region reserved below 4 GB in the
     ARM64 process.
   - The Win32/D3D9 layer must be re-targeted to SDL/GLES or Vulkan.

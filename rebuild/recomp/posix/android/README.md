# Android build (arm64-v8a)

The Linux/POSIX build of the recompiled game (`..`), packaged as an Android app. The code is the
same: the lifted game, the shared host (`../../win`) and the POSIX Win32 layer (`../w32`). On top
of that come SDL2's Android activity, DXVK Native on Vulkan, FFmpeg for the movies, and
libadrenotools for custom Vulkan drivers.

The APK holds no game data. The user copies the PC install of Fable: The Lost Chapters (the folder
with `Fable.exe`) to the device and picks it in the launcher.

## Build

```sh
# NDK r27c, SDK (platforms;android-34, build-tools;35.0.0), JDK 17+, cmake, ninja, meson,
# glslang-tools, pkg-config; lifter output for Fable.exe and ConfigDetect.dll (see ../../README.md)
export MINGW_HEADERS=<llvm-mingw>/generic-w64-mingw32/include DXVK_SRC=<dxvk checkout> \
       DXVK2_SRC=<dxvk v2.6.2 checkout>
rebuild/recomp/posix/android/build_android.sh <work dir> <gen_win> <gen_cfg>
# -> <work dir>/FableRecomp.apk
```

`build_android.sh` downloads SDL2 2.30.9, FFmpeg 6.1.2 and libadrenotools into the work
directory and builds them. It applies `dxvk-android.patch` to the DXVK checkout, which:

- loads `libSDL2.so`;
- creates the Vulkan surface from the window's `ANativeWindow`, through DXVK's own loader;
- takes `vkGetInstanceProcAddr` from `DXVK_VK_GET_INSTANCE_PROC_ADDR`, when the host has
  loaded a custom driver.

All libraries are linked for 16 KiB pages. The APK is signed with a local key
(`~/.android/fablerecomp.keystore`, created on first use).

## The app

- **Launcher** (`LauncherActivity`) has these settings:
  - **Game folder.** Typed in, or chosen with the system folder picker. A picked folder
    must map to a file path.
  - **Interface scale.** 1-3×, passed as `--ui-scale=N`.
  - **On-screen controls.** Whether they start shown.
  - **Vulkan driver.** The system driver, or a custom driver installed from a `.zip`.
  - On Android 11+, reading the game folder in place needs "All files access". The launcher
    asks for it.
- **Game** (`GameActivity`, an `SDLActivity`):
  - Runs in its own process (`:game`), because the native side cannot start twice in one
    process.
  - Data, saves, the registry file, `FableRecomp.log` and DXVK logs go to
    `Android/data/org.fablerecomp/files/`.
- **Controllers** (Bluetooth or USB) work through SDL's game controller support, as on PC
  (`../w32/xinput.cpp` → `../../win/controller.cpp`). The mapping file is
  `FableRecomp_controller.ini` in the game folder.
- **Touch controls** (`TouchControls`), toggled by the round button at the top:
  - a floating left stick (movement);
  - A/B/X/Y, LB/LT/RB/RT, Back/Start, the d-pad and L3/R3. These are a virtual controller
    merged into pad 0, so the controller mapping applies.
  - Everywhere else is a touchpad. Dragging moves the mouse (camera, menu cursor), a tap
    clicks, and tap-and-hold holds the left button.
  - When the controls are hidden, touches reach the game as an ordinary mouse.
  - The Back button is Escape.
- **Custom Vulkan drivers** (`Drivers`, `android_host.cpp`):
  - A driver package is a `.zip` with the driver `.so` and a `meta.json` naming it in
    `libraryName`. This is the adrenotools / Turnip release format.
  - The package is unpacked into the app's private files directory.
  - libadrenotools then loads the driver past the linker-namespace restriction, and DXVK uses
    it in place of the system `libvulkan.so`.

- **Two DXVK builds.** The APK carries DXVK 3.x (`libdxvk_d3d9.so`) and DXVK 2.6.2
  (`libdxvk_d3d9_v2.so`, `dxvk2-android.patch`).
  - DXVK 3.0 started requiring `shaderInt64`, which Qualcomm's own Adreno driver lacks (seen on
    an Adreno 740).
  - `android_host.cpp` checks the driver for `shaderInt64` and picks 2.6 when it is missing. The
    launcher's "Renderer" setting can force either one.
  - `d3d9.cpp` opens the chosen library at run time (`FABLE_D3D9_LIBRARY`).
- **When neither renderer can start**, the game shows a message pointing to custom drivers
  instead of crashing. The patched DXVK also logs every missing feature, not only the first.

DXVK needs Vulkan 1.3 with the extensions and features it lists for D3D9. Many stock mobile
drivers fall short, which is what custom drivers are for: Mesa Turnip on Adreno.

## Android-specific parts of the host

- **Guest memory.** Guest pointers are host pointers below 4 GiB (identity memory). ART keeps
  its heap and boot image in the low 4 GiB of every app process. On Android the guest window
  therefore runs to 3 GiB and is reserved around ART's mappings, in 64 KiB pieces
  (`../w32/kernel.cpp`).
- **TEB access.** MinGW's ARM64 `NtCurrentTeb`/`GetCurrentFiber` read register x18. That
  register is no TEB on Linux or Android, so `../include/windows.h` swaps in the emulated TEB.
- **Exception landing pads** use `_setjmp` on ARM64, because clang has no
  `__builtin_setjmp` there (`../../runtime/recomp.h`).
- **Stack.** The game runs on a 256 MiB thread, like the Windows build's main thread.
  SDL's Java thread has about 1 MiB.
- **RDTSC** is a 3 GHz counter from the monotonic clock.
- **Logs.** stdout/stderr go to logcat (tag `FableRecomp`) and `FableRecomp.log`.

## Testing without a device

The ARM64 code paths can be exercised on an x86-64 Linux machine. Build the Linux version for
`aarch64-linux-gnu` (clang `--target`, Ubuntu multiarch arm64 SDL2/FFmpeg/Vulkan, DXVK cross-built
with `aarch64-linux-gnu-g++`) and run it under `qemu-aarch64` with lavapipe on Xvfb. That covers
the fibers, landing pads, char signedness and the TEB. It does not cover the app shell, touch input
or real GPU drivers.

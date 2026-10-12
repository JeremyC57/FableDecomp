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
- **GPU driver.** SDL's KMSDRM backend needs the OS's GBM + EGL + GLES (the Mali blob
  with GBM support, as the ports/PortMaster builds for the H700 use). If the OS's SDL2 is
  needed instead of the bundled one (a fbdev-only Mali blob), put it first in the library
  path in `Fable.sh`. `FABLE_RENDERER=dxvk` switches back to DXVK on Vulkan where a Vulkan
  ICD exists. The log (`log.txt`) names the GL driver and version on start.
- **GLES scope.** What the game uses on the PC, per a Direct3D call census: vs_1_1 and
  ps_1_1/1_4 shaders (translated to GLSL ES 3.00), the fixed-function pixel pipeline,
  pre-transformed UI quads, DXT1/3/5 textures (decoded on the CPU; Mali has no S3TC),
  render-target and depth textures, occlusion queries. No state blocks, cube/volume
  textures or multisampling.
- **Speed.** The x86 game runs as lifted C on four A53 cores at 1.5 GHz; expect a fraction
  of the Android frame rate. 720x480 keeps the GPU load low.
- **Memory.** 2 GB is tight; keep other apps closed.
- **Screen.** 720x480 is offered as a resolution; a new profile defaults to it. The UI
  scale is automatic (1x at 480 lines, so the HUD is laid out as on the Xbox).
- **Controls.** The built-in pad is an SDL game controller: the Xbox control scheme with the
  Xbox menus and camera (settings in `game/FableRecomp_controller.ini`).

## Building
```sh
# arm64 packages (dpkg --add-architecture arm64; libsdl2-dev:arm64 libavcodec-dev:arm64 ...)
meson setup build/dxvk-a64 <dxvk 2.x> --cross-file android/linux-aarch64.ini --buildtype release \
  -Denable_dxgi=false -Denable_d3d8=false -Denable_d3d10=false -Denable_d3d11=false -Dnative_sdl2=enabled
ninja -C build/dxvk-a64
cmake -S rebuild/recomp/posix -B build/linux-a64 -G Ninja -DCMAKE_TOOLCHAIN_FILE=rebuild/recomp/posix/rg34xx/linux-a64.cmake \
  -DFABLE_GEN_DIR=gen_win "-DFABLE_GUEST_DLLS=cfgdetect|ConfigDetect.dll|gen_cfg" \
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
Smoke test on x86-64 (qemu-user, Mesa llvmpipe through `lib/fallback`, SDL on X11):
`SDL_VIDEODRIVER=x11 FABLE_RENDERER=gles LIBGL_ALWAYS_SOFTWARE=1 qemu-aarch64 Fable/lib/ld-linux-aarch64.so.1 --library-path Fable/lib:Fable/lib/fallback Fable/FableRecomp --game <game>`.

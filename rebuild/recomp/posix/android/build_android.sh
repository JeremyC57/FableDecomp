#!/bin/bash
# Android (arm64-v8a) build of the recompiled game: dependencies, libmain.so and the APK.
#
#   build_android.sh <work dir> <lifter output (gen_win)> <ConfigDetect lifter output (gen_cfg)>
#
# Needs: patchelf, Android NDK (ANDROID_NDK, default <work>/android-ndk-r27c), SDK (ANDROID_HOME, default
# <work>/sdk, with platforms;android-34 and build-tools;35.0.0), a JDK, cmake, ninja, meson,
# glslang, pkg-config and the MinGW-w64 headers (MINGW_HEADERS). Sources it downloads or
# expects: SDL2, FFmpeg, libadrenotools, a DXVK checkout (DXVK_SRC, tested at d30be2ba) for
# dxvk-android.patch, and DXVK 2.6.2 (DXVK2_SRC, a checkout of tag v2.6.2 with submodules) for
# dxvk2-android.patch: the fallback for drivers without shaderInt64 (Qualcomm's Adreno driver).
# Output: <work>/FableRecomp.apk
set -euo pipefail
WORK=$(realpath "$1"); GEN=$(realpath "$2"); CFG=$(realpath "$3")
HERE=$(cd "$(dirname "$0")" && pwd)
NDK=${ANDROID_NDK:-$WORK/android-ndk-r27c}
SDK=${ANDROID_HOME:-$WORK/sdk}
BT=$SDK/build-tools/35.0.0
PLATFORM=$SDK/platforms/android-34/android.jar
MINGW_HEADERS=${MINGW_HEADERS:?set MINGW_HEADERS to a MinGW-w64 include directory}
DXVK_SRC=${DXVK_SRC:?set DXVK_SRC to a DXVK checkout}
DXVK2_SRC=${DXVK2_SRC:?set DXVK2_SRC to a DXVK v2.6.2 checkout}
API=26
PREFIX=$WORK/prefix
TC=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin
LDPAGE=-Wl,-z,max-page-size=16384   # 16 KiB page devices
CMAKE_ANDROID=(-G Ninja -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a
               -DANDROID_PLATFORM=$API -DCMAKE_BUILD_TYPE=Release)
cd "$WORK"

SDL_VER=2.30.9 FFMPEG_VER=6.1.2
[ -d SDL2-$SDL_VER ] || curl -sSL https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VER/SDL2-$SDL_VER.tar.gz | tar xz
[ -d ffmpeg-$FFMPEG_VER ] || curl -sSL https://ffmpeg.org/releases/ffmpeg-$FFMPEG_VER.tar.xz | tar xJ
[ -d libadrenotools ] || { git clone -q https://github.com/bylaws/libadrenotools.git && git -C libadrenotools submodule update --init -q; }

# ---- SDL2 -------------------------------------------------------------------------------------
if [ ! -f $PREFIX/lib/libSDL2.so ]; then
  cmake -S SDL2-$SDL_VER -B build-sdl "${CMAKE_ANDROID[@]}" -DCMAKE_INSTALL_PREFIX=$PREFIX -DSDL_STATIC=OFF -DSDL_TEST=OFF \
        -DCMAKE_SHARED_LINKER_FLAGS=$LDPAGE
  ninja -C build-sdl install
fi

# ---- FFmpeg: just what the game's movies need (ASF, WMV1/2/3, WMA) ---------------------------------
if [ ! -f $PREFIX/lib/libavcodec.so ]; then
  (cd ffmpeg-$FFMPEG_VER && ./configure --prefix=$PREFIX --target-os=android --arch=aarch64 --enable-cross-compile \
    --cc=$TC/aarch64-linux-android$API-clang --cxx=$TC/aarch64-linux-android$API-clang++ --ar=$TC/llvm-ar \
    --nm=$TC/llvm-nm --ranlib=$TC/llvm-ranlib --strip=$TC/llvm-strip --extra-ldflags=$LDPAGE \
    --enable-shared --disable-static --disable-programs --disable-doc --disable-everything \
    --enable-demuxer=asf --enable-decoder=wmv1,wmv2,wmv3,wmav1,wmav2,wmapro,vc1 --enable-parser=vc1 \
    --enable-protocol=file --enable-swscale --enable-swresample --disable-avdevice --disable-avfilter \
    --disable-postproc --disable-network --enable-pic && make -j"$(nproc)" && make install)
fi

# ---- libadrenotools (custom Vulkan drivers) --------------------------------------------------------
if [ ! -f build-adreno/libadrenotools.a ]; then
  cmake -S libadrenotools -B build-adreno "${CMAKE_ANDROID[@]}" -DCMAKE_SHARED_LINKER_FLAGS=$LDPAGE
  ninja -C build-adreno
fi

# ---- DXVK Native (Direct3D 9 on Vulkan) ----------------------------------------------------------
cat > android-aarch64.ini <<INI
[binaries]
c = '$TC/aarch64-linux-android$API-clang'
cpp = '$TC/aarch64-linux-android$API-clang++'
ar = '$TC/llvm-ar'
strip = '$TC/llvm-strip'
pkg-config = 'pkg-config'

[properties]
pkg_config_libdir = '$PREFIX/lib/pkgconfig'

[built-in options]
cpp_link_args = ['$LDPAGE', '-static-libstdc++']
c_link_args = ['$LDPAGE']

[host_machine]
system = 'android'
cpu_family = 'aarch64'
cpu = 'armv8-a'
endian = 'little'
INI
build_dxvk() {  # <source> <patch> <build dir>
  if [ ! -f $3/src/d3d9/libdxvk_d3d9.so ]; then
    git -C "$1" apply --check "$HERE/$2" 2>/dev/null && git -C "$1" apply "$HERE/$2"
    meson setup $3 "$1" --cross-file android-aarch64.ini --buildtype release -Denable_dxgi=false \
      -Denable_d3d8=false -Denable_d3d10=false -Denable_d3d11=false -Dnative_sdl2=enabled -Dnative_glfw=disabled \
      -Dnative_sdl3=disabled
  fi
  ninja -C $3
}
build_dxvk "$DXVK_SRC" dxvk-android.patch build-dxvk
build_dxvk "$DXVK2_SRC" dxvk2-android.patch build-dxvk26
# Its own soname, or Android's linker would take it for the 3.x library.
patchelf --set-soname libdxvk_d3d9_v2.so --output build-dxvk26/libdxvk_d3d9_v2.so build-dxvk26/src/d3d9/libdxvk_d3d9.so

# ---- the game: libmain.so ------------------------------------------------------------------------
PKG_CONFIG_LIBDIR=$PREFIX/lib/pkgconfig cmake -S "$HERE/.." -B build-host "${CMAKE_ANDROID[@]}" -DANDROID_STL=c++_static \
  -DFABLE_GEN_DIR="$GEN" "-DFABLE_GUEST_DLLS=cfgdetect|ConfigDetect.dll|$CFG" -DMINGW_HEADERS="$MINGW_HEADERS" \
  -DDXVK_D3D9=$WORK/build-dxvk/src/d3d9/libdxvk_d3d9.so -DADRENOTOOLS_DIR=$WORK/libadrenotools \
  -DADRENOTOOLS_BUILD=$WORK/build-adreno -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH \
  -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH
ninja -C build-host

# ---- APK ---------------------------------------------------------------------------------------
APK=$WORK/apk
rm -rf "$APK" && mkdir -p "$APK/classes" "$APK/res" "$APK/root/lib/arm64-v8a"
SDL_JAVA=SDL2-$SDL_VER/android-project/app/src/main/java
javac -nowarn -source 8 -target 8 -encoding UTF-8 -classpath "$PLATFORM" -d "$APK/classes" \
  $(find "$SDL_JAVA" "$HERE/app/java" -name '*.java') 2>&1 | grep -v "^warning: \[options\]" || true
jar cf "$APK/classes.jar" -C "$APK/classes" .
"$BT/d8" --release --min-api $API --lib "$PLATFORM" --output "$APK/root" "$APK/classes.jar"
"$BT/aapt2" compile --dir "$HERE/app/res" -o "$APK/res.zip"
"$BT/aapt2" link -I "$PLATFORM" --manifest "$HERE/app/AndroidManifest.xml" --min-sdk-version $API \
  --target-sdk-version 34 -o "$APK/base.apk" "$APK/res.zip"
LIBS=(build-host/libmain.so $PREFIX/lib/libSDL2.so build-dxvk/src/d3d9/libdxvk_d3d9.so build-dxvk26/libdxvk_d3d9_v2.so
      $PREFIX/lib/libavcodec.so $PREFIX/lib/libavformat.so $PREFIX/lib/libavutil.so $PREFIX/lib/libswscale.so
      $PREFIX/lib/libswresample.so build-adreno/src/hook/*.so)
for l in "${LIBS[@]}"; do "$TC/llvm-strip" --strip-unneeded -o "$APK/root/lib/arm64-v8a/$(basename "$l")" "$l"; done
cp "$APK/base.apk" "$APK/unsigned.apk"
(cd "$APK/root" && zip -qr "$APK/unsigned.apk" classes.dex lib)
"$BT/zipalign" -f -P 16 4 "$APK/unsigned.apk" "$APK/aligned.apk"
KEY=${FABLE_KEYSTORE:-$HOME/.android/fablerecomp.keystore}
if [ ! -f "$KEY" ]; then
  mkdir -p "$(dirname "$KEY")"
  keytool -genkeypair -keystore "$KEY" -storepass fablerecomp -keypass fablerecomp -alias fablerecomp \
    -keyalg RSA -keysize 2048 -validity 10000 -dname "CN=Fable Recomp" >/dev/null
fi
"$BT/apksigner" sign --ks "$KEY" --ks-pass pass:fablerecomp --key-pass pass:fablerecomp --out "$WORK/FableRecomp.apk" "$APK/aligned.apk"
echo "built $WORK/FableRecomp.apk"

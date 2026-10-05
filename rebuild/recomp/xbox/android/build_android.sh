#!/bin/bash
# Android (arm64-v8a) build of the recompiled Xbox Fable: SDL2, libadrenotools, libmain.so and the APK.
#
#   build_android.sh <work dir> <lifter output (lift.sh ... <gen>)>
#
# Needs: Android NDK (ANDROID_NDK, default <work>/android-ndk-r27c), SDK (ANDROID_HOME, default
# <work>/sdk, with platforms;android-34 and build-tools;35.0.0), a JDK, cmake, ninja, git, curl.
# It downloads SDL2 2.30.9 and libadrenotools into the work directory. No game data goes into the
# APK: the user extracts their own disc and picks the folder in the launcher.
# Output: <work>/FableXbox.apk
set -euo pipefail
WORK=$(realpath "$1"); GEN=$(realpath "$2")
HERE=$(cd "$(dirname "$0")" && pwd)
NDK=${ANDROID_NDK:-$WORK/android-ndk-r27c}
SDK=${ANDROID_HOME:-$WORK/sdk}
BT=$SDK/build-tools/35.0.0
PLATFORM=$SDK/platforms/android-34/android.jar
API=26
PREFIX=$WORK/prefix
TC=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin
LDPAGE=-Wl,-z,max-page-size=16384   # 16 KiB page devices
CMAKE_ANDROID=(-G Ninja -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a
               -DANDROID_PLATFORM=$API -DCMAKE_BUILD_TYPE=Release)
cd "$WORK"

SDL_VER=2.30.9
[ -d SDL2-$SDL_VER ] || curl -sSL https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VER/SDL2-$SDL_VER.tar.gz | tar xz
[ -d libadrenotools ] || { git clone -q https://github.com/bylaws/libadrenotools.git && git -C libadrenotools submodule update --init -q; }

# ---- SDL2 -------------------------------------------------------------------------------------
if [ ! -f $PREFIX/lib/libSDL2.so ]; then
  cmake -S SDL2-$SDL_VER -B build-sdl "${CMAKE_ANDROID[@]}" -DCMAKE_INSTALL_PREFIX=$PREFIX -DSDL_STATIC=OFF -DSDL_TEST=OFF \
        -DCMAKE_SHARED_LINKER_FLAGS=$LDPAGE
  ninja -C build-sdl install
fi

# ---- libadrenotools (custom Vulkan drivers) --------------------------------------------------------
if [ ! -f build-adreno/libadrenotools.a ]; then
  cmake -S libadrenotools -B build-adreno "${CMAKE_ANDROID[@]}" -DCMAKE_SHARED_LINKER_FLAGS=$LDPAGE
  ninja -C build-adreno
fi

# ---- the game: libmain.so ------------------------------------------------------------------------
PKG_CONFIG_LIBDIR=$PREFIX/lib/pkgconfig cmake -S "$HERE/.." -B build-xbox "${CMAKE_ANDROID[@]}" -DANDROID_STL=c++_static \
  -DFABLE_GEN_DIR="$GEN" -DADRENOTOOLS_DIR=$WORK/libadrenotools -DADRENOTOOLS_BUILD=$WORK/build-adreno \
  -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH \
  ${FABLE_GLSLANG_DIR:+-DFABLE_GLSLANG_DIR=$FABLE_GLSLANG_DIR}
ninja -C build-xbox

# ---- APK ---------------------------------------------------------------------------------------
APK=$WORK/apk-xbox
rm -rf "$APK" && mkdir -p "$APK/classes" "$APK/root/lib/arm64-v8a"
SDL_JAVA=SDL2-$SDL_VER/android-project/app/src/main/java
javac -nowarn -source 8 -target 8 -encoding UTF-8 -classpath "$PLATFORM" -d "$APK/classes" \
  $(find "$SDL_JAVA" "$HERE/app/java" -name '*.java') 2>&1 | grep -v "^warning: \[options\]" || true
jar cf "$APK/classes.jar" -C "$APK/classes" .
"$BT/d8" --release --min-api $API --lib "$PLATFORM" --output "$APK/root" "$APK/classes.jar"
"$BT/aapt2" compile --dir "$HERE/app/res" -o "$APK/res.zip"
"$BT/aapt2" link -I "$PLATFORM" --manifest "$HERE/app/AndroidManifest.xml" --min-sdk-version $API \
  --target-sdk-version 34 -o "$APK/base.apk" "$APK/res.zip"
LIBS=(build-xbox/libmain.so $PREFIX/lib/libSDL2.so build-adreno/src/hook/*.so)
for l in "${LIBS[@]}"; do "$TC/llvm-strip" --strip-unneeded -o "$APK/root/lib/arm64-v8a/$(basename "$l")" "$l"; done
cp "$APK/base.apk" "$APK/unsigned.apk"
(cd "$APK/root" && zip -qr "$APK/unsigned.apk" classes.dex lib)
"$BT/zipalign" -f -P 16 4 "$APK/unsigned.apk" "$APK/aligned.apk"
KEY=${FABLE_KEYSTORE:-$HOME/.android/fablexbox.keystore}
if [ ! -f "$KEY" ]; then
  mkdir -p "$(dirname "$KEY")"
  keytool -genkeypair -keystore "$KEY" -storepass fablexbox -keypass fablexbox -alias fablexbox \
    -keyalg RSA -keysize 2048 -validity 10000 -dname "CN=Fable Xbox Recomp" >/dev/null
fi
"$BT/apksigner" sign --ks "$KEY" --ks-pass pass:fablexbox --key-pass pass:fablexbox --out "$WORK/FableXbox.apk" "$APK/aligned.apk"
echo "built $WORK/FableXbox.apk"

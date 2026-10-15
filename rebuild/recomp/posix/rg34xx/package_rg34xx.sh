#!/bin/bash
# Packages the aarch64 Linux build for the Anbernic RG34XX SP (Allwinner H700: 4x Cortex-A53,
# Mali-G31 MP2, 2 GB, 720x480 3:2 screen; the stock Linux OS launches a .sh).
#
#   [FFMPEG_PREFIX=<minimal FFmpeg install>] package_rg34xx.sh <aarch64 build dir> <dxvk build dir> <out dir>
#
# FFMPEG_PREFIX: an FFmpeg built for aarch64-linux with only the movie codecs (see README.md);
# without it Ubuntu's FFmpeg and its 100+ libraries are bundled.
#
# The build dir is the posix CMake build cross-compiled with aarch64-linux-gnu-g++ against
# Ubuntu's arm64 packages (SDL2, FFmpeg, the Vulkan loader; see README.md). The package is
# self-contained: it carries its own glibc, libstdc++, SDL2, DXVK, FFmpeg and the Vulkan
# loader, started through the bundled dynamic loader so the device's own (older) glibc does
# not matter. The device must still provide a Vulkan driver (ICD) for the Mali-G31.
set -e
BUILD=${1:?aarch64 build dir}; DXVK=${2:?dxvk build dir}; OUT=${3:?out dir}
LIBDIR=/usr/lib/aarch64-linux-gnu
HERE=$(cd "$(dirname "$0")" && pwd)
# layout: <out>/Fable.sh next to <out>/Fable/ (the launcher looks for its files in the folder
# of its own name)
rm -rf "$OUT"; mkdir -p "$OUT/Fable/lib" "$OUT/Fable/game" "$OUT/Fable/save"
cp "$HERE/Fable.sh" "$OUT/Fable.sh"; cp "$HERE/Fable-diag.sh" "$OUT/Fable-diag.sh"; chmod +x "$OUT/Fable.sh" "$OUT/Fable-diag.sh"
OUT="$OUT/Fable"
cp "$BUILD/FableRecomp" "$OUT/FableRecomp"
cp "$DXVK/src/d3d9/libdxvk_d3d9.so" "$OUT/lib/libdxvk_d3d9.so.0"
# The profiler's (FABLE_PROFILE=1) names for the host code of the stripped binary: its symbols
# without the ~150k lifted F_ functions (named from the recompiled tables), tagged with the
# build id so a stale file is ignored. Link (mangled) names: the profiler demangles them for
# its log and keeps them for FABLE_PROFILE_DUMP (gen_hot_order.py --counts, an lld ordering file).
NM=${NM:-aarch64-linux-gnu-nm}; command -v "$NM" >/dev/null || NM=nm
{
  echo "# host symbols of FableRecomp for the profiler (package_rg34xx.sh): <link address> <name>"
  aarch64-linux-gnu-readelf -n "$BUILD/FableRecomp" | awk '/Build ID/ {print "# build-id " $3}'
  aarch64-linux-gnu-readelf -SW "$BUILD/FableRecomp" | sed -n 's/.*\] \.plt  *PROGBITS  *\([0-9a-f]*\) .*/\1 [plt]/p'
  "$NM" -n --defined-only "$BUILD/FableRecomp" | awk '$2 ~ /^[tTwW]$/ && $3 !~ /^(F_|\$|\.L)/ { print $1, $3 }'
} > "$OUT/FableRecomp.syms"
aarch64-linux-gnu-strip --strip-unneeded "$OUT/FableRecomp" "$OUT/lib/libdxvk_d3d9.so.0"

if [ -n "$FFMPEG_PREFIX" ]; then  # the minimal FFmpeg first, so the sysroot's is not pulled in
  for l in "$FFMPEG_PREFIX"/lib/lib*.so.*; do [ -L "$l" ] && cp -L "$l" "$OUT/lib/"; done  # the soname links only
fi
# every shared library the binary and DXVK need, recursively, from the arm64 sysroot
needed() { aarch64-linux-gnu-readelf -d "$1" 2>/dev/null | awk '/NEEDED/ {gsub(/[\[\]]/, "", $5); print $5}'; }
queue=("$OUT/FableRecomp" "$OUT/lib/libdxvk_d3d9.so.0")
# libraries SDL2 and the Vulkan loader open at run time rather than link
for extra in libdrm.so.2 libasound.so.2 libvulkan.so.1 libudev.so.1 libdbus-1.so.3; do
  [ -f "$LIBDIR/$extra" ] && { cp -L "$LIBDIR/$extra" "$OUT/lib/"; queue+=("$OUT/lib/$extra"); }
done
# the GPU side (GBM, EGL, GLES) must come from the device's own driver; Mesa's copies are a
# fallback (software rendering at best) for an OS that has none
mkdir -p "$OUT/lib/fallback"
for extra in libgbm.so.1 libEGL.so.1 libGLESv2.so.2; do
  [ -f "$LIBDIR/$extra" ] && { cp -L "$LIBDIR/$extra" "$OUT/lib/fallback/"; queue+=("$OUT/lib/fallback/$extra"); }
done
declare -A seen
while [ ${#queue[@]} -gt 0 ]; do
  f=${queue[0]}; queue=("${queue[@]:1}")
  for n in $(needed "$f"); do
    [ -n "${seen[$n]}" ] && continue; seen[$n]=1
    [ -f "$OUT/lib/$n" ] && { queue+=("$OUT/lib/$n"); continue; }
    [ -f "$OUT/lib/fallback/$n" ] && { queue+=("$OUT/lib/fallback/$n"); continue; }
    case "$n" in libgbm.so.1|libEGL.so.1|libGLESv2.so.2|libGLESv1_CM.so.1|libGL.so.1) continue ;; esac  # device's
    src=$(find "$LIBDIR" "$LIBDIR/pulseaudio" /lib/aarch64-linux-gnu -maxdepth 1 -name "$n" 2>/dev/null | head -1)
    [ -z "$src" ] && { echo "warning: $n not found"; continue; }
    cp -L "$src" "$OUT/lib/$n"; queue+=("$OUT/lib/$n")
  done
done
cp -L "$LIBDIR/ld-linux-aarch64.so.1" "$OUT/lib/"
# vulkan loader + layers config dir is not needed; the ICD comes from the device
chmod +x "$OUT/FableRecomp"
cp "$HERE/README.md" "$OUT/README.txt"
cp "$HERE/gamecontrollerdb.txt" "$OUT/gamecontrollerdb.txt"
cat > "$OUT/FableRecomp_controller.ini.default" <<INI
; copied into game/ on first start if no FableRecomp_controller.ini exists there
NATIVE_PAD = 1
XBOX_MENUS = 1
XBOX_CAMERA = 1
INI
du -sh "$OUT"; ls "$OUT/lib" | wc -l

#!/bin/bash
# Stand up the Linux verification lane: VC7.1-family compiler (DDK 3790.1830, cl.exe 13.10.4035)
# running under 32-bit Wine, laid out as the vc71 tree verify_and_land.py expects.
#
#   bash tools/decomp_pipeline/linux_lane/setup_vc71_wine.sh <lane-dir>
#   source <lane-dir>/env.sh
#   python tools/decomp_pipeline/verify_and_land.py <payload.json> <oracle.tsv> [--land]
#
# Prerequisites: apt-get install wine32:i386 wine binutils (objdump must list pe-i386), git >= 2.25,
# network access to github.com. The toolset stays in <lane-dir>; never commit it.
# Proven 2026-10-13: eight repo MATCH functions re-graded MATCH/PASS through this lane.
set -euo pipefail
LANE=${1:?lane directory}
REPO=$(cd "$(dirname "$0")/../../.." && pwd)
DDK=$LANE/winddk/3790.1830
mkdir -p "$LANE"

if [ ! -f "$DDK/bin/x86/cl.exe" ]; then
  # Blob-less clone, then check out only the compiler, the CRT headers/libs and three Win32 libs.
  # (The whole lib/wnet/i386 tree is hundreds of megabytes; name the files instead.)
  GIT_LFS_SKIP_SMUDGE=1 git clone --depth 1 --filter=blob:none --no-checkout https://github.com/9176324/winddk "$LANE/winddk"
  git -C "$LANE/winddk" sparse-checkout init --no-cone
  git -C "$LANE/winddk" sparse-checkout set '/3790.1830/bin/x86/' '/3790.1830/inc/crt/' '/3790.1830/lib/crt/i386/' \
      '/3790.1830/lib/wnet/i386/kernel32.lib' '/3790.1830/lib/wnet/i386/user32.lib' '/3790.1830/lib/wnet/i386/uuid.lib' \
      '/3790.1830/lib/wnet/i386/bufferoverflowu.lib'
  git -C "$LANE/winddk" checkout HEAD -- .
fi

VC=$LANE/vc71
rm -rf "$VC"; mkdir -p "$VC/lib" "$VC/include"
ln -s "$DDK/bin/x86" "$VC/bin"
for f in "$DDK"/inc/crt/*; do ln -s "$f" "$VC/include/$(basename "$f")"; done
for f in "$DDK"/lib/crt/i386/* "$DDK"/lib/wnet/i386/kernel32.lib "$DDK"/lib/wnet/i386/user32.lib \
         "$DDK"/lib/wnet/i386/uuid.lib "$DDK"/lib/wnet/i386/bufferoverflowu.lib; do
  [ -e "$f" ] && ln -s "$f" "$VC/lib/$(basename "$f")"
done
# The DDK's <cstdio>-style wrappers predate the `namespace std { using ::printf; }` blocks of the
# retail VC7.1 headers, so std::printf etc. would not resolve; overlay thin wrappers that add them.
python3 "$REPO/tools/decomp_pipeline/linux_lane/write_std_shims.py" "$VC/include"

export WINEPREFIX=$LANE/prefix WINEARCH=win32 WINEDEBUG=-all WINEDLLOVERRIDES="mscoree,mshtml=" DISPLAY=
[ -d "$WINEPREFIX/drive_c" ] || wineboot --init >/dev/null 2>&1

cat > "$LANE/env.sh" <<ENV
# source this before running verify_and_land.py through the Linux lane
export WINEPREFIX=$LANE/prefix WINEARCH=win32 WINEDEBUG=-all WINEDLLOVERRIDES="mscoree,mshtml=" DISPLAY=
export FABLE_ROOT=$REPO FABLE_VC71=$VC FABLE_OBJDUMP=objdump FABLE_WINE=wine
export LANDVERIFY_WORK=$LANE/landverify
export LINK=bufferoverflowu.lib   # the DDK CRT was built with /GS and needs __security_check_cookie from here
ENV
mkdir -p "$LANE/landverify"
wine "$VC/bin/cl.exe" 2>&1 | head -1
echo "lane ready: source $LANE/env.sh"

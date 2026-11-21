#!/bin/bash
# fetch_vc10sp1.sh: put the VC10 SP1 x86 compiler (cl 16.00.40219.01, the build Ego_d.pdb / FableWin.pdb record) into $S/wine/vc10sp1/bin.
# It is an EVIDENCE tool only: it compiles candidate texts with Ego_d's own flags so their /Od line sizes can be compared with Ego_d's
# line table exactly. It never builds or grades a row (that is VC7.1 3077). Nothing it fetches is ever committed.
#
# Source: Microsoft's "Visual C++ 2010 SP1 Compiler Update for the Windows SDK 7.1" (KB2519277, download.microsoft.com id 4422).
#   VC-Compiler-KB2519277.exe  126,890,440 bytes
#   sha1   387abb694df7bb3af18b3cc85e2616a70d74cc4e
#   sha256 46497a249dac03f8a6d6b4f4a737546246752ca751ae16c34127a1d784232645
# The package's vc_stdx86.cab holds three builds of each compiler binary (x86 native, x86->amd64, x86->ia64); the file keys below are
# the x86-native ones (cl.exe "_M_IX86", c2.dll from p2\x86). mspdb100.dll / msobj100.dll sit under the old keys FL_mspdb71 / FL_msobj71
# (their version resources say MSPDB100.DLL / MSOBJ100.DLL 10.00.40219.01). The Windows SDK 7.1 ISO is NOT needed.
# Usage: S=<lane scratch> bash fetch_vc10sp1.sh   (DL=<dir with the .exe already downloaded> skips the download)
set -euo pipefail
: "${S:?export S (the lane scratch) first}"
URL="https://download.microsoft.com/download/7/5/0/75040801-126C-4591-BCE4-4CD1FD1499AA/VC-Compiler-KB2519277.exe"
SHA256=46497a249dac03f8a6d6b4f4a737546246752ca751ae16c34127a1d784232645
DL="${DL:-$S/sdk/vc10/dl}"; WORK="$S/sdk/vc10/work.$$"; BIN="$S/wine/vc10sp1/bin"
mkdir -p "$DL" "$WORK" "$BIN/1033"
EXE="$DL/VC-Compiler-KB2519277.exe"
[ -f "$EXE" ] || curl -fsSL -o "$EXE" "$URL"
echo "$SHA256  $EXE" | sha256sum -c -
7z x -y -o"$WORK/x" "$EXE" vc_stdx86.cab > /dev/null
7z x -y -o"$WORK/cab" "$WORK/x/vc_stdx86.cab" > /dev/null
C="$WORK/cab"
pick() { local f; f=$(ls "$C"/$1 2>/dev/null | head -1); [ -n "$f" ] || { echo "missing $1" >&2; exit 1; }; cp "$f" "$BIN/$2"; }
pick 'FL_cl_exe_7279_x86_ln.*'      cl.exe
pick 'FL_c1_dll_3958_x86_ln.*'      c1.dll
pick 'FL_c1xx_dll_67102_67102_x86_ln.*' c1xx.dll
pick 'FL_c2_dll_11003_x86_ln.*'     c2.dll
pick 'FL_clui_dll_95594_95594_x86_enu.*' clui.dll
cp "$BIN/clui.dll" "$BIN/1033/clui.dll"
pick 'FL_mspdb71_dll_2_60032_x86_ln.*' mspdb100.dll
pick 'FL_msobj71_dll_1_60033_x86_ln.*' msobj100.dll
pick 'FL_mspdbcore_dll_92167_92167_x86_ln.*' mspdbcore.dll
pick 'FL_mspdbsrv_exe_92168_92168_x86_ln.*'  mspdbsrv.exe
pick 'F_CENTRAL_msvcr100_x86' msvcr100.dll
pick 'F_CENTRAL_msvcp100_x86' msvcp100.dll
rm -rf "$WORK"
# expected (2026-11-21): cl.exe e4208d3e6a09087ad673cb061559f6f16259218fe9d88f6617fdf33f28c594fb, c2.dll 1de3fbe45436937e9a07c097f5d542bd89ae8ddab006efb3e7d3eebabe2f63c3
sha256sum "$BIN/cl.exe" "$BIN/c2.dll"
echo "installed $BIN; check: wine $BIN/cl.exe  -> 'Version 16.00.40219.01 for 80x86'"

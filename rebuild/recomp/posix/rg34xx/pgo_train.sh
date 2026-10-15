#!/bin/bash
# Profile-guided optimisation for the handheld build, trained on the x86-64 Linux build: the
# lifted C is the same, and clang's instrumentation profile is keyed by function name and source
# structure, not by the target, so a profile of a desktop session optimises the aarch64 build.
#
#   pgo_train.sh <x86-64 build dir> <work dir> -- <command that plays the game>
#
# 1. configures <work dir>/build like <x86-64 build dir> (same lifter output, headers, DXVK)
#    with FABLE_PGO=gen, and builds it (instrumented: ~1.7x the compile time, ~2.4x the code)
# 2. runs the command with FABLE_BUILD=<work dir>/build exported (the harness's BDIR) and
#    LLVM_PROFILE_FILE set; the game writes the counts every 30 s (codelayout.cpp), so a run the
#    harness ends with a signal keeps them. Play what the handheld is slow at: a world scene
#    (Bowerstone, Oakvale) for 10+ minutes, walking, fighting, opening the menus.
# 3. merges the counts into <work dir>/fable.profdata and writes <work dir>/hot_symbols.txt
#    (gen_hot_order.py from the function and block counts)
# Then: copy hot_symbols.txt over rg34xx/hot_symbols.txt and configure the aarch64 build with
# -DFABLE_PGO=<work dir>/fable.profdata. Re-train after the lifter output changes much: changed
# functions lose their profile (silently, they are just compiled as before).
#
# Needs clang's profile runtime (apt install libclang-rt-18-dev), or FABLE_PGO_RUNTIME=<a
# libclang_rt.profile.a of the same clang version> in the environment.
set -e
SRC=$(cd "$(dirname "$0")/.." && pwd)    # rebuild/recomp/posix
LIKE=${1:?x86-64 build dir}; WORK=${2:?work dir}; shift 2
[ "$1" = "--" ] && shift
[ $# -gt 0 ] || { echo "usage: $0 <x86-64 build dir> <work dir> -- <play command>" >&2; exit 1; }
cache() { sed -n "s/^$1:[A-Z]*=//p" "$LIKE/CMakeCache.txt"; }
mkdir -p "$WORK/raw"
WORK=$(cd "$WORK" && pwd)
cmake -S "$SRC" -B "$WORK/build" -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER="$(cache CMAKE_C_COMPILER)" -DCMAKE_CXX_COMPILER="$(cache CMAKE_CXX_COMPILER)" \
  -DFABLE_GEN_DIR="$(cache FABLE_GEN_DIR)" -DFABLE_GUEST_DLLS="$(cache FABLE_GUEST_DLLS)" \
  -DFABLE_GEN_OPT="$(cache FABLE_GEN_OPT)" -DMINGW_HEADERS="$(cache MINGW_HEADERS)" \
  -DDXVK_D3D9="$(cache DXVK_D3D9)" -DFABLE_PGO=gen -DFABLE_PGO_RUNTIME="${FABLE_PGO_RUNTIME:-}"
ninja -C "$WORK/build"
rm -f "$WORK"/raw/*.profraw
FABLE_BUILD="$WORK/build" LLVM_PROFILE_FILE="$WORK/raw/fable-%p.profraw" "$@" || true
ls "$WORK"/raw/*.profraw >/dev/null 2>&1 || { echo "no profile written (did the game run 30 s?)" >&2; exit 1; }
PROFDATA=$(command -v llvm-profdata-18 || command -v llvm-profdata)
"$PROFDATA" merge -o "$WORK/fable.profdata" "$WORK"/raw/*.profraw
"$PROFDATA" show --all-functions --counts "$WORK/fable.profdata" > "$WORK/fable.profdata.txt"
python3 "$SRC/rg34xx/gen_hot_order.py" --profdata "$WORK/fable.profdata.txt" --gen-dir "$(cache FABLE_GEN_DIR)" \
  --expand 0 --callers 0 --budget-kb "${HOT_KB:-6144}" -o "$WORK/hot_symbols.txt"
echo "profile: $WORK/fable.profdata   ordering: $WORK/hot_symbols.txt"

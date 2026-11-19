#!/bin/bash
# Build the native-decomp objects: FullDecomp's byte-matched C++ for a list of retail addresses,
# lowered by fable_lower to host code for the FableRecomp build (see README.md beside this script).
#
#   build_native.sh <addresses.txt> <out dir>
#
# addresses.txt: one hex retail address per line (the landed functions to take; the files that
# define them may carry more landed functions, which come along). Environment:
#   FULLDECOMP  a checkout (or `git archive`) of the FullDecomp branch (default: extract origin/FullDecomp)
#   EXCLUDE     addresses never made native (default: every --hook / --wrap in ../HANDOFF.md)
# Writes <out>/nd_x64.o (Linux x86-64), <out>/nd_a64.o (aarch64), <out>/nd_win.o (Windows x64), <out>/report.tsv (native / lifted and why), and
# <out>/wraps.txt (the --wrap arguments the lift needs). Then: lift with wraps.txt appended,
# configure with -DFABLE_NATIVE_OBJS=<out>/nd_<arch>.o, build.
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../../.." && pwd)
LIST=$(readlink -f "$1"); OUT=$(mkdir -p "$2" && cd "$2" && pwd)
FD=${FULLDECOMP:-$OUT/fulldecomp}
if [ ! -d "$FD/rebuild/src/compiled" ]; then
  mkdir -p "$FD" && git -C "$REPO" archive origin/FullDecomp rebuild/src/compiled rebuild/include rebuild/oracles/reloc_bindings.tsv | tar -x -C "$FD"
fi
LLVMCFG=${LLVMCFG:-llvm-config-18}
if [ ! -x "$OUT/fable_lower" ] || [ "$HERE/fable_lower.cpp" -nt "$OUT/fable_lower" ]; then
  clang++ -O1 -g -std=c++17 "$HERE/fable_lower.cpp" $($LLVMCFG --cxxflags | sed 's/-std=c++[0-9]*//') $($LLVMCFG --ldflags) $($LLVMCFG --libs all) -o "$OUT/fable_lower"
fi
EXCL=${EXCLUDE:-$OUT/exclude.txt}
[ -n "${EXCLUDE:-}" ] || grep -ohE "(hook|wrap) 0x[0-9A-Fa-f]+" "$REPO/rebuild/recomp/HANDOFF.md" | awk '{print $2}' | sort -u > "$EXCL"

# 1. the files: <name>_<address>.cpp
mkdir -p "$OUT/ir"; rm -f "$OUT"/ir/*.ll "$OUT"/ir/*.err
( cd "$FD/rebuild/src/compiled" && find . -name "*.cpp" ) > "$OUT/all_files.txt"
: > "$OUT/files.txt"
while read -r a; do
  [ -n "$a" ] || continue
  a=$(printf "%08x" "0x${a#0x}")
  f=$(grep -m1 "_${a}\.cpp$" "$OUT/all_files.txt" || true)
  [ -n "$f" ] && echo "${a^^} $f" >> "$OUT/files.txt" || echo "no matched source for ${a^^}" >&2
done < "$LIST"

# 2. 32-bit MSVC-ABI IR (VC7.1 dialect), every definition weak so duplicate helpers link
CF="--target=i686-pc-windows-msvc -fms-compatibility -fms-compatibility-version=13.10 -fms-extensions -fdelayed-template-parsing
    -fno-exceptions -w -std=c++98 -O2 -fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks -isystem $HERE/shim -I $FD/rebuild/include"
compile() { local a=$1 f=$2
  if clang++ $CF -S -emit-llvm -o "$OUT/ir/$a.ll.tmp" "$FD/rebuild/src/compiled/$f" 2> "$OUT/ir/$a.err"; then
    sed -E 's/^define dso_local /define weak_odr dso_local /; s/^define (x86_|noundef|zeroext|signext|void|i[0-9]|float|double|ptr)/define weak_odr \1/' \
      "$OUT/ir/$a.ll.tmp" > "$OUT/ir/$a.ll"
  fi
  rm -f "$OUT/ir/$a.ll.tmp"
}
export -f compile; export OUT CF FD
xargs -P "$(nproc)" -n 2 bash -c 'compile "$0" "$1"' < "$OUT/files.txt"
echo "compiled $(ls "$OUT"/ir/*.ll 2>/dev/null | wc -l) of $(wc -l < "$OUT/files.txt") files"

# 3. the landed function of each one-definition file (others are found through the bindings)
python3 - "$OUT" <<'PY'
import re, glob, os, sys
out = sys.argv[1]
with open(os.path.join(out, 'natives.tsv'), 'w') as w:
    for ll in sorted(glob.glob(os.path.join(out, 'ir', '*.ll'))):
        names = []
        for line in open(ll):
            if line.startswith('define weak_odr'):
                m = re.search(r'@("(?:[^"\\]|\\.)*"|[\w.$]+)\(', line)
                if m: names.append(m.group(1).strip('"'))
        if len(names) == 1: w.write(os.path.basename(ll)[:-3] + '\t' + names[0] + '\n')
PY

# 4. link, lower per host, compile
llvm-link-18 -S "$OUT"/ir/*.ll -o "$OUT/linked.ll"
for T in x64:x86_64-linux-gnu a64:aarch64-linux-gnu win:x86_64-w64-windows-gnu; do
  n=${T%%:*}; t=${T#*:}
  "$OUT/fable_lower" "$OUT/linked.ll" "$OUT/low_$n.bc" --bindings "$FD/rebuild/oracles/reloc_bindings.tsv" --natives "$OUT/natives.tsv" \
    --exclude "$EXCL" --triple "$t" --report "$OUT/report.tsv"
  pic=-fPIC; [ "$n" = win ] && pic=
  clang --target="$t" -O2 $pic -Wno-override-module -c "$OUT/low_$n.bc" -o "$OUT/nd_$n.o"
done
awk -F'\t' 'NR>1 && $3=="native"{printf "--wrap 0x%s=nd_wrap_%s ", $1, $1}' "$OUT/report.tsv" > "$OUT/wraps.txt"
echo "native: $(awk -F'\t' 'NR>1 && $3=="native"' "$OUT/report.tsv" | wc -l), lifted: $(awk -F'\t' 'NR>1 && $3=="lifted"' "$OUT/report.tsv" | wc -l) (report.tsv)"

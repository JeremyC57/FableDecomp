#!/bin/bash
# usage: cc.sh file.cpp [extra cl flags...]  -> file.obj, file.asm (objdump -dr), file.cl.log
export S=$S
source $S/wine/env.sh >/dev/null 2>&1
f=$(readlink -f "$1"); shift
d=$(dirname "$f"); b=$(basename "$f" .cpp)
VC=$S/wine/vc71rtm
inc="Z:${VC//\//\\}\\include"
inc="$inc;Z:\\home\\user\\FableDecomp\\rebuild\\include"
if [ -n "$OVL" ]; then inc="Z:${OVL//\//\\};$inc"; fi
export INCLUDE="$inc"
rm -f "$d/$b.obj"
w() { echo "Z:${1//\//\\}"; }
timeout 120 wine $VC/bin/cl.exe /nologo /c /O2 /Oy /W3 "$@" "/Fo$(w $d/$b.obj)" "$(w $f)" > "$d/$b.cl.log" 2>&1
rc=$?
[ -f "$d/$b.obj" ] || { echo "COMPILE FAIL rc=$rc"; tail -20 "$d/$b.cl.log"; exit 1; }
objdump -d -r -M intel --no-show-raw-insn "$d/$b.obj" > "$d/$b.asm"
grep -v '^\s*$' "$d/$b.cl.log" | grep -v "$b.cpp$" | head -30

# vtlib429.py: genmodel.py's retail vftable reader (RTTI type descriptor -> COL -> vftable), importable.
# genmodel.py <Class> [--base <Base> --basehdr <engine/Base.h> --basesw SW1,SW2] [--slot N=<mangled>] [--src <header>]
# NF322 (2026-11-23): writes hdr/engine/<Class>.h = the CURRENT repo header with an opt-in polymorphic model
#   #ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL  <new model>  #else  <flat model, text unchanged>  #endif
# inserted round the flat model (from its `#pragma pack(push, 1)` line to its last FABLE_STATIC_ASSERT line).
# Slots come from the retail vftable found through RTTI (tools/vtdump.py logic) and their RTTI-port labels:
#   - base-less mode (no --base): every slot must be the class's own (label names <Class>, or --slot override); the model declares
#     all of them in slot order (slot 0 `??_E` / `??_G` -> `virtual ~<Class>();`), replacing `void* __vftable`.
#   - base mode: only slots whose label names <Class> are declared (overrides; inherited slots stay the base's), and slots past the
#     base's retail slot count must all be <Class>'s (new virtuals, declared in slot order). Members below sizeof(base) are dropped.
# Data members are the flat model's lines, unchanged (same names, types, offset comments); the opt-in pins sizeof and every kept
# member offset with FABLE_STATIC_ASSERT inside the #ifdef. Forward declarations for every class / struct / enum the declarations
# name (key from the mangled name: V class, U struct, W4 enum), namespaces kept. Declarations only, no bodies.
# Prints a one-line report and writes work/model_<Class>.tsv (slot, addr, label, action).
import sys, re, struct, os, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF429')
R = Path('/home/user/FableDecomp')
args=[]
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; secs = []
for i in range(ns):
    name = d[so + i * 40: so + i * 40 + 8].rstrip(b'\0').decode('latin1')
    vs, va, rs, raw = struct.unpack_from('<IIII', d, so + i * 40 + 8)
    secs.append((name, base + va, max(vs, rs), raw, rs))
def sec_of(va):
    for nm, s, n, r, rs in secs:
        if s <= va < s + n: return nm
def rd(va):
    for nm, s, n, r, rs in secs:
        if s <= va < s + rs: return struct.unpack_from('<I', d, r + va - s)[0]
def off2va(o):
    for nm, s, n, r, rs in secs:
        if r <= o < r + rs: return s + o - r
def find_all(pat):
    i = d.find(pat)
    while i >= 0:
        yield i; i = d.find(pat, i + 1)
L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open(R / 'ghidra_out' / f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[0].lower(), p[1])
def vtable(cls):
    res = []
    for o in find_all(b'.?AV' + cls.encode() + b'@@\0'):
        tdva = off2va(o) - 8
        for o1 in find_all(struct.pack('<I', tdva)):
            col = off2va(o1) - 12
            if col is None or rd(col) != 0 or rd(col + 4) != 0: continue
            for o2 in find_all(struct.pack('<I', col)):
                vt = off2va(o2) + 4
                if sec_of(vt) not in ('.rdata', '.data'): continue
                sl = []; j = 0
                while True:
                    t = rd(vt + 4 * j)
                    if t is None or sec_of(t) != '.text': break
                    sl.append(t); j += 1
                res.append((vt, sl))
    return res

"""cross-check each Alloc->vtable pair: the class's sdd (slot 0) or the dtor it calls stores the same vtable;
and the vtable dword is referenced in .text only by the Alloc/ctor and sdd/dtor."""
import sys, json, struct, collections, re
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/bounds')
from extent import Exe
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
E = Exe(); md = Cs(CS_ARCH_X86, CS_MODE_32)
J = json.load(open('scan.json')); NV = json.load(open('named_vts.json'))
d = E.d
def text_refs(v):
    p = struct.pack('<I', v); out = []; o0 = E.off(E.tlo); i = o0; end = o0 + len(E.text)
    while True:
        i = d.find(p, i, end)
        if i < 0: break
        out.append(E.tlo + i - o0); i += 1
    return out
res = collections.Counter(); bad = []
for a, A in J['allocs'].items():
    vt = A['vt']; sdd = E.dword(vt)
    body = E.read(sdd, 0x60); ok = struct.pack('<I', vt) in body
    if not ok:
        for i in md.disasm(body, sdd):
            if i.mnemonic == 'call':
                try: t = int(i.op_str, 16)
                except: continue
                if struct.pack('<I', vt) in E.read(t, 0x80): ok = True
                break
            if i.mnemonic == 'ret': break
    tr = text_refs(vt)
    al = int(a, 16); ct = A['ctor']
    inside = all((al <= r < al + 0x100) or (ct and ct <= r < ct + 0x200) or (sdd <= r < sdd + 0x100) for r in tr)
    res[(ok, inside)] += 1
    if not ok or not inside: bad.append((a, '%08x' % vt, NV.get('%08x' % vt), ok, ['%08x' % r for r in tr]))
print(res)
for b in bad: print(b)

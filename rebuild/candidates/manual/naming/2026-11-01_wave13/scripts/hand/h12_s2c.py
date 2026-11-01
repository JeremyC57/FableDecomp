# sub-wave 2c (hand): nest CMemoryArea / CMemoryAreaChunk in CMemoryAllocatorVariableSize (Ego_r + FableWin), with the PDB access
# (RemoveAreaFromFreeList / InsertAreaIntoFreeList / NewChunk / NewChunkDemandPaged / InsertIntoTree / RemoveFromTree protected).
# Rows 00a65060 00a650f0 00a65410 00a65590 00a65830 00a65930; 00a658e0 is CMemoryAreaChunk::FreeArea (PDB-build order: FreeArea
# 0x00C079E0 sits between Allocate and the ctor 0x00C07A30, 0x50 bytes = the retail 80-byte body at 0x00A658E0 just before the ctor
# 0x00A65930; body unlinks from the allocated list +0x30 and decrements NoAllocatedAreas +0x08, then InsertAreaIntoFreeList +
# ConsolidateAdjacentFreeAreas). HE_OV=<overlay>
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from he import put, edit, edit_all
import rows
MARK = '// Nested (naming wave 12)'
PROT = ('RemoveAreaFromFreeList(', 'InsertAreaIntoFreeList(', 'NewChunk(', 'NewChunkDemandPaged(', 'InsertIntoTree(', 'RemoveFromTree(')
def block(t, rx):
    m = re.search(rx, t, re.M)
    if not m: return None
    i = t.index('{', m.start()); d = 0; j = i
    while True:
        c = t[j]
        if c == '{': d += 1
        elif c == '}':
            d -= 1
            if d == 0: break
        j += 1
    k = t.index(';', j) + 1
    if k < len(t) and t[k] == '\n': k += 1
    return m.start(), k
def prot(body, keep_public=()):
    out = []
    for l in body.split('\n'):
        s = l.strip()
        if any(s.startswith(p) or re.match(r'(void|CMemory\w+\*)\s+' + re.escape(p), s) for p in PROT) and not any(k in s for k in keep_public):
            out += ['protected:', l, 'public:']
        else: out.append(l)
    return '\n'.join(out).replace('public:\nprotected:\n', '')
def nest(t):
    if MARK in t: return t
    t = t.replace('struct CMemoryAreaChunk;\n', '', 1)
    spans = {}
    for k, rx in (('A', r'^struct CMemoryArea\b'), ('C', r'^struct CMemoryAreaChunk\b'), ('B', r'^struct CMemoryAllocatorBase\b'), ('V', r'^class CMemoryAllocatorVariableSize\b[^;]*\{')):
        s = block(t, rx)
        if s: spans[k] = s
    txt = {k: t[a:b] for k, (a, b) in spans.items()}
    pos = min(a for a, b in spans.values())
    for k, (a, b) in sorted(spans.items(), key=lambda x: -x[1][0]): t = t[:a] + t[b:]
    def reopen(s, name):
        first, rest = s.split('\n', 1)
        first = re.sub(r'^struct (CMemoryArea|CMemoryAreaChunk)\s*\{', 'class CMemoryAllocatorVariableSize::%s { public:' % name, first)
        return first + '\n' + rest
    A = reopen(txt['A'], 'CMemoryArea'); C = prot(reopen(txt['C'], 'CMemoryAreaChunk'))
    if 'V' in txt:
        first, rest = txt['V'].split('\n', 1)
        V = first + '\n    class CMemoryArea;\n    class CMemoryAreaChunk;\n    friend class CMemoryAreaChunk;\n' + prot(rest)
    else:
        V = 'class CMemoryAllocatorVariableSize { public:\n    class CMemoryArea;\n    class CMemoryAreaChunk;\n};\n'
    new = (MARK + ': Ego_r.pdb nests CMemoryArea / CMemoryAreaChunk in CMemoryAllocatorVariableSize.\n' + txt.get('B', '') + V + A + C +
           'typedef CMemoryAllocatorVariableSize::CMemoryArea CMemoryArea;\ntypedef CMemoryAllocatorVariableSize::CMemoryAreaChunk CMemoryAreaChunk;\n')
    return t[:pos] + new + t[pos:]
for a in ('00a65060', '00a650f0', '00a65410', '00a65590', '00a65830', '00a65930'):
    t = open(os.path.join(rows.ROOT, 'rebuild/src/compiled', he.R[a]['Source']), encoding='utf-8', errors='surrogateescape', newline='').read()
    put(a, 'src', nest(t))
# ---- 0x00A658E0 CMemoryAllocatorVariableSize::CMemoryAreaChunk::FreeArea(CMemoryArea*) -- the landed model, respelled onto the PDB members
F = '''// CMemoryAllocatorVariableSize::CMemoryAreaChunk::FreeArea(CMemoryArea* area) -- unlink the area from the chunk's allocated list (area next at +0, prev at +8; the chunk's PFirstAllocatedArea at +0x30), clear its links, decrement NoAllocatedAreas (+8), then re-insert the area into the free list (0x00A650F0) and consolidate its neighbours (0x00A65590)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A658E0, length 80 bytes.
// Leaf: ?FreeArea@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAVCMemoryArea@2@@Z
// Name: Ego_r.pdb + FableWin.pdb CMemoryAreaChunk::FreeArea (public); PDB-build order puts FreeArea (0x00C079E0, 0x50 bytes) right
//   before the CMemoryAllocatorVariableSize ctor (0x00C07A30), as retail 0x00A658E0 (80 bytes) sits before the ctor 0x00A65930.
//   (Landed before naming wave 12 as a second RemoveAreaFromFreeList; 0x00A65060 is RemoveAreaFromFreeList.)
// Callees: InsertAreaIntoFreeList @ 0x00A650F0 (protected), ConsolidateAdjacentFreeAreas @ 0x00A65590 (both thiscall on the chunk with
// the area). Shape: the area in esi, `this` in edi, `ret 4`.
// Layout: Ego_r.pdb (CMemoryArea PNextArea +0x00, PPreviousArea +0x08; CMemoryAreaChunk NoAllocatedAreas +0x08, PFirstAllocatedArea +0x30).
class CMemoryAllocatorVariableSize { public:
    class CMemoryArea;
    class CMemoryAreaChunk;
};
class CMemoryAllocatorVariableSize::CMemoryArea { public:
    CMemoryArea*      PNextArea;                   // +0x00
    CMemoryArea*      PRightChild;                 // +0x04
    CMemoryArea*      PPreviousArea;               // +0x08
};
class CMemoryAllocatorVariableSize::CMemoryAreaChunk { public:
    long              TotalDataSize;               // +0x00
    long              NoFreeAreas;                 // +0x04
    long              NoAllocatedAreas;            // +0x08
    unsigned char     _pad0c[0x24];                // +0x0c
    CMemoryArea*      PFirstAllocatedArea;         // +0x30
    void ConsolidateAdjacentFreeAreas(CMemoryArea* area);   // 0x00A65590
    void FreeArea(CMemoryArea* area);
protected:
    void InsertAreaIntoFreeList(CMemoryArea* area);         // 0x00A650F0
};
typedef CMemoryAllocatorVariableSize::CMemoryArea CMemoryArea;
typedef CMemoryAllocatorVariableSize::CMemoryAreaChunk CMemoryAreaChunk;

void CMemoryAreaChunk::FreeArea(CMemoryArea* area)
{
    if (area->PPreviousArea)
        area->PPreviousArea->PNextArea = area->PNextArea;
    else
        PFirstAllocatedArea = area->PNextArea;
    if (area->PNextArea)
        area->PNextArea->PPreviousArea = area->PPreviousArea;
    area->PNextArea = 0;
    area->PPreviousArea = 0;
    --NoAllocatedAreas;
    InsertAreaIntoFreeList(area);
    ConsolidateAdjacentFreeAreas(area);
}
'''
put('00a658e0', 'src', F)
edit_all('00a658e0', 'tst', 'RemoveAreaFromFreeList', 'FreeArea')

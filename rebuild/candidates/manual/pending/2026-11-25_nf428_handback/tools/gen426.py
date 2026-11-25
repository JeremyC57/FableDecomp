#!/usr/bin/env python3
"""gen426.py <out dir> [obj substrings...]: ONE generator for the unreferenced out-of-line CDefPointer<const T> COMDAT copies (NF426).
Input evidence/single.json (tools/single.py): count-equal single-T retail TUs (2 retail copies, FableWin.pdb module stream lists exactly
?Release@ and ??1 of one CDefPointer<const T>) with an Ego_r.pdb second source. Lower address = Release (VC7.1 emits a COMDAT before the
COMDATs that inline it: bottom-up order, measured on toy TUs), higher = ~CDefPointer. Keeps the HEAD row's own `#pragma optimize` line.
Writes <out>/rows/<addr>.cpp, <out>/tests/<addr>_test.cpp and <out>/PLAN.tsv (address member T old_leaf new_leaf obj)."""
import sys, os, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); out = sys.argv[1]; want = sys.argv[2:]
S = json.load(open(A + '/evidence/single.json'))
os.makedirs(out + '/rows', exist_ok=True); os.makedirs(out + '/tests', exist_ok=True)
plan = []
def tname(T):
    m = re.match(r'^\$\$CBV(\w+)@@$', T)
    return m.group(1) if m else None
for x in S:
    if not x['nego'] or not all(x['fam']) or x['tmpl'] != 'CDefPointer': continue
    if want and not any(w in x['obj'] for w in want): continue
    t = tname(x['T'])
    if not t: continue
    a1, a2 = sorted(x['addrs'])
    obj = x['obj'].split('\\')[-1]
    ego = [e for e in x['ego'] if re.match(r'^(CDefClassBase::GetSubDef|CDefinitionManager::GetDef)<', e)]
    if not ego: continue                       # the second source must be an instantiation on T
    ego = ego[0]
    for a, mem in ((a1, 'Release'), (a2, 'dtor')):
        head = nf.src_of(a).read_text(encoding='utf-8')
        old = re.search(r'^// Leaf:\s*(\S+)', head, re.M).group(1)
        prag = [l for l in head.splitlines() if l.startswith('#pragma optimize')]
        n = len(bytes.fromhex(nf.catalog()[a]['bytes']))
        if mem == 'Release':
            leaf = '?Release@?$CDefPointer@$$CBV%s@@@@QAEXXZ' % t
            l1 = '// CDefPointer::Release() -- drop the def handle\'s counted reference (the pointee deletes itself on the last one) and empty it'
            inst = 'template void CDefPointer<const %s>::Release();' % t
            what = 'inline CDefPointer<const %s>::Release()' % t
            order = 'the lower of the two retail copies (0x%08X) is Release, the higher (0x%08X) the destructor: VC7.1 /O1 and\n// /O2 emit an inline\'s out-of-line Release copy before the ~CDefPointer copy that inlines it, /Od the other way round (NF426 toy TUs;\n// /Od reproduces FableWin.pdb\'s dtor-first module order, which all 62 single-T modules show).' % (int(a1, 16), int(a2, 16))
        else:
            leaf = '??1?$CDefPointer@$$CBV%s@@@@QAE@XZ' % t
            l1 = '// CDefPointer::~CDefPointer() -- destroy the def handle: its inline Release() expanded (count down, pointee self-delete at zero, empty)'
            inst = 'template CDefPointer<const %s>::~CDefPointer();' % t
            what = 'inline CDefPointer<const %s>::~CDefPointer() (Release() forced inline, the header\'s FABLE_DEFPTR_INLINE)' % t
            order = 'the higher of the two retail copies (0x%08X) is the destructor, the lower (0x%08X) Release: VC7.1 /O1 and\n// /O2 emit an inline\'s out-of-line Release copy before the ~CDefPointer copy that inlines it, /Od the other way round (NF426 toy TUs;\n// /Od reproduces FableWin.pdb\'s dtor-first module order, which all 62 single-T modules show).' % (int(a2, 16), int(a1, 16))
        src = '\n'.join([l1, '// Leaf: ' + leaf,
            '// Retail: Fable.exe (TLC, VC7.1) @ 0x%08X, %d bytes, no retail caller: the unreferenced out-of-line COMDAT copy of the' % (int(a, 16), n),
            '// %s that the %s TU emits (retail links without /OPT:REF).' % (what, obj[:-4]),
            '// Identity (NF426, two sources): T from FableWin.pdb\'s %s module stream, whose only CDefPointer / CIVCountedPointer' % x['obj'],
            '// copies are ?Release@ and ??1 of CDefPointer<const %s> (the retail TU holds exactly two such copies), and Ego_r.pdb\'s' % t,
            '// %s (%s). Member: %s' % (obj, ego, order),
            '// Retail check (NF426 assignscan, 56 single-T TUs): no assign body (SetDynamicCast / operator=, which expand Release) sits below the',
            '// lower copy in any of them; 15 sit between the copies. The toys do not reproduce retail\'s layout, only that order.',
            '// Emitted by explicit instantiation of the shared header\'s member (a select-any COMDAT, as retail\'s inline copy).'] + prag + [
            '#define FABLE_RETAIL_DEF_CLASS_BASE', '#include "engine/CDefPointer.h"', '', 'class %s;' % t, '', inst, ''])
        open(out + '/rows/%s.cpp' % a, 'w').write(src)
        call = 'p.Release();' if mem == 'Release' else '{ CDefPointer<const %s> q; q.Object = p.Object; p.Object = 0; q.~CDefPointer(); if (q.Object != 0) return 0; }' % t
        decl = 'void Release();' if mem == 'Release' else '~CDefPointer();'
        tag = nf.field(a, 'PassPattern') or ('%s_TEST PASS' % a)   # the row's catalog PassPattern, unchanged
        test = '''// Behaviour test for 0x%(A)s: links the row object's %(leaf)s (declared here, defined only there).
#include <cstdio>
class CDefPointeeBase;
class %(t)s;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    %(decl)s
};
static int g_deletes = 0;
static const void* g_deleted = 0;
struct TestPointee {                       // the pointee layout the row relies on: vptr, then IVRefCount at +4; slot 1 = Delete
    virtual ~TestPointee() { }
    virtual void Delete() const { ++g_deletes; g_deleted = this; }
    unsigned long IVRefCount;
};
static int Run(TestPointee& obj, unsigned long count)
{
    obj.IVRefCount = count;
    CDefPointer<const %(t)s> p;
    p.Object = reinterpret_cast<CDefPointeeBase*>(&obj);
    %(call)s
    return p.Object == 0 ? 1 : 0;
}
int main()
{
    TestPointee last, shared;
    if (!Run(last, 1) || last.IVRefCount != 0 || g_deletes != 1 || g_deleted != &last) return 1;
    if (!Run(shared, 2) || shared.IVRefCount != 1 || g_deletes != 1) return 2;
    CDefPointer<const %(t)s> empty;
    empty.Object = 0;
%(emptycall)s
    if (g_deletes != 1) return 3;
    std::printf("%(tag)s\\n");
    return 0;
}
''' % dict(A=a.upper(), leaf=leaf, t=t, decl=decl, call=call, tag=tag,
           emptycall=('    empty.Release();' if mem == 'Release' else '    { CDefPointer<const %s> q; q.Object = 0; }' % t))
        open(out + '/tests/%s_test.cpp' % a, 'w').write(test)
        plan.append([a, mem, t, old, leaf, x['obj'], ''.join(x['shapes'])])
open(out + '/PLAN.tsv', 'w').write(''.join('\t'.join(p) + '\n' for p in plan))
print(len(plan))

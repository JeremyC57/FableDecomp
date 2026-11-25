#!/usr/bin/env python3
"""gen428_reset.py <out dir> [inst addrs comma list]: ONE generator for the CCountedPointer<T>::Reset stand-in self rows (local SCountBlock
model, self ?Reset@CCountedPointer_<x>@@QAEXPAX@Z) whose T has two sources: (1) the retail callers' table spelling, the PDB name
?Reset@?$CCountedPointer@<T>@@@@QAEXP..@Z (whole FableWin.pdb entry), (2) the deleter the retail body stores in the new count block
(`mov dword [eax+4], imm32`) is bound, alone, to ?DeleteData@?$CCountedPointer@<T>@@@@SIXPAX@Z (evidence/dd_reset.tsv status DD_OK).
Form = NF371's landed strong specialization (0x00447D90): RESET_CAMERA_DECL + DELETEDATA_DECL_ONLY, body written out. Rows listed in the
second argument take NF371 R1 form b instead (the sibling explicit instantiation), for a T whose binder emits a pick-any Reset copy.
Writes rows/<a>.cpp, tests/<a>_test.cpp, PLAN.tsv (addr member T old_leaf new_leaf deleter form)."""
import sys, os, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); out = sys.argv[1]; inst = set((sys.argv[2] if len(sys.argv) > 2 else '').split(','))
os.makedirs(out + '/rows', exist_ok=True); os.makedirs(out + '/tests', exist_ok=True)
fam = {r['target']: r for r in json.load(open(A + '/fam.json'))}
def tdecl(T):
    """mangled pointee -> (const?, C++ type, forward declaration) for a plain or namespace-scoped class; None otherwise"""
    c = T.startswith('$$CB'); t = T[4:] if c else T
    m = re.match(r'^V(\w+)(?:@(N\w+))?$', t)
    if not m: return None
    cls, ns = m.group(1), m.group(2)
    full = (ns + '::' if ns else '') + cls
    d = ('namespace %s { class %s; }' % (ns, cls)) if ns else 'class %s;' % cls
    return c, full, d
plan = []
for l in open(A + '/evidence/dd_reset.tsv'):
    a, T, d, st, names, blk, n = l.rstrip('\n').split('\t')
    if st != 'DD_OK' or blk: continue
    td = tdecl(T)
    if not td: continue
    c, full, decl = td
    ct = ('const ' if c else '') + full
    old = fam[a]['self_leaf']; new = fam[a]['pdb_spelling']
    head = nf.src_of(a).read_text(encoding='utf-8')
    assert re.search(r'^// Leaf:\s*' + re.escape(old) + r'\s*$', head, re.M), a
    callers = [x.strip() for x in fam[a]['spellings'].split('||') if x.strip().startswith(new)][0]
    callers = re.search(r'\(([^)]*)\)', callers).group(1)
    form = 'inst' if a in inst else 'spec'
    hdr = ['// CCountedPointer::Reset(T* data) -- (CCountedPointer<%s>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block' % ct,
           '// Leaf: ' + new,
           '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes, thiscall, `ret 4`. Stores DeleteData 0x%s.' % (a.upper(), n, d.upper()),
           '// Names (NF428, 2026-11-25; was the stand-in %s::Reset with a local count-block model, self' % old.split('@')[1],
           '//   %s): T from two sources: (1) the retail callers bind the PDB name (table, at %s), a whole' % (old, callers),
           '//   FableWin.pdb entry; (2) retail RTTI: a retail call site passes a freshly constructed %s (its vftable RTTI or ??0 call' % ct,
           '//   just before the call; NF428 rtti428). The deleter the body stores, 0x%s, is bound to' % d.upper(),
           '//   ?DeleteData@?$CCountedPointer@%s@@@@SIXPAX@Z, a name derived from (1), consistent with it.' % T]
    if form == 'spec':
        hdr += ['// Form (NF371, landed 0x00447D90): a strong explicit specialization of engine/CCountedPointer.h\'s member with the landed body',
                '//   (ReleaseData written out; Data = data; for a non-null data the count block re-reads Data through `this`; a failed new leaves Info 0).',
                '//   FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL keeps the in-class body out, so the specialization is emitted as a plain function',
                '//   (COMDAT selection 1) like the stand-in; FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY leaves the stored deleter an external',
                '//   reference to its owner row.',
                '#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL', '#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY',
                '#include "engine/CCountedPointer.h"', decl, '',
                'template <>', 'void CCountedPointer<%s>::Reset(%s* data)' % (ct, ct), '{',
                '    if (Info) {', '        --Info->RefCount;', '        if (Info->RefCount == 0) {', '            Info->DeleteFunc(Info->Data);',
                '            delete Info;', '        }', '    }', '    Info = 0;', '    Data = data;', '    if (data)',
                '        Info = new CCPPointerInfo(1, DeleteData, (void*)Data);   // re-read through `this` (retail loads [esi] for the block), not the argument',
                '}', '']
    else:
        hdr += ['// Form (NF371 R1 form b, landed 0x00A299C0): the sibling explicit instantiation of engine/CCountedPointer.h\'s member, because a',
                '//   landed binder emits a pick-any copy of this Reset (a strong definition would be an equal-body LNK2005);',
                '//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY leaves the stored deleter an external reference to its owner row.',
                '#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY', '#include "engine/CCountedPointer.h"', decl, '',
                'template void CCountedPointer<%s>::Reset(%s*);' % (ct, ct), '']
    open(out + '/rows/%s.cpp' % a, 'w').write('\n'.join(hdr))
    tag = nf.field(a, 'PassPattern') or ('%s_TEST PASS' % a)
    nsopen = ''
    m = re.match(r'^(\w+)::(\w+)$', full)
    tdef = ('namespace %s { class %s { public: int v; }; }' % m.groups()) if m else 'class %s { public: int v; };' % full
    test = '''// Behaviour test for 0x%(A)s: links the row object's %(new)s.
// The shared engine/CCountedPointer.h with Reset declaration-only (FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL, so the call below is an
// external reference the row defines%(instnote)s) and DeleteData declaration-only (defined here for T, the row stores its address).
// T stays incomplete: Reset only stores the pointer, the deleter receives it as void*.
#include <cstdio>
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
%(decl)s
static int g_deletes = 0;
static const void* g_deleted = 0;
template <> void FABLE_FASTCALL CCountedPointer<%(ct)s>::DeleteData(void* data) { ++g_deletes; g_deleted = data; }
int main()
{
    static int objA, objB;                               // stand-in storage: T is only declared
    %(ct)s* a = reinterpret_cast<%(ct)s*>(&objA);
    %(ct)s* b = reinterpret_cast<%(ct)s*>(&objB);
    CCountedPointer<%(ct)s> p; p.Data = 0; p.Info = 0;
    p.Reset(a);                                         // adopt a: fresh block, count 1, deleter stored
    if (p.Data != a || !p.Info || p.Info->RefCount != 1 || p.Info->Data != (void*)a || p.Info->DeleteFunc != &CCountedPointer<%(ct)s>::DeleteData) return 1;
    CCPPointerInfo* first = p.Info;
    first->RefCount = 2;                                // a second sharer: Reset only drops our reference
    p.Reset(b);
    if (first->RefCount != 1 || g_deletes != 0 || p.Data != b || !p.Info || p.Info == first) return 2;
    p.Reset(0);                                         // last reference on b: deleter runs on b, pointer empties
    if (g_deletes != 1 || g_deleted != (void*)b || p.Data != 0 || p.Info != 0) return 3;
    delete first;
    p.Data = 0; p.Info = 0;                             // the header has no destructor body to run here
    std::printf("%(tag)s\\n");
    return 0;
}
''' % dict(A=a.upper(), new=new, ct=ct, decl=decl, tag=tag,
           instnote=('; the row is the explicit instantiation, the test alone opts in to the declaration' if form == 'inst' else ''))
    open(out + '/tests/%s_test.cpp' % a, 'w').write(test)
    plan.append([a, 'Reset', ct, old, new, d, form])
open(out + '/PLAN.tsv', 'w').write(''.join('\t'.join(p) + '\n' for p in plan))
print(len(plan))

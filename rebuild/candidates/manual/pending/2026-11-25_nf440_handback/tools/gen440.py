#!/usr/bin/env python3
"""gen440.py <run> <addr...>: row + test texts for assigned copies (evidence/assign.tsv) -> runs/<run>/{rows,tests}/, PLAN.tsv
(address member T old_leaf new_leaf decl form). The row stays the explicit instantiation of the shared header's body; only T (and, for a
ReleaseData copy, the member plus the header's FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA opt-in, the landed 0x00403750 / 0x004190B6 form)
changes. Tests include the shared header with the member declaration-only, so the call is the row's symbol (linked with the row object)."""
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from lib440 import *
import nf
run = sys.argv[1]; B = W + '/runs/' + run
for d in ('rows', 'tests', 'evidence', 'obj'): os.makedirs(B + '/' + d, exist_ok=True)
A = {f[0]: f for f in tsv(W + '/evidence/assign.tsv')}
lay = layouts()
plan = []

def decl(M):
    c = demangle_T(M)
    const = c.startswith('const ')
    c = c.replace('const ', '')
    parts = c.split('::')
    if any(p in lay for p in parts[:-1]): raise ValueError('class scope ' + c)
    key = 'struct' if M.replace('$$CB', '')[0] == 'U' else 'class'
    d = '%s %s;' % (key, parts[-1])
    for ns in reversed(parts[:-1]): d = 'namespace %s { %s }' % (ns, d)
    return ('const ' if const else '') + c, d

def basis_line(a, f):
    tier, note = f[3], f[7]
    if tier == 'T1':
        return '// Name: the binding table already binds this PDB name here (a landed caller); the self now spells it (one address, one name).'
    tn = note.split('; member: ')[0]
    t = ('T2: retail TU order v the FableWin.pdb module\'s pointee types (' + tn + ')') if tier == 'T2' else ('T3: neighbour (' + tn + ')')
    m = f[5]
    return ('// Name: best guess (owner rule 2026-11-25; one source, listed in rebuild/oracles/descriptive_names.tsv). T from ' + t +
            '. Member: ' + {'RD': 'ReleaseData', 'D': '~CCountedPointer', 'SH': 'ShareData'}[m] + ' (' +
(note.split('member: ')[1] if 'member: ' in note else 'the shape') + ').')

for a in sys.argv[2:]:
    f = A[a]; M, mem, sym = f[4], f[5], f[6]
    src = nf.src_of(a); head = open(src, encoding='utf-8').read()
    oldleaf = re.search(r'^// Leaf:\s*(\S+)', head, re.M).group(1)
    T, d = decl(M)
    size = head.startswith('#pragma optimize("s",on)')
    retail = re.search(r'^// Retail:.*$', head, re.M).group(0)
    L = []
    if size: L.append('#pragma optimize("s",on)')
    if mem == 'D':
        L += ['// CCountedPointer<%s>::~CCountedPointer() -- release one counted pointer' % T, '// Leaf: ' + sym, retail, '//',
              '// The shared count block\'s RefCount drops by one; at zero its DeleteFunc runs on the block\'s Data and the block is',
              '// freed. Data and then Info are cleared (the header\'s destructor body). Retail emits this body under both ~CCountedPointer<T>',
              '// and ReleaseData<T> (0x0041BC90 / 0x0041BB30 for CTexture); no caller names this copy.']
    elif mem == 'RD':
        L += ['// CCountedPointer<%s>::ReleaseData() -- release one counted pointer' % T, '// Leaf: ' + sym, retail, '//',
              '// The shared count block\'s RefCount drops by one; at zero its DeleteFunc runs on the block\'s Data and the block is',
              '// freed. Data and then Info are cleared (the header\'s FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA body, as the landed',
              '// ReleaseData rows 0x00403750 / 0x004190B6). Retail emits this body under both ~CCountedPointer<T> and ReleaseData<T>.']
    else:
        L += ['// CCountedPointer<%s>::ShareData(T* data, CCPPointerInfo* info) -- share another pointer\'s count block' % T, '// Leaf: ' + sym,
              retail, '//', '// When the block differs from the current one: release the current block (RefCount drop; at zero DeleteFunc on its Data',
              '// and the block freed), then adopt data / info and take a reference on the new block (engine/CCountedPointer.h).']
    L.append(basis_line(a, f))
    L.append('// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.')
    if mem == 'RD': L.append('#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA')
    if mem == 'SH':
        L.insert(len(L) - 1, '// The release ShareData expands is the ReleaseData<T> body retail emits (Data zeroed before Info); its zero stores are')
        L.insert(len(L) - 1, '// overwritten by the adoption, so the leaf is the same, and the pick-any ReleaseData<T> copy this TU emits is retail\'s body.')
        L.append('#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA')
    L.append('#include "engine/CCountedPointer.h"')
    L.append(d)
    if mem == 'D': L.append('template CCountedPointer<%s>::~CCountedPointer();' % T)
    elif mem == 'RD': L.append('template void CCountedPointer<%s>::ReleaseData();' % T)
    else: L.append('template void CCountedPointer<%s>::ShareData(%s*, CCPPointerInfo*);' % (T, T))
    open(B + '/rows/%s.cpp' % a, 'w').write('\n'.join(L) + '\n')
    # ---- test ----
    macro = {'D': 'FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY', 'RD': 'FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY',
             'SH': 'FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY'}[mem]
    call = {'D': 'p->CCountedPointer<%s>::~CCountedPointer()' % T, 'RD': 'p->ReleaseData()', 'SH': ''}[mem]
    t = ['// Behaviour test for 0x%s: links the row object\'s %s.' % (a.upper(), sym),
         '// The shared engine/CCountedPointer.h with the member declaration-only (%s), so the call is an external' % macro,
         '// reference the row defines. T stays incomplete: the count block\'s deleter receives the data as void*.',
         '#include <cstdio>', '#include <cstring>', '#define ' + macro, '#include "engine/CCountedPointer.h"', d,
         'static int g_deletes = 0;', 'static void* g_deleted = 0;',
         'static void FABLE_FASTCALL TestDelete(void* data) { ++g_deletes; g_deleted = data; }',
         'int main()', '{',
         '    static int objA, objB;',
         '    %s* a = reinterpret_cast<%s*>(&objA);' % (T, T), '    %s* b = reinterpret_cast<%s*>(&objB);' % (T, T),
         '    static char buf[sizeof(CCountedPointer<%s>)];' % T,
         '    CCountedPointer<%s>* p = reinterpret_cast<CCountedPointer<%s>*>(buf);' % (T, T)]
    if mem in ('D', 'RD'):
        t += ['    CCPPointerInfo* shared = new CCPPointerInfo(2, TestDelete, (void*)a);',
              '    p->Data = a; p->Info = shared;',
              '    %s;                                 // a second sharer remains: only the count drops' % call,
              '    if (shared->RefCount != 1 || g_deletes != 0 || p->Data != 0 || p->Info != 0) return 1;',
              '    p->Data = a; p->Info = shared;',
              '    %s;                                 // last reference: the deleter runs on Data, the block is freed' % call,
              '    if (g_deletes != 1 || g_deleted != (void*)a || p->Data != 0 || p->Info != 0) return 2;',
              '    p->Data = 0; p->Info = 0;',
              '    %s;                                 // empty pointer: nothing to release' % call,
              '    if (g_deletes != 1 || p->Data != 0 || p->Info != 0) return 3;']
    else:
        t += ['    CCPPointerInfo* first = new CCPPointerInfo(1, TestDelete, (void*)a);',
              '    CCPPointerInfo* second = new CCPPointerInfo(1, TestDelete, (void*)b);',
              '    p->Data = a; p->Info = first;',
              '    p->ShareData(a, first);                       // same block: nothing changes',
              '    if (first->RefCount != 1 || p->Data != a || p->Info != first) return 1;',
              '    p->ShareData(b, second);                      // last reference on the old block: deleter on a; adopt b, count 2',
              '    if (g_deletes != 1 || g_deleted != (void*)a || p->Data != b || p->Info != second || second->RefCount != 2) return 2;',
              '    p->ShareData(0, 0);                           // old block keeps a reference: no delete; pointer empties',
              '    if (g_deletes != 1 || second->RefCount != 1 || p->Data != 0 || p->Info != 0) return 3;',
              '    delete second;']
    pp = nf.field(a, 'PassPattern') or ('PASS_' + a)
    t += ['    std::printf("%s\\n");' % (pp if 'PASS' in pp else pp + ' PASS'), '    return 0;', '}']
    open(B + '/tests/%s_test.cpp' % a, 'w').write('\n'.join(t) + '\n')
    plan.append([a, mem, T, oldleaf, sym, d, 'expl'])
open(B + '/PLAN.tsv', 'a').write(''.join('\t'.join(x) + '\n' for x in plan))
print(len(plan), 'rows ->', B)

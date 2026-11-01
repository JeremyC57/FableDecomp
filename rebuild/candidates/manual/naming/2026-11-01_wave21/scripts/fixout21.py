# fixout21.py: post-fixes of RENAMES.tsv / LABELS.tsv the demangle-driven generators get wrong this wave (run after mkren21 / mklabels21):
#   00415d70 the conversion operator (qname dropped the class); 006d75c0 / 006d7640 anonymous-namespace hash only (no rename, no label);
#   012240a0 / 01224b40 dynamic initializers stay in _static_init (wave 13 GSystemManager precedent; Ego_r 0x0101AC00 / FableWin 0x0400AB10
#   `dynamic initializer for 'GDefStringTable'').
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
def rw(p, f):
    L = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, p))]
    out = [L[0]] + [r for r in (f(r) for r in L[1:]) if r]
    open(os.path.join(W, p), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
def ren(r):
    a = r[0]
    if a in ('006d75c0', '006d7640'): return None
    if a == '00415d70':
        r[2] = 'CDefString::operator CCharString'; r[4] = 'CDefString'
        r[6] = 'rebuild/src/compiled/00/41/CDefString_operatorCCharString_00415d70.cpp'; r[8] = r[8] and 'rebuild/tests/00/41/CDefString_operatorCCharString_00415d70_test.cpp'
    for addr, nm, f in (('012240a0', 'GDefStringTable', 'staticinit_GDefStringTable_012240a0'), ('01224b40', 'g_DefaultAllocator_0143bb34', 'staticinit_gDefaultAllocator0143bb34_01224b40')):
        if a == addr:
            r[2] = '_static_init::' + nm; r[4] = '_static_init'
            r[6] = 'rebuild/src/compiled/01/22/%s.cpp' % f; r[8] = r[8] and 'rebuild/tests/01/22/%s_test.cpp' % f
    return r
def lab(r):
    a = r[0]
    if a in ('006d75c0', '006d7640'): return None
    if a == '00415d70': r[1] = 'CDefString::operator CCharString'
    if a == '012240a0': r[1] = "_$E1 = dynamic initializer for 'GDefStringTable'"; r[2] = "naming wave 21 (gdst): Ego_r 0x0101AC00 / FableWin 0x0400AB10 `dynamic initializer for 'GDefStringTable''; constructs 0x013CA828 (was g_CStatic_009d4db0_013ca828)"
    if a == '01224b40': r[1] = "_$E1 = dynamic initializer for 'g_DefaultAllocator_0143bb34'"; r[2] = 'naming wave 21 (alloc): constructs the libpk default allocator 0x0143BB34 (descriptive, libpk; was g_CStatic_00c166f0_0143bb34)'
    return r
rw('RENAMES.tsv', ren); rw('LABELS.tsv', lab)

# h26_cti.py: naming wave 26, family cti -- the chunk 694 queue row CClassTypeInit (chunk 694 decision 2): AllocFunc is
#  CClass* (__fastcall*)() per FableWin.pdb ??0CClassTypeInit@@QAE@JP6IPAVCClass@@XZ_NABVCCharString@@@Z (P6I = __fastcall), as the landed
#  0x00A36F90 / 0x00A371C0 / 0x004C7DA0 models already spell it. The nine CClassTypeInfo models that spelled the pointer __cdecl now
#  spell __fastcall (the record is copied / filled, never called, in these rows: no mangled symbol carries the type).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
ROWS = ['0051fcc0', '00a37f40', '00a37e20', '00a38080', '00a37be0', '00a38150', '00a37b50', '00a380e0', '00a37c40']
NOTE = ('// Naming wave 26 (family cti, chunk 694 decision 2): CClassTypeInfo::AllocFunc is CClass* (__fastcall*)() (FableWin.pdb\n'
        '//   ??0CClassTypeInit@@QAE@JP6IPAVCClass@@XZ_NABVCCharString@@@Z); was __cdecl.')
for a in ROWS:
    p, rel, t = he._rw(a, 'src')
    t2, n = re.subn(r'CClass\*(\s*)\(__cdecl(\s*)\*(\s*\w*)\)\(\)', r'CClass*\1(__fastcall\2*\3)()', t)
    assert n == 1, (a, n)
    L = t2.split('\n'); lf = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')]
    assert lf, (a, 'no Leaf')
    L.insert(lf[0] + 1, NOTE); he._w(p, '\n'.join(L)); print('ok', a, 'src')

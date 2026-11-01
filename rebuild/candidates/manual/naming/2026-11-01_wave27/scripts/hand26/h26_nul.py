# h26_nul.py: naming wave 26, family nul -- the libpk nil-header pointer 0x0139C8FC is ?_NullString@@3PAUCStringData@@A
#  (DECISIONS_W25_RESOLVED D1: retail's own assert text "stringSrc.GetData() != _NullString" in Fable.exe; no _afx name in the
#  binary). The three _afxDataNil binders 0x00C0B910 / 0x00C0B9F0 / 0x00C0BA40 take the spelling 0x00C0C0C0 / 0x00C0DAB0 bind.
#  0x0139C900 (the nil characters, _afxPchNil) stays: DECISIONS_W25_RESOLVED D4 HOLD.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
NOTE = ('// Naming wave 26: the nil header pointer 0x0139C8FC is _NullString (retail assert text "stringSrc.GetData() != _NullString";\n'
        '//   DECISIONS_W25_RESOLVED D1); was _afxDataNil.\n')
for a in ('00c0b910', '00c0b9f0', '00c0ba40'):
    p, rel, t = he._rw(a, 'src')
    if '_afxDataNil' not in t: print('already', a, 'src'); continue
    t = t.replace('_afxDataNil', '_NullString')
    L = t.split('\n'); i = [j for j, x in enumerate(L) if x.startswith('// Leaf: ')][0]
    L.insert(i + 1, NOTE.rstrip('\n'))
    he._w(p, '\n'.join(L)); print('ok', a, 'src')

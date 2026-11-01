# h17_orient.py: naming wave 17, family data -- the twelve CTCInventoryItem orientation sets are FableWin.pdb anonymous-namespace
# constants of the CTCInventoryItem TU (c675: dynamic-initializer order, 0x0137CF74 DEFAULT_ORIENTATION ... 0x0137D034
# BACKWARDS_LEFT_FORTY_FIVE_DEGREES_ORIENTATION, as the landed GetRHSetFromOrientation 0x005D8EF0 declares them). Each landed row is
# its own lane TU, so the symbol carries that TU's ?A0x hash (bound per row). Also 0x005D92F0 calls the private static
# CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation) (Ego_r CI) instead of a free __fastcall stand-in. Name-only.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, _rw, _w
INIT = {'0121ac30': ('g_InventoryOrientationRHSet1', 'FORTY_FIVE_DEGREES_ORIENTATION', '0x013B8BCC'),
        '0121acd0': ('g_InventoryOrientationRHSet5', 'UPSIDE_DOWN_FORTY_FIVE_DEGREES_ORIENTATION', '0x013B8BE4'),
        '0121ad20': ('g_InventoryOrientationRHSet6', 'UPSIDE_DOWN_RIGHT_FORTY_FIVE_DEGREES_ORIENTATION', '0x0137CFD4'),
        '0121ad50': ('g_InventoryOrientationRHSet7', 'UPSIDE_DOWN_LEFT_FORTY_FIVE_DEGREES_ORIENTATION', '0x0137CFEC')}
for a, (old, new, addr) in INIT.items():
    p, rel, t = _rw(a, 'src')
    if old not in t: print('already', a); continue
    t = t.replace('-- dynamic initializer of CRightHandedSet %s @ %s' % (old, addr), '-- dynamic initializer of the anonymous-namespace const CRightHandedSet %s @ %s' % (new, addr))
    t = t.replace('//   identifier is not in the PDB name set: the name is descriptive.\n',
                  '//   identifier is the FableWin.pdb anonymous-namespace constant %s of the CTCInventoryItem TU (chunk c675:\n'
                  '//   dynamic-initializer order); naming wave 17 respelled the descriptive %s (name-only).\n' % (new, old))
    k = t.index('CRightHandedSet %s(' % old)
    e = t.index(';', k)
    t = t[:k] + 'namespace {\nconst CRightHandedSet %s(%s;\n}\n' % (new, t[k + len('CRightHandedSet %s(' % old):e]) + t[e + 1:].lstrip('\n')
    _w(p, t); print('ok', a, 'src')
edit('005d9730', 'src', 'extern const CRightHandedSet g_DefaultRHSet;            // 0x0137CF74',
     'namespace {\nextern const CRightHandedSet DEFAULT_ORIENTATION;      // 0x0137CF74 (FableWin anonymous-namespace constant of the TU; naming wave 17, was g_DefaultRHSet)\n}')
edit('005d9730', 'src', '    return g_DefaultRHSet;', '    return DEFAULT_ORIENTATION;')
edit('005d92f0', 'src', 'CRightHandedSet __fastcall GetRHSetFromOrientation(long orientation);    // 0x005D8EF0\n',
     'enum EInventoryObjectOrientation { };\n')
edit('005d92f0', 'src', '''    CRightHandedSet GetStartingRightHandedSet();
};''', '''    CRightHandedSet GetStartingRightHandedSet();
private:
    static CRightHandedSet __fastcall GetRHSetFromOrientation(EInventoryObjectOrientation orientation);   // 0x005D8EF0 (Ego_r CI; naming wave 17, was a free __fastcall stand-in)
};''')
edit('005d92f0', 'src', '    return GetRHSetFromOrientation(def->Orientation);', '    return GetRHSetFromOrientation((EInventoryObjectOrientation)def->Orientation);')

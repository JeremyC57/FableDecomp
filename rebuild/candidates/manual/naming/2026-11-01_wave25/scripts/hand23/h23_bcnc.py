# h23_bcnc.py: naming wave 23, family bcnc -- 0x0099A300 is CBaseClassNonCopyable::~CBaseClassNonCopyable (chunk 690 label; Ego_r.pdb /
# FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ, protected virtual, as engine/CBaseClassNonCopyable.h declares). The body stores only the
# CBaseClass vtable 0x01231710: the derived destructor's own vtable store (0x0129A7C4, the one its ctor 0x0099A2F0 writes) is dead once
# the inlined ~CBaseClass overwrites it. Retail kept ~CBaseClass 0x0099A2E0 and this destructor in one translation unit (BaseClass.cpp:
# Ego_r.pdb / FableWin.pdb place the CBaseClass ctor / dtor side by side, 0x0099A2D0..0x0099A300 are contiguous), so the own row defines
# both, as retail's TU did; the row was landed as a second ~CBaseClass (0x0099A2E0 is the landed ~CBaseClass).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
SRC = '''// CBaseClassNonCopyable::~CBaseClassNonCopyable() -- the non-copyable root's destructor: with the base destructor inlined only the
// CBaseClass vtable store remains (its own vtable store 0x0129A7C4 is dead before ~CBaseClass overwrites it)
// Leaf: ??1CBaseClassNonCopyable@@MAE@XZ
// Retail: Fable.exe @ 0x0099A300, 7 bytes, thiscall plain `ret`.
// Naming (naming wave 23): chunk 690 proved the identity (labels_chunk690: Ego_r + FableWin exact; called as the base destructor of the two
//   CSimpleMemoryPool and of C3DAnimationManager inside ~CEngineResourceManager); the PDB access is protected virtual (MAE), as
//   engine/CBaseClassNonCopyable.h declares. Was landed as a second CBaseClass::~CBaseClass (that is 0x0099A2E0).
// Shape: ~CBaseClass (0x0099A2E0) is defined in the same translation unit, as in retail's BaseClass.cpp (the ctor / dtor pairs
//   0x0099A2D0..0x0099A300 are contiguous), so VC7.1 inlines it and drops the dead derived vtable store.
// Callees: none.
#include "engine/CBaseClassNonCopyable.h"

CBaseClass::~CBaseClass()
{
}

CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
}'''
p, rel, t = he._rw('0099a300', 'src')
if not t.endswith('\n'): pass
he.put('0099a300', 'src', SRC + ('\n' if t.endswith('\n') else ''))
he.edit('0099a300', 'tst', '// Behaviour test for 0x0099A300 CBaseClass::~CBaseClass(): the empty root destructor, run through a virtual delete after\n// the derived destructor.',
        '// Behaviour test for 0x0099A300 CBaseClassNonCopyable::~CBaseClassNonCopyable(): the empty destructor (with the root\n// destructor inlined), run through a virtual delete after the derived destructor.')
# 0x00B3B8E0 (~CEngineResourceManager, chunk 690): the base destructor of its two CSimpleMemoryPool and C3DAnimationManager members is
# ~CBaseClassNonCopyable 0x0099A300 -- take the engine header (protected virtual, PDB MAE) for the hand-declared public pair.
he.edit('00b3b8e0', 'src', '''class CBaseClass { public:
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CBaseClassNonCopyable { public:
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};
''', '''#include "engine/CBaseClassNonCopyable.h"              // ~CBaseClass 0x0099A2E0; ~CBaseClassNonCopyable 0x0099A300 (protected virtual, PDB MAE; naming wave 23)
''')

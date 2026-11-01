// CBaseClassNonCopyable::~CBaseClassNonCopyable() -- the non-copyable root's destructor: with the base destructor inlined only the
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
}
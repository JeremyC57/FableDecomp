// NUISystem::CMorphingSprite::`scalar deleting destructor'(unsigned int flags) -- run ~CMorphingSprite, free the control when bit 0 is set
// Leaf: ??_GCMorphingSprite@NUISystem@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00555140, 30 bytes, thiscall, `ret 4`. Compiler-generated.
// Identity (two sources): slot 0 of CMorphingSprite's primary vtable 0x0124A49C (RTTI .?AVCMorphingSprite@NUISystem@@); it calls
//   ~CMorphingSprite 0x005541E0 and operator delete 0x00BFE9BC; the matched adjustor thunks 0x00555120 (+4) / 0x00555130 (+0x18) jump to
//   it. VC7.1 emits the function as ??_G and the vtable / thunk entry ??_ECMorphingSprite@NUISystem@@UAEPAXI@Z (Ego_r.pdb's slot-0 name,
//   0x005CE2B0) as a weak alias of it (objdump -t: scl 105; no array new in the TU).
// Was: the stand-in CMorphingSprite::ScalarDeletingDestructor (?ScalarDeletingDestructor@CMorphingSprite@@QAEPAXI@Z, local struct model,
//   rebuild/src/compiled/00/55/NUISystemCMorphingSprite_scalardeletingdestructor_00555140.cpp).
// TU: the destructor's TU, as the matched thunk 0x00555120 (~CMorphingSprite 0x005541E0 defined here makes VC7.1 emit ??_G and both
//   thunks; it is its own row, not graded here, and stays a call in the ??_G, as in retail). Only the ??_G is this row.
// Header: engine/NUISystem_CMorphingSprite.h.
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/NUISystem_CMorphingSprite.h"
namespace NUISystem {
CMorphingSprite::~CMorphingSprite()
{
    for (std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin(); it != Handles.end(); ++it)
        delete *it;
    Handles.clear();
}
}

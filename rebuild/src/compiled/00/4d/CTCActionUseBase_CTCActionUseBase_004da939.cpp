// CTCActionUseBase::CTCActionUseBase(CThing&) -- CTCBase(owner), own vtable, null action-use def, usable
// Leaf: ??0CTCActionUseBase@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCActionUseBase@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA939, length 32 bytes (a /Os module: unaligned entry,
// `push dword ptr [esp+8]` pass-through, `and dword ptr [..],0` zero store).
// Callee: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is a
// propagated name). Own vtable 0x0123F09C = CTCActionUseBase (rtti-vtables.tsv); the landed
// CTCActionUse* constructors (0x004DA987, 0x004DAB6C, ...) call this one.
//
// Layout = Ego_r.pdb CTCActionUseBase (0x14 bytes): CTCBase 0xc, CDefPointer<CActionUseDef> PActionUseDef
// +0x0c, Usable +0x10. Shape: base constructor, vtable, the def pointer's inline constructor (one zero
// store, `and` under /Os), then Usable = true. engine/CTCBase.h holds the layout only, so the base is
// modelled as `CTCBase_Calls` (as the landed CTCRockTrollShield).
#pragma optimize("s", on)

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CActionUseDef;

template<class T>
class CDefPointer { public:
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class CTCActionUseBase : public CTCBase { public:                   // vtable 0x0123F09C
    CDefPointer<CActionUseDef> PActionUseDef;               // +0x0c
    bool                       Usable;                      // +0x10
    CTCActionUseBase(CThing& owner);
};

CTCActionUseBase::CTCActionUseBase(CThing& owner)
    : CTCBase(owner)
    , Usable(true)
{
}
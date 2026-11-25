// CTCThingOwner::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCThingOwner::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCThingOwner@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DDB1C, 16 bytes: mov ecx,[esp+4]; call CTCThingOwner::StatGetClassTypeName 0x004D318D; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCThingOwner@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012412A4 (RTTI .?AVCTCThingOwner@@) slot 22 (0x012412FC -> 0x004DDB1C) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D318D, the landed CTCThingOwner::StatGetClassTypeName (?StatGetClassTypeName@CTCThingOwner@@SI?AVCCharString@@XZ; literal "CTCThingOwner").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCThingOwner_GetClassTypeName@4 calling the made-up ?Init_004d318d@T@@QAEXXZ.
// Header: engine/CTCThingOwner.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCThingOwner CTCThingOwnerLayout
#include "engine/CTCThingOwner.h"
#undef CTCThingOwner

class CTCThingOwner : public CTCThingOwnerLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D318D
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCThingOwner::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

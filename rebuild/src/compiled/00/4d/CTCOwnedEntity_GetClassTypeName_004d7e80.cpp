// CTCOwnedEntity::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCOwnedEntity::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCOwnedEntity@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D7E80, 16 bytes: mov ecx,[esp+4]; call CTCOwnedEntity::StatGetClassTypeName 0x004D3589; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCOwnedEntity@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123EB4C (RTTI .?AVCTCOwnedEntity@@) slot 22 (0x0123EBA4 -> 0x004D7E80) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3589, the landed CTCOwnedEntity::StatGetClassTypeName (?StatGetClassTypeName@CTCOwnedEntity@@SI?AVCCharString@@XZ; literal "CTCOwnedEntity").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCOwnedEntity_GetClassTypeName@4 calling the made-up ?Init_004d3589@T@@QAEXXZ.
// Header: engine/CTCOwnedEntity.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCOwnedEntity CTCOwnedEntityLayout
#include "engine/CTCOwnedEntity.h"
#undef CTCOwnedEntity

class CTCOwnedEntity : public CTCOwnedEntityLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3589
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCOwnedEntity::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

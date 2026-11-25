// CTCFireRangedWeaponOnLeavingStateGroup::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCFireRangedWeaponOnLeavingStateGroup::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCFireRangedWeaponOnLeavingStateGroup@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D6591, 16 bytes: mov ecx,[esp+4]; call CTCFireRangedWeaponOnLeavingStateGroup::StatGetClassTypeName 0x004D655C; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCFireRangedWeaponOnLeavingStateGroup@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123E464 (RTTI .?AVCTCFireRangedWeaponOnLeavingStateGroup@@) slot 22 (0x0123E4BC -> 0x004D6591) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D655C, the landed CTCFireRangedWeaponOnLeavingStateGroup::StatGetClassTypeName (?StatGetClassTypeName@CTCFireRangedWeaponOnLeavingStateGroup@@SI?AVCCharString@@XZ; literal "CTCFireRangedWeaponOnLeavingStateGroup").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCFireRangedWeaponOnLeavingStateGroup_GetClassTypeName@4 calling the made-up ?Init_004d655c@T@@QAEXXZ.
// Header: no engine header defines CTCFireRangedWeaponOnLeavingStateGroup; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCFireRangedWeaponOnLeavingStateGroup@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCFireRangedWeaponOnLeavingStateGroup : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D655C
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCFireRangedWeaponOnLeavingStateGroup::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

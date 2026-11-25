// CTCSingleUserActionPoints::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCSingleUserActionPoints::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCSingleUserActionPoints@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E036C, 16 bytes: mov ecx,[esp+4]; call CTCSingleUserActionPoints::StatGetClassTypeName 0x004D34DA; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCSingleUserActionPoints@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01241EAC (RTTI .?AVCTCSingleUserActionPoints@@) slot 22 (0x01241F04 -> 0x004E036C) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D34DA, the landed CTCSingleUserActionPoints::StatGetClassTypeName (?StatGetClassTypeName@CTCSingleUserActionPoints@@SI?AVCCharString@@XZ; literal "CTCSingleUserActionPoints").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCSingleUserActionPoints_GetClassTypeName@4 calling the made-up ?Init_004d34da@T@@QAEXXZ.
// Header: engine/CTCUserActionPointBase.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCSingleUserActionPoints CTCSingleUserActionPointsLayout
#include "engine/CTCUserActionPointBase.h"
#undef CTCSingleUserActionPoints

class CTCSingleUserActionPoints : public CTCSingleUserActionPointsLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D34DA
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCSingleUserActionPoints::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

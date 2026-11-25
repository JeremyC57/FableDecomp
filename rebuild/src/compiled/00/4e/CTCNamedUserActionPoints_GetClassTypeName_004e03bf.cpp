// CTCNamedUserActionPoints::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCNamedUserActionPoints::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCNamedUserActionPoints@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E03BF, 16 bytes: mov ecx,[esp+4]; call CTCNamedUserActionPoints::StatGetClassTypeName 0x004D34ED; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCNamedUserActionPoints@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01241F24 (RTTI .?AVCTCNamedUserActionPoints@@) slot 22 (0x01241F7C -> 0x004E03BF) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D34ED, the landed CTCNamedUserActionPoints::StatGetClassTypeName (?StatGetClassTypeName@CTCNamedUserActionPoints@@SI?AVCCharString@@XZ; literal "CTCNamedUserActionPoints").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCNamedUserActionPoints_GetClassTypeName@4 calling the made-up ?Init_004d34ed@T@@QAEXXZ.
// Header: engine/CTCNamedUserActionPoints.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCNamedUserActionPoints CTCNamedUserActionPointsLayout
#include "engine/CTCNamedUserActionPoints.h"
#undef CTCNamedUserActionPoints

class CTCNamedUserActionPoints : public CTCNamedUserActionPointsLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D34ED
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCNamedUserActionPoints::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

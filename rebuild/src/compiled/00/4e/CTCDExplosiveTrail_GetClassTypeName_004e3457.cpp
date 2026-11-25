// CTCDExplosiveTrail::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDExplosiveTrail::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDExplosiveTrail@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3457, 16 bytes: mov ecx,[esp+4]; call CTCDExplosiveTrail::StatGetClassTypeName 0x004D66C3; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDExplosiveTrail@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01242B4C (RTTI .?AVCTCDExplosiveTrail@@) slot 22 (0x01242BA4 -> 0x004E3457) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D66C3, the landed CTCDExplosiveTrail::StatGetClassTypeName (?StatGetClassTypeName@CTCDExplosiveTrail@@SI?AVCCharString@@XZ; literal "CTCDExplosiveTrail").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCDExplosiveTrail_GetClassTypeName@4 calling the made-up ?Init_004d66c3@T@@QAEXXZ.
// Header: engine/CTCDExplosiveTrail.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_EXPLOSIVE_TRAIL_UPDATE_PASS28
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCDExplosiveTrail CTCDExplosiveTrailLayout
#include "engine/CTCDExplosiveTrail.h"
#undef CTCDExplosiveTrail

class CTCDExplosiveTrail : public CTCDExplosiveTrailLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D66C3
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDExplosiveTrail::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

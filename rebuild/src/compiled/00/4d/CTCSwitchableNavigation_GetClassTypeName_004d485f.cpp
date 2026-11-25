// CTCSwitchableNavigation::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCSwitchableNavigation::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCSwitchableNavigation@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D485F, 16 bytes: mov ecx,[esp+4]; call CTCSwitchableNavigation::StatGetClassTypeName 0x004D482C; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCSwitchableNavigation@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123C02C (RTTI .?AVCTCSwitchableNavigation@@) slot 22 (0x0123C084 -> 0x004D485F) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D482C, the landed CTCSwitchableNavigation::StatGetClassTypeName (?StatGetClassTypeName@CTCSwitchableNavigation@@SI?AVCCharString@@XZ; literal "CTCSwitchableNavigation").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCSwitchableNavigation_GetClassTypeName@4 calling the made-up ?Init_004d482c@T@@QAEXXZ.
// Header: engine/CTCSwitchableNavigation.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCSwitchableNavigation CTCSwitchableNavigationLayout
#include "engine/CTCSwitchableNavigation.h"
#undef CTCSwitchableNavigation

class CTCSwitchableNavigation : public CTCSwitchableNavigationLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D482C
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCSwitchableNavigation::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

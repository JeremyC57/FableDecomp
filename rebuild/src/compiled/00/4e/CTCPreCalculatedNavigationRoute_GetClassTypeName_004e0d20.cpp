// CTCPreCalculatedNavigationRoute::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCPreCalculatedNavigationRoute::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCPreCalculatedNavigationRoute@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0D20, 16 bytes: mov ecx,[esp+4]; call CTCPreCalculatedNavigationRoute::StatGetClassTypeName 0x004D4D4B; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCPreCalculatedNavigationRoute@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01242444 (RTTI .?AVCTCPreCalculatedNavigationRoute@@) slot 22 (0x0124249C -> 0x004E0D20) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4D4B, the landed CTCPreCalculatedNavigationRoute::StatGetClassTypeName (?StatGetClassTypeName@CTCPreCalculatedNavigationRoute@@SI?AVCCharString@@XZ; literal "CTCPreCalculatedNavigationRoute").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCPreCalculatedNavigationRoute_GetClassTypeName@4 calling the made-up ?Init_004d4d4b@T@@QAEXXZ.
// Header: engine/CTCPreCalculatedNavigationRoute.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCPRECALCULATEDNAVIGATIONROUTE_LOAD_PASS26
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCPreCalculatedNavigationRoute CTCPreCalculatedNavigationRouteLayout
#include "engine/CTCPreCalculatedNavigationRoute.h"
#undef CTCPreCalculatedNavigationRoute

class CTCPreCalculatedNavigationRoute : public CTCPreCalculatedNavigationRouteLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4D4B
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCPreCalculatedNavigationRoute::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

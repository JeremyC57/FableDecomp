// CTCCarriedActionUseFishingRod::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseFishingRod::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseFishingRod@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB27D, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseFishingRod::StatGetClassTypeName 0x004D5CFD; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseFishingRod@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240634 (RTTI .?AVCTCCarriedActionUseFishingRod@@) slot 22 (0x0124068C -> 0x004DB27D) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5CFD, the landed CTCCarriedActionUseFishingRod::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseFishingRod@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseFishingRod").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseFishingRod_GetClassTypeName@4 calling the made-up ?Init_004d5cfd@T@@QAEXXZ.
// Header: engine/CTCCarriedActionUseFishingRod.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCCARRIEDACTIONUSEFISHINGROD_PASS22
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCCarriedActionUseFishingRod CTCCarriedActionUseFishingRodLayout
#include "engine/CTCCarriedActionUseFishingRod.h"
#undef CTCCarriedActionUseFishingRod

class CTCCarriedActionUseFishingRod : public CTCCarriedActionUseFishingRodLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5CFD
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseFishingRod::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

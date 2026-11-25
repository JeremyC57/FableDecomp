// CTCInterestingToVillagers::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCInterestingToVillagers::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCInterestingToVillagers@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB0EC, 16 bytes: mov ecx,[esp+4]; call CTCInterestingToVillagers::StatGetClassTypeName 0x004D4FEA; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCInterestingToVillagers@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012402F4 (RTTI .?AVCTCInterestingToVillagers@@) slot 22 (0x0124034C -> 0x004DB0EC) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4FEA, the landed CTCInterestingToVillagers::StatGetClassTypeName (?StatGetClassTypeName@CTCInterestingToVillagers@@SI?AVCCharString@@XZ; literal "CTCInterestingToVillagers").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCInterestingToVillagers_GetClassTypeName@4 calling the made-up ?Init_004d4fea@T@@QAEXXZ.
// Header: engine/CTCInterestingToVillagers.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCInterestingToVillagers CTCInterestingToVillagersLayout
#include "engine/CTCInterestingToVillagers.h"
#undef CTCInterestingToVillagers

class CTCInterestingToVillagers : public CTCInterestingToVillagersLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4FEA
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCInterestingToVillagers::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

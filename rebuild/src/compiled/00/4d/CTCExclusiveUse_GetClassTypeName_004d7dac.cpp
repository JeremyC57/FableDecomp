// CTCExclusiveUse::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCExclusiveUse::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCExclusiveUse@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D7DAC, 16 bytes: mov ecx,[esp+4]; call CTCExclusiveUse::StatGetClassTypeName 0x004D3500; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCExclusiveUse@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123EA5C (RTTI .?AVCTCExclusiveUse@@) slot 22 (0x0123EAB4 -> 0x004D7DAC) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3500, the landed CTCExclusiveUse::StatGetClassTypeName (?StatGetClassTypeName@CTCExclusiveUse@@SI?AVCCharString@@XZ; literal "CTCExclusiveUse").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCExclusiveUse_GetClassTypeName@4 calling the made-up ?Init_004d3500@T@@QAEXXZ.
// Header: engine/CTCExclusiveUse.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCEXCLUSIVEUSE
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCExclusiveUse CTCExclusiveUseLayout
#include "engine/CTCExclusiveUse.h"
#undef CTCExclusiveUse

class CTCExclusiveUse : public CTCExclusiveUseLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3500
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCExclusiveUse::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

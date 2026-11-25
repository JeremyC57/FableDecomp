// CTCGeneratedCreature::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCGeneratedCreature::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCGeneratedCreature@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D8026, 16 bytes: mov ecx,[esp+4]; call CTCGeneratedCreature::StatGetClassTypeName 0x004D3780; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCGeneratedCreature@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123EBC4 (RTTI .?AVCTCGeneratedCreature@@) slot 22 (0x0123EC1C -> 0x004D8026) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3780, the landed CTCGeneratedCreature::StatGetClassTypeName (?StatGetClassTypeName@CTCGeneratedCreature@@SI?AVCCharString@@XZ; literal "CTCGeneratedCreature").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCGeneratedCreature_GetClassTypeName@4 calling the made-up ?Init_004d3780@T@@QAEXXZ.
// Header: engine/CTCGeneratedCreature.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCGeneratedCreature CTCGeneratedCreatureLayout
#include "engine/CTCGeneratedCreature.h"
#undef CTCGeneratedCreature

class CTCGeneratedCreature : public CTCGeneratedCreatureLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3780
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCGeneratedCreature::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

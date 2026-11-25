// CTCCreatureGroup::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCreatureGroup::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCreatureGroup@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5C61, 16 bytes: mov ecx,[esp+4]; call CTCCreatureGroup::StatGetClassTypeName 0x004D4789; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCreatureGroup@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124326C (RTTI .?AVCTCCreatureGroup@@) slot 22 (0x012432C4 -> 0x004E5C61) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4789, the landed CTCCreatureGroup::StatGetClassTypeName (?StatGetClassTypeName@CTCCreatureGroup@@SI?AVCCharString@@XZ; literal "CTCCreatureGroup").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCreatureGroup_GetClassTypeName@4 calling the made-up ?Init_004d4789@T@@QAEXXZ.
// Header: engine/CTCCreatureGroup.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCCreatureGroup CTCCreatureGroupLayout
#include "engine/CTCCreatureGroup.h"
#undef CTCCreatureGroup

class CTCCreatureGroup : public CTCCreatureGroupLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4789
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCreatureGroup::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

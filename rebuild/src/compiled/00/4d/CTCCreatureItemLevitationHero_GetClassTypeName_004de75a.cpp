// CTCCreatureItemLevitationHero::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCreatureItemLevitationHero::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCreatureItemLevitationHero@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE75A, 16 bytes: mov ecx,[esp+4]; call CTCCreatureItemLevitationHero::StatGetClassTypeName 0x004D45A1; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCreatureItemLevitationHero@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124167C (RTTI .?AVCTCCreatureItemLevitationHero@@) slot 22 (0x012416D4 -> 0x004DE75A) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D45A1, the landed CTCCreatureItemLevitationHero::StatGetClassTypeName (?StatGetClassTypeName@CTCCreatureItemLevitationHero@@SI?AVCCharString@@XZ; literal "CTCCreatureItemLevitationHero").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCreatureItemLevitationHero_GetClassTypeName@4 calling the made-up ?Init_004d45a1@T@@QAEXXZ.
// Header: no engine header defines CTCCreatureItemLevitationHero; a local class over its RTTI nearest ancestor with a header (direct base CTCCreatureItemLevitationBase has none) engine/CTCBase.h (rtti_bases.tsv: .?AVCTCCreatureItemLevitationHero@@ : CTCCreatureItemLevitationBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCCreatureItemLevitationHero : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D45A1
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCreatureItemLevitationHero::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

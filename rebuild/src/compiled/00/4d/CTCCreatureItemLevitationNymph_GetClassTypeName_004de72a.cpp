// CTCCreatureItemLevitationNymph::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCreatureItemLevitationNymph::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCreatureItemLevitationNymph@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE72A, 16 bytes: mov ecx,[esp+4]; call CTCCreatureItemLevitationNymph::StatGetClassTypeName 0x004D458E; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCreatureItemLevitationNymph@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012415F4 (RTTI .?AVCTCCreatureItemLevitationNymph@@) slot 22 (0x0124164C -> 0x004DE72A) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D458E, the landed CTCCreatureItemLevitationNymph::StatGetClassTypeName (?StatGetClassTypeName@CTCCreatureItemLevitationNymph@@SI?AVCCharString@@XZ; literal "CTCCreatureItemLevitationNymph").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCreatureItemLevitationNymph_GetClassTypeName@4 calling the made-up ?Init_004d458e@T@@QAEXXZ.
// Header: no engine header defines CTCCreatureItemLevitationNymph; a local class over its RTTI nearest ancestor with a header (direct base CTCCreatureItemLevitationBase has none) engine/CTCBase.h (rtti_bases.tsv: .?AVCTCCreatureItemLevitationNymph@@ : CTCCreatureItemLevitationBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCCreatureItemLevitationNymph : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D458E
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCreatureItemLevitationNymph::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

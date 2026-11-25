// CTCCarriedActionUseAbility::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseAbility::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseAbility@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB58D, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseAbility::StatGetClassTypeName 0x004D673A; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseAbility@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240B1C (RTTI .?AVCTCCarriedActionUseAbility@@) slot 22 (0x01240B74 -> 0x004DB58D) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D673A, the landed CTCCarriedActionUseAbility::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseAbility@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseAbility").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseAbility_GetClassTypeName@4 calling the made-up ?Init_004d673a@T@@QAEXXZ.
// Header: no engine header defines CTCCarriedActionUseAbility; a local class over its RTTI direct base engine/CTCCarriedActionUseBase.h (rtti_bases.tsv: .?AVCTCCarriedActionUseAbility@@ : CTCCarriedActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCCarriedActionUseBase.h"

class CTCCarriedActionUseAbility : public CTCCarriedActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D673A
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseAbility::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

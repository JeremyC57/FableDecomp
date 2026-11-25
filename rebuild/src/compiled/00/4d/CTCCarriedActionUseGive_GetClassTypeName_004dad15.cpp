// CTCCarriedActionUseGive::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseGive::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseGive@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAD15, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseGive::StatGetClassTypeName 0x004D2CDA; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseGive@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123FAB4 (RTTI .?AVCTCCarriedActionUseGive@@) slot 22 (0x0123FB0C -> 0x004DAD15) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2CDA, the landed CTCCarriedActionUseGive::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseGive@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseGive").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseGive_GetClassTypeName@4 calling the made-up ?Init_004d2cda@T@@QAEXXZ.
// Header: no engine header defines CTCCarriedActionUseGive; a local class over its RTTI direct base engine/CTCCarriedActionUseBase.h (rtti_bases.tsv: .?AVCTCCarriedActionUseGive@@ : CTCCarriedActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCCarriedActionUseBase.h"

class CTCCarriedActionUseGive : public CTCCarriedActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2CDA
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseGive::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

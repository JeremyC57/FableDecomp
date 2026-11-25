// CTCCarriedActionUseGoldBag::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseGoldBag::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseGoldBag@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAD6A, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseGoldBag::StatGetClassTypeName 0x004D2D00; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseGoldBag@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123FBC4 (RTTI .?AVCTCCarriedActionUseGoldBag@@) slot 22 (0x0123FC1C -> 0x004DAD6A) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2D00, the landed CTCCarriedActionUseGoldBag::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseGoldBag@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseGoldBag").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseGoldBag_GetClassTypeName@4 calling the made-up ?Init_004d2d00@T@@QAEXXZ.
// Header: no engine header defines CTCCarriedActionUseGoldBag; a local class over its RTTI direct base engine/CTCCarriedActionUseBase.h (rtti_bases.tsv: .?AVCTCCarriedActionUseGoldBag@@ : CTCCarriedActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCCarriedActionUseBase.h"

class CTCCarriedActionUseGoldBag : public CTCCarriedActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2D00
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseGoldBag::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

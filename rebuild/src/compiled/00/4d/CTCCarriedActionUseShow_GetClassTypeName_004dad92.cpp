// CTCCarriedActionUseShow::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseShow::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseShow@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAD92, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseShow::StatGetClassTypeName 0x004D2D13; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseShow@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123FC4C (RTTI .?AVCTCCarriedActionUseShow@@) slot 22 (0x0123FCA4 -> 0x004DAD92) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2D13, the landed CTCCarriedActionUseShow::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseShow@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseShow").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseShow_GetClassTypeName@4 calling the made-up ?Init_004d2d13@T@@QAEXXZ.
// Header: no engine header defines CTCCarriedActionUseShow; a local class over its RTTI direct base engine/CTCCarriedActionUseBase.h (rtti_bases.tsv: .?AVCTCCarriedActionUseShow@@ : CTCCarriedActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCCarriedActionUseBase.h"

class CTCCarriedActionUseShow : public CTCCarriedActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2D13
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseShow::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

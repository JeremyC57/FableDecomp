// CTCCarriedActionUseRead::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseRead::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseRead@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB386, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseRead::StatGetClassTypeName 0x004D5EB7; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseRead@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240834 (RTTI .?AVCTCCarriedActionUseRead@@) slot 22 (0x0124088C -> 0x004DB386) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5EB7, the landed CTCCarriedActionUseRead::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseRead@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseRead").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseRead_GetClassTypeName@4 calling the made-up ?Init_004d5eb7@T@@QAEXXZ.
// Header: no engine header defines CTCCarriedActionUseRead; a local class over its RTTI direct base engine/CTCCarriedActionUseBase.h (rtti_bases.tsv: .?AVCTCCarriedActionUseRead@@ : CTCCarriedActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCCarriedActionUseBase.h"

class CTCCarriedActionUseRead : public CTCCarriedActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5EB7
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseRead::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

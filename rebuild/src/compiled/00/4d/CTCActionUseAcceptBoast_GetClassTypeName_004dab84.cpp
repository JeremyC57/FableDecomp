// CTCActionUseAcceptBoast::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUseAcceptBoast::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUseAcceptBoast@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAB84, 16 bytes: mov ecx,[esp+4]; call CTCActionUseAcceptBoast::StatGetClassTypeName 0x004D2C55; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUseAcceptBoast@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123F674 (RTTI .?AVCTCActionUseAcceptBoast@@) slot 22 (0x0123F6CC -> 0x004DAB84) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2C55, the landed CTCActionUseAcceptBoast::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUseAcceptBoast@@SI?AVCCharString@@XZ; literal "CTCActionUseAcceptBoast").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUseAcceptBoast_GetClassTypeName@4 calling the made-up ?Init_004d2c55@T@@QAEXXZ.
// Header: no engine header defines CTCActionUseAcceptBoast; a local class over its RTTI direct base engine/CTCActionUseBase.h (rtti_bases.tsv: .?AVCTCActionUseAcceptBoast@@ : CTCActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCActionUseBase.h"

class CTCActionUseAcceptBoast : public CTCActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2C55
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUseAcceptBoast::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

// CTCGeneratesExperienceOnKilling::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCGeneratesExperienceOnKilling::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCGeneratesExperienceOnKilling@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5787, 16 bytes: mov ecx,[esp+4]; call CTCGeneratesExperienceOnKilling::StatGetClassTypeName 0x004D5756; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCGeneratesExperienceOnKilling@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123D224 (RTTI .?AVCTCGeneratesExperienceOnKilling@@) slot 22 (0x0123D27C -> 0x004D5787) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5756, the landed CTCGeneratesExperienceOnKilling::StatGetClassTypeName (?StatGetClassTypeName@CTCGeneratesExperienceOnKilling@@SI?AVCCharString@@XZ; literal "CTCGeneratesExperienceOnKilling").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCGeneratesExperienceOnKilling_GetClassTypeName@4 calling the made-up ?Init_004d5756@T@@QAEXXZ.
// Header: no engine header defines CTCGeneratesExperienceOnKilling; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCGeneratesExperienceOnKilling@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCGeneratesExperienceOnKilling : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5756
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCGeneratesExperienceOnKilling::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

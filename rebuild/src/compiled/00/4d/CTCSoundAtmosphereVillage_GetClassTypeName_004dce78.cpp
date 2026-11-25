// CTCSoundAtmosphereVillage::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCSoundAtmosphereVillage::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCSoundAtmosphereVillage@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCE78, 16 bytes: mov ecx,[esp+4]; call CTCSoundAtmosphereVillage::StatGetClassTypeName 0x004D6122; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCSoundAtmosphereVillage@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124111C (RTTI .?AVCTCSoundAtmosphereVillage@@) slot 22 (0x01241174 -> 0x004DCE78) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D6122, the landed CTCSoundAtmosphereVillage::StatGetClassTypeName (?StatGetClassTypeName@CTCSoundAtmosphereVillage@@SI?AVCCharString@@XZ; literal "CTCSoundAtmosphereVillage").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCSoundAtmosphereVillage_GetClassTypeName@4 calling the made-up ?Init_004d6122@T@@QAEXXZ.
// Header: no engine header defines CTCSoundAtmosphereVillage; a local class over its RTTI nearest ancestor with a header (direct base CTCSoundAtmosphere has none) engine/CTCBase.h (rtti_bases.tsv: .?AVCTCSoundAtmosphereVillage@@ : CTCSoundAtmosphere).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCSoundAtmosphereVillage : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D6122
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCSoundAtmosphereVillage::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

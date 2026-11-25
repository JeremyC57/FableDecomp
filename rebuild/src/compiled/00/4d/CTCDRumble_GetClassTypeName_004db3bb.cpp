// CTCDRumble::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDRumble::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDRumble@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB3BB, 16 bytes: mov ecx,[esp+4]; call CTCDRumble::StatGetClassTypeName 0x004D5FF2; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDRumble@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012408BC (RTTI .?AVCTCDRumble@@) slot 22 (0x01240914 -> 0x004DB3BB) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5FF2, the landed CTCDRumble::StatGetClassTypeName (?StatGetClassTypeName@CTCDRumble@@SI?AVCCharString@@XZ; literal "CTCDRumble").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCDRumble_GetClassTypeName@4 calling the made-up ?Init_004d5ff2@T@@QAEXXZ.
// Header: no engine header defines CTCDRumble; a local class over its RTTI direct base engine/CTCDriverBase.h (rtti_bases.tsv: .?AVCTCDRumble@@ : CTCDriverBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCDriverBase.h"

class CTCDRumble : public CTCDriverBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5FF2
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDRumble::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

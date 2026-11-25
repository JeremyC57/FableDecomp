// CTCUndeadSoul::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCUndeadSoul::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCUndeadSoul@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2DF7, 16 bytes: mov ecx,[esp+4]; call CTCUndeadSoul::StatGetClassTypeName 0x004D2DC8; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCUndeadSoul@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123A364 (RTTI .?AVCTCUndeadSoul@@) slot 22 (0x0123A3BC -> 0x004D2DF7) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2DC8, the landed CTCUndeadSoul::StatGetClassTypeName (?StatGetClassTypeName@CTCUndeadSoul@@SI?AVCCharString@@XZ; literal "CTCUndeadSoul").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCUndeadSoul_GetClassTypeName@4 calling the made-up ?Init_004d2dc8@T@@QAEXXZ.
// Header: no engine header defines CTCUndeadSoul; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCUndeadSoul@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCUndeadSoul : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2DC8
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCUndeadSoul::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

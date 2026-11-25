// CTCTavernTankard::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCTavernTankard::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCTavernTankard@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D8B6C, 16 bytes: mov ecx,[esp+4]; call CTCTavernTankard::StatGetClassTypeName 0x004D51A2; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCTavernTankard@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123EDCC (RTTI .?AVCTCTavernTankard@@) slot 22 (0x0123EE24 -> 0x004D8B6C) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D51A2, the landed CTCTavernTankard::StatGetClassTypeName (?StatGetClassTypeName@CTCTavernTankard@@SI?AVCCharString@@XZ; literal "CTCTavernTankard").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCTavernTankard_GetClassTypeName@4 calling the made-up ?Init_004d51a2@T@@QAEXXZ.
// Header: engine/CTCTavernTankard.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCTavernTankard CTCTavernTankardLayout
#include "engine/CTCTavernTankard.h"
#undef CTCTavernTankard

class CTCTavernTankard : public CTCTavernTankardLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D51A2
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCTavernTankard::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

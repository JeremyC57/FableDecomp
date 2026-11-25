// CTCDiceGame::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDiceGame::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDiceGame@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB407, 16 bytes: mov ecx,[esp+4]; call CTCDiceGame::StatGetClassTypeName 0x004D6069; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDiceGame@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124093C (RTTI .?AVCTCDiceGame@@) slot 22 (0x01240994 -> 0x004DB407) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D6069, the landed CTCDiceGame::StatGetClassTypeName (?StatGetClassTypeName@CTCDiceGame@@SI?AVCCharString@@XZ; literal "CTCDiceGame").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCDiceGame_GetClassTypeName@4 calling the made-up ?Init_004d6069@T@@QAEXXZ.
// Header: no engine header defines CTCDiceGame; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCDiceGame@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCDiceGame : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D6069
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDiceGame::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

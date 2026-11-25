// CTCDPhysicalObstruction::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDPhysicalObstruction::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDPhysicalObstruction@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF1AC, 16 bytes: mov ecx,[esp+4]; call CTCDPhysicalObstruction::StatGetClassTypeName 0x004D5FDB; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDPhysicalObstruction@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01241C0C (RTTI .?AVCTCDPhysicalObstruction@@) slot 22 (0x01241C64 -> 0x004DF1AC) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5FDB, the landed CTCDPhysicalObstruction::StatGetClassTypeName (?StatGetClassTypeName@CTCDPhysicalObstruction@@SI?AVCCharString@@XZ; literal "CTCDPhysicalObstruction").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCDPhysicalObstruction_GetClassTypeName@4 calling the made-up ?Init_004d5fdb@T@@QAEXXZ.
// Header: engine/CTCDPhysicalObstruction.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCDPhysicalObstruction CTCDPhysicalObstructionLayout
#include "engine/CTCDPhysicalObstruction.h"
#undef CTCDPhysicalObstruction

class CTCDPhysicalObstruction : public CTCDPhysicalObstructionLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5FDB
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDPhysicalObstruction::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

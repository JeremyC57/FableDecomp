// CTCTavernGameMaster::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCTavernGameMaster::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCTavernGameMaster@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5B4E, 16 bytes: mov ecx,[esp+4]; call CTCTavernGameMaster::StatGetClassTypeName 0x004D5B19; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCTavernGameMaster@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123D664 (RTTI .?AVCTCTavernGameMaster@@) slot 22 (0x0123D6BC -> 0x004D5B4E) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5B19, the landed CTCTavernGameMaster::StatGetClassTypeName (?StatGetClassTypeName@CTCTavernGameMaster@@SI?AVCCharString@@XZ; literal "CTCTavernGameMaster").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCTavernGameMaster_GetClassTypeName@4 calling the made-up ?Init_004d5b19@T@@QAEXXZ.
// Header: engine/CTCTavernGameMaster.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCTavernGameMaster CTCTavernGameMasterLayout
#include "engine/CTCTavernGameMaster.h"
#undef CTCTavernGameMaster

class CTCTavernGameMaster : public CTCTavernGameMasterLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5B19
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCTavernGameMaster::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

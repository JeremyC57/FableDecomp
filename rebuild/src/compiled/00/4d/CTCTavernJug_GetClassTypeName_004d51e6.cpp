// CTCTavernJug::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCTavernJug::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCTavernJug@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D51E6, 16 bytes: mov ecx,[esp+4]; call CTCTavernJug::StatGetClassTypeName 0x004D51B5; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCTavernJug@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123CA14 (RTTI .?AVCTCTavernJug@@) slot 22 (0x0123CA6C -> 0x004D51E6) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D51B5, the landed CTCTavernJug::StatGetClassTypeName (?StatGetClassTypeName@CTCTavernJug@@SI?AVCCharString@@XZ; literal "CTCTavernJug").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCTavernJug_GetClassTypeName@4 calling the made-up ?Init_004d51b5@T@@QAEXXZ.
// Header: engine/CTCTavernJug.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCTavernJug CTCTavernJugLayout
#include "engine/CTCTavernJug.h"
#undef CTCTavernJug

class CTCTavernJug : public CTCTavernJugLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D51B5
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCTavernJug::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

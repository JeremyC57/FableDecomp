// NThingShot::CTCShotDefinitionThrow::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionThrow::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionThrow@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D548A, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionThrow::StatGetClassTypeName 0x004D5447; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionThrow@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123CDC4 (RTTI .?AVCTCShotDefinitionThrow@NThingShot@@) slot 22 (0x0123CE1C -> 0x004D548A) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5447, the landed NThingShot::CTCShotDefinitionThrow::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionThrow@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionThrow").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionThrow_GetClassTypeName@4 calling the made-up ?Init_004d5447@T@@QAEXXZ.
// Header: engine/NThingShot_CTCShotDefinitionThrowPredictive.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCShotDefinitionThrow CTCShotDefinitionThrowLayout
#include "engine/NThingShot_CTCShotDefinitionThrowPredictive.h"
#undef CTCShotDefinitionThrow

namespace NThingShot {
class CTCShotDefinitionThrow : public CTCShotDefinitionThrowLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5447
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionThrow::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

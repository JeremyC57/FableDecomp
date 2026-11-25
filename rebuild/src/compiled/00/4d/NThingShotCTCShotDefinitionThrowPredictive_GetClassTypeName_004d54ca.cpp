// NThingShot::CTCShotDefinitionThrowPredictive::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionThrowPredictive::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionThrowPredictive@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D54CA, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionThrowPredictive::StatGetClassTypeName 0x004D549F; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionThrowPredictive@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123CE9C (RTTI .?AVCTCShotDefinitionThrowPredictive@NThingShot@@) slot 22 (0x0123CEF4 -> 0x004D54CA) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D549F, the landed NThingShot::CTCShotDefinitionThrowPredictive::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionThrowPredictive@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionThrowPredictive").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionThrowPredictive_GetClassTypeName@4 calling the made-up ?Init_004d549f@T@@QAEXXZ.
// Header: engine/NThingShot_CTCShotDefinitionThrowPredictive.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCShotDefinitionThrowPredictive CTCShotDefinitionThrowPredictiveLayout
#include "engine/NThingShot_CTCShotDefinitionThrowPredictive.h"
#undef CTCShotDefinitionThrowPredictive

namespace NThingShot {
class CTCShotDefinitionThrowPredictive : public CTCShotDefinitionThrowPredictiveLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D549F
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionThrowPredictive::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

// NThingShot::CTCShotDefinitionProjectilePredictive::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionProjectilePredictive::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionProjectilePredictive@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCAA9, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionProjectilePredictive::StatGetClassTypeName 0x004D54DA; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionProjectilePredictive@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240EF4 (RTTI .?AVCTCShotDefinitionProjectilePredictive@NThingShot@@) slot 22 (0x01240F4C -> 0x004DCAA9) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D54DA, the landed NThingShot::CTCShotDefinitionProjectilePredictive::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionProjectilePredictive@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionProjectilePredictive").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionProjectilePredictive_GetClassTypeName@4 calling the made-up ?Init_004d54da@T@@QAEXXZ.
// Header: engine/NThingShot_CTCShotDefinitionProjectilePredictive.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCShotDefinitionProjectilePredictive CTCShotDefinitionProjectilePredictiveLayout
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"
#undef CTCShotDefinitionProjectilePredictive

namespace NThingShot {
class CTCShotDefinitionProjectilePredictive : public CTCShotDefinitionProjectilePredictiveLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D54DA
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionProjectilePredictive::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

// NThingShot::CTCShotDefinitionDrainLife::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionDrainLife::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionDrainLife@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB18B, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionDrainLife::StatGetClassTypeName 0x004D554F; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionDrainLife@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012403F4 (RTTI .?AVCTCShotDefinitionDrainLife@NThingShot@@) slot 22 (0x0124044C -> 0x004DB18B) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D554F, the landed NThingShot::CTCShotDefinitionDrainLife::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionDrainLife@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionDrainLife").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionDrainLife_GetClassTypeName@4 calling the made-up ?Init_004d554f@T@@QAEXXZ.
// Header: engine/NThingShot_CTCShotDefinitionDrainLife.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCShotDefinitionDrainLife CTCShotDefinitionDrainLifeLayout
#include "engine/NThingShot_CTCShotDefinitionDrainLife.h"
#undef CTCShotDefinitionDrainLife

namespace NThingShot {
class CTCShotDefinitionDrainLife : public CTCShotDefinitionDrainLifeLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D554F
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionDrainLife::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

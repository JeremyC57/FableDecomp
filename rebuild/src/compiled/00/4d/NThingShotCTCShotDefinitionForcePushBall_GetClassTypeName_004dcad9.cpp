// NThingShot::CTCShotDefinitionForcePushBall::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionForcePushBall::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionForcePushBall@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCAD9, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionForcePushBall::StatGetClassTypeName 0x004D5529; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionForcePushBall@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240FAC (RTTI .?AVCTCShotDefinitionForcePushBall@NThingShot@@) slot 22 (0x01241004 -> 0x004DCAD9) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5529, the landed NThingShot::CTCShotDefinitionForcePushBall::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionForcePushBall@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionForcePushBall").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionForcePushBall_GetClassTypeName@4 calling the made-up ?Init_004d5529@T@@QAEXXZ.
// Header: no engine header defines NThingShot::CTCShotDefinitionForcePushBall; a local class over its RTTI direct base engine/NThingShot_CTCShotDefinitionProjectilePredictive.h (rtti_bases.tsv: .?AVCTCShotDefinitionForcePushBall@NThingShot@@ : NThingShot::CTCShotDefinitionProjectile).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"

namespace NThingShot {
class CTCShotDefinitionForcePushBall : public CTCShotDefinitionProjectile
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5529
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionForcePushBall::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

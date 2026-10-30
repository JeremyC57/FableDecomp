// CGameScriptInterface::EntityTeleportToThing(const CScriptThing& entity, const CScriptThing& target, bool b) const -- script call: teleports a valid entity to a valid target's position and facing angle (through EntityTeleportToPosition)
// Leaf: ?EntityTeleportToThing@CGameScriptInterface@@UBEXABVCScriptThing@@0_N@Z
// Retail: Fable.exe @ 0x0088E540, 86 bytes, thiscall `ret 0Ch`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 472 holds this body; slot 472 is
//   ?EntityTeleportToThing@CGameScriptInterface@@UBEXABVCScriptThing@@0_N@Z; it calls slot 473 (+0x764)
//   ?EntityTeleportToPosition@CGameScriptInterface@@UBEXABVCScriptThing@@ABVC3DVector@@M_N2@Z.
// Body: if entity.IsAlive() and target.IsAlive() (CScriptThing slot 75, +0x12c):
//   EntityTeleportToPosition(entity, target.GetPos() (slot 6, +0x18), target.GetAngleXY() (slot 10, +0x28, a float), true, b).
//   VC evaluates the arguments right to left (GetAngleXY before GetPos). The CScriptThing slot names GetPos / GetAngleXY
//   are the Ego_r.pdb CScriptThing virtuals with these signatures (UBEABVC3DVector@@XZ, UBEMXZ); their slot numbers are
//   not proven (virtual calls bind no symbol). The last bool's name is not in the name sets (descriptive).
class C3DVector { public: float X, Y, Z; };
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntityTeleportToThing(const CScriptThing& entity, const CScriptThing& target, bool b) const
{
    if (entity.IsAlive() && target.IsAlive())
        EntityTeleportToPosition(entity, target.GetPos(), target.GetAngleXY(), true, b);
}
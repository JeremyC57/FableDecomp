// CGameScriptInterface::EntityTeleportToThing(const CScriptThing& entity, const CScriptThing& target, bool b) const -- script call: teleports a valid entity to a valid target's position and facing angle (through EntityTeleportToPosition)
// Leaf: ?EntityTeleportToThing@CGameScriptInterface@@UBEXABVCScriptThing@@0_N@Z
// Retail: Fable.exe @ 0x0088E540, 86 bytes, thiscall `ret 0Ch`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 472 holds this body; slot 472 is
//   ?EntityTeleportToThing@CGameScriptInterface@@UBEXABVCScriptThing@@0_N@Z; it calls slot 473 (+0x764)
//   ?EntityTeleportToPosition@CGameScriptInterface@@UBEXABVCScriptThing@@ABVC3DVector@@M_N2@Z.
// Body: if entity.IsValid() and target.IsValid() (CScriptThing slot 75, +0x12c):
//   EntityTeleportToPosition(entity, target.GetPos() (slot 6, +0x18), target.GetAngleXY() (slot 10, +0x28, a float), true, b).
//   VC evaluates the arguments right to left (GetAngleXY before GetPos). The CScriptThing slot names GetPos / GetAngleXY
//   are the Ego_r.pdb CScriptThing virtuals with these signatures (UBEABVC3DVector@@XZ, UBEMXZ); their slot numbers are
//   not proven (virtual calls bind no symbol). The last bool's name is not in the name sets (descriptive).
class C3DVector { public: float X, Y, Z; };
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
    virtual void v007(); virtual void v008(); virtual void v009();
    virtual float GetAngleXY() const;                    // slot 10 (+0x28)
    virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntityTeleportToThing(const CScriptThing& entity, const CScriptThing& target, bool b) const
{
    if (entity.IsValid() && target.IsValid())
        EntityTeleportToPosition(entity, target.GetPos(), target.GetAngleXY(), true, b);
}
// CTCActionUseWallMount::OnActionUse(CThing& user) [Ego_r.pdb; was OnActionUse(CThingCreatureBase*)] -- after the base handling, a living user first lets the mounted thing's wall-mount effects (0xDB) handle the use; when they do not, the user starts the use-wall-mount action on this object
// Leaf: ?OnActionUse@CTCActionUseWallMount
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EFE70, 256 bytes, `ret 4`; the OnActionUse slot 28 (catalog "vfunc_28").
// Callees: CTCActionUseBase::OnActionUse 0x007EAB90 (manifest "$E2" is propagated), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020, the wall mount's mounted-thing getter 0x007DE4E0 (interface 0xDA; descriptive name,
//   manifest label propagated), the effects' use test 0x007DF3D0 (manifest "CTCCameraCollision::HasCameraPhysicsMesh
//   Object", propagated; descriptive name) and CTCWallMountEffects::OnWallMountUseAction(CThingCreatureBase*)
//   0x007DF4B0 (interface 0xDB), CCreatureAction_UseWallMount ctor 0x00843610 (vtable 0x0127498C per RTTI; the
//   manifest and the landed 0x007EDDF0 call it CCreatureAction_DeclarePlayerACriminal), CThingCreatureBase::
//   SetCurrentAction 0x006644F0, ~CBaseIntelligentPointer 0x00A01C10, ~CCreatureActionBase 0x00693EF0.
// Layout: CTCBase ParentThing +4; CThing dead bit +0x91 bit 0, interface bitset +0x20, map +0x44; the action is
//   0xb0 bytes (retail CCreatureActionBase 0xa8 + the target intelligent pointer).
// Shape: nested ifs with the effects' veto as an early `return`; the wall-mount component is the ternary GetTC with its
//   own null test, the effects component a named local (esi); the action is a temporary passed by address straight
//   from the ctor (implicit destructor: member then base, no vtable store).
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCreatureActionBase;
class CThingPlayerCreature;

class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
inline bool IsThingAlive(const CThing* thing)
{
    return thing && !thing->IsDead();
}
class CTCWallMount { public:                           // interface 0xDA
    CThing* GetMountedThing();                  // 0x007DE4E0
};
class CTCWallMountEffects { public:                    // interface 0xDB
    bool IsUsable();                            // 0x007DF3D0
    bool OnWallMountUseAction(CThingCreatureBase* user);   // 0x007DF4B0
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_UseWallMount : public CCreatureActionBase { public:   // vtable 0x0127498C
    CIntelligentPointer<CThing> PWallMount;     // +0xa8
    CCreatureAction_UseWallMount(CThingCreatureBase* owner, CThingPlayerCreature* target);   // 0x00843610
};

class CTCActionUseBase {
public:
    // +0x00 vptr (CTCBase; OnActionUse is virtual)
    CThing* ParentThing;                        // +0x04
    virtual void OnActionUse(CThing& user);   // 0x007EAB90 Ego_r.pdb ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z (empty `ret 4`, RTTI slot 28; was OnActionUse(CThingCreatureBase*))
};
class CTCActionUseWallMount : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);   // Ego_r.pdb (UAE; RTTI slot 28, was OnActionUse(CThingCreatureBase*))
};

void CTCActionUseWallMount::OnActionUse(CThing& thing)
{
    CThingCreatureBase* user = (CThingCreatureBase*)&thing;
    CTCActionUseBase::OnActionUse(thing);
    if (IsThingAlive(user)) {
        if (ParentThing->HasTC(TCI_WALL_MOUNT)) {
            CTCWallMount* mount = ((CTCWallMount*)&ParentThing->GetTC(TCI_WALL_MOUNT));
            if (mount) {
                CThing* mounted = mount->GetMountedThing();
                if (IsThingAlive(mounted) && mounted->HasTC(TCI_WALL_MOUNT_EFFECTS)) {
                    CTCWallMountEffects* effects = ((CTCWallMountEffects*)&mounted->GetTC(TCI_WALL_MOUNT_EFFECTS));
                    if (effects->IsUsable() && effects->OnWallMountUseAction(user))
                        return;
                }
            }
        }
        user->SetCurrentAction(CCreatureAction_UseWallMount(user, (CThingPlayerCreature*)ParentThing));
    }
}
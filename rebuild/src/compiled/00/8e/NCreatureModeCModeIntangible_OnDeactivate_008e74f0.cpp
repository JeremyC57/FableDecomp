// NCreatureMode::CModeIntangible::OnDeactivate() -- the creature becomes tangible again: re-enable gravity and finish the base deactivation,
//   make it drawable-off, put it back in the mapwho, re-enable physics and its default interaction flags, make it bloody-willable and
//   targetable, and for a player creature resume its AI, restore its Damageable bit and start the queued on-deactivate action
// Leaf: ?OnDeactivate@CModeIntangible@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E74F0, 258 bytes (padded to 272), `ret`; vtable 0x01268514 slot 6.
// Name: Ego_r.pdb 0x0090F0D0 + FableWin.pdb ?OnDeactivate@CModeIntangible@NCreatureMode@@UAEXXZ (slot 6; slot 5 OnActivate 0x008E7390, slot 3
//   FrameUpdate 0x008E7600, landed).
// Callees: the physics component's virtual slots 38 EnableGravity (+0x98), 36 EnablePhysics (+0x90) and 44 ReturnToDefaultInteractionFlags
//   (+0xB0) (engine/CTCPhysicsBase.h); CThing::SetAsDrawable (slot 28, +0x70); CVectorMap::LowerBound 0x0040F020 (the expanded GetTC of
//   CTCMapwho, type 0x3A, and CTCTargeted, type 9); CTCWillResponse::SetThingAsNotBloodyWillableAtAll 0x007E4040 (static __fastcall, bool in dl);
//   CThingAICreature::SuspendAI 0x00833A10; CThingCreatureBase::SetCurrentAction(const CCountedPointer<CCreatureActionBase>&) 0x00664670;
//   the action's virtual slot 4 (+0x10, FinishAction) when it is refused.
// Layout: Ego_r.pdb NCreatureMode::CModeIntangible (0x44): the NCreatureMode::CModeDragonBase base (0x30: CModeBase ParentThing +4, Active +0x18;
//   Filter +0x1C, PTarget +0x28), then Timer +0x30, BossPhase +0x34, WasDamageable +0x38, PActionOnDeactivate (CCountedPointer<CCreatureActionBase>)
//   +0x3C. No shared header exists for CModeIntangible: it is declared here, on the shared base.
// Shape: the start is the expanded qualified CModeDragonBase::OnDeactivate() call (EnableGravity(true) through the physics slot 38, then
//   `mov byte ptr [esi+18h],0`, the inline CModeBase::OnDeactivate): its one body is inline in engine/CModeDragonBase.h (NF92 opt-in
//   FABLE_RETAIL_CMODEDRAGONBASE_ONDEACTIVATE_INLINE; out-of-line COMDAT 0x008E72B0), on NF90's inline CModeBase::OnDeactivate. The
//   player-creature tail runs on a named thing local (edi survives the calls); the Damageable store is the one-bit bitfield assignment (xor/and/xor).
// One definition (NF92, q11224): the same two statements are CModeDragonBase::OnDeactivate 0x008E72B0, and the sibling OnActivate pair
//   (0x008E7290 / 0x008E7390) opens the same way; a same-TU probe (work/probe_008e74f0_expand.cpp) grades this row RELOCATION_MATCH from the
//   qualified call and 0x008E72B0 MATCH. See NOTES (FableWin.pdb places the original body in the .cpp; the header inline is the repo's spelling).
// Port (NF90, q11187 binder / q11189): was an all-local July model of CThing (v00..v27 placeholders), CTCPhysicsBase, CCreatureActionBase,
//   CThingAICreature, CModeBase, CTCMapwho, CTCTargeted and CTCWillResponse binding 0x00664670 as ?SetCurrentAction@CThing@@...; now on the shared
//   headers (the 0x008D6C80 / 0x008E7E70 define sets) + engine/CModeDragonBase.h (NF90 opt-in FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HITSETTERS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_ONDEACTIVATE_INLINE
#define FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE
#define FABLE_RETAIL_CMODEDRAGONBASE_ONDEACTIVATE_INLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SETINMAP
#define FABLE_RETAIL_CTCTARGETED
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCWillResponse.h"
#include "engine/CModeDragonBase.h"

namespace NCreatureMode {
class CModeIntangible : public CModeDragonBase {
public:
    virtual void OnDeactivate();
protected:
    long Timer;                                                   // +0x30
    long BossPhase;                                               // +0x34
    bool WasDamageable;                                           // +0x38
    CCountedPointer<CCreatureActionBase> PActionOnDeactivate;     // +0x3C
};

void CModeIntangible::OnDeactivate()
{
    CModeDragonBase::OnDeactivate();
    ParentThing.SetAsDrawable(false);
    CTCMapwho* mapwho;
    if (ParentThing.GetTC(&mapwho))
        mapwho->SetInMap(true);
    ParentThing.GetPhysicsTC()->EnablePhysics(true);
    ParentThing.GetPhysicsTC()->ReturnToDefaultInteractionFlags();
    CTCWillResponse::SetThingAsNotBloodyWillableAtAll(&ParentThing, false);
    CTCTargeted* targeted;
    if (ParentThing.GetTC(&targeted))
        targeted->SetTargetable(true);
    CThing& thing = ParentThing;
    if (thing.GetType() == 1) {
        static_cast<CThingAICreature&>(thing).SuspendAI(false, this);
        static_cast<CThingPhysical&>(thing).SetAsDamageable(WasDamageable);
        if (PActionOnDeactivate.Data) {
            if (!static_cast<CThingCreatureBase&>(thing).SetCurrentAction(PActionOnDeactivate))
                PActionOnDeactivate->FinishAction();
        }
    }
}
}

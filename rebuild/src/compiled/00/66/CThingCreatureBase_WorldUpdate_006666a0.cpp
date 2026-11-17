// CThingCreatureBase::WorldUpdate() -- per-frame world update of a creature: refresh the turn speed from the physics component, run the game-object update (stop when it asks to), update the body reorientation, forget the last attacker after 300 seconds, update components 0x77 and 0xCD, and once no pause mode is active start unsheathing the item queued for after the cutscene (then clear it)
// Leaf: ?WorldUpdate@CThingCreatureBase@@UAE_NXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pThingManager (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006666A0, length 383 bytes, plain `ret`, bool in al.
//   Name from Ego_r.pdb `?WorldUpdate@CThingCreatureBase@@UAE_NXZ`.
// Callees: CTCPhysicsBase::GetTurnSpeedAngleXY @ 0x006AFF40, CThingGameObject::WorldUpdate
//   @ 0x00838BA0, CThingCreatureBase::UpdateBodyReorientation @ 0x006622D0, CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50 / SetPItem @ 0x00A01B90 / ctor @ 0x00A01B10 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable
//   0x01238C6C), CThing::PeekWorld @ 0x004C7AB0, CWorld::GetWorldFrame @ 0x0049D870, CThingCreatureBase::
//   ClearAttackedByCreature @ 0x006691C0, LowerBound @ 0x0040F020, NThingShot::CTCShotDefinitionBase::Update
//   @ 0x007C21A0 (component 0x77: its GetInterfaceType 0x004D5472 returns 0x77; Ego_r.pdb
//   ?Update@CTCShotDefinitionBase@NThingShot@@QAEXXZ) and CTCCreatureHitNotification::UpdateHitNotification @ 0x008650E0
//   (component 0xCD: GetInterfaceType 0x004E7FC2 returns 0xCD; Ego_r.pdb ?UpdateHitNotification@CTCCreatureHitNotification@@QAEXXZ),
//   CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase&, CThing&, CCreatureActionBase*, long) @ 0x006A0150 and
//   its folded destructor @ 0x005CC091, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0.
// Globals: CMainGameComponent::ConstantFPS @ 0x01375550, CThing::GetPCurrentThingManager() @ 0x013B8A1C (NonScriptedEntitesPauseMode +0x54,
//   AllEntitesPauseMode +0x55).
// Layout (retail): CThing bitset +0x20 (0x77 = bit 23 of +0x2c, 0xCD = bit 13 of +0x38), map +0x44, PhysicsTC +0x60
//   (PDB +0x70), Dead bit 0 of +0x91; CThingCreatureBase PLastAttackedByCreature +0xf4, WFLastAttackedByCreature +0xfc,
//   TurnSpeed +0x128, PItemToUnsheatheAfterCutscene +0x150 (PDB +0x110/+0x118/+0x14c/+0x178).
// Shape: the world is a named local and the last-attacked frame is read into a local after it (retail loads it into
//   edi between PeekWorld and the static GetWorldFrame); the action is a temporary destroyed before the pointer reset;
//   the CIntelligentPointer converting ctor and operator= are expanded (header opt-ins FORCEINLINE_CTOR / ASSIGN_FORCEINLINE).
// NF66 (round 7): ported onto engine/CThingCreatureBase.h (retail branch; the define block of the landed same-TU row 0x006624A0
//   UpdateLastAttackingCreature, whose body this function expands in place) and the CThing / CThingGameObject / CTCPhysicsBase /
//   CWorld / CMainGameComponent / CThingManager / CCreatureAction_UnsheatheItemFromInventory / NThingShot::CTCShotDefinitionBase
//   headers; every local model of a shared-header class deleted. Bindings now the PDB spellings: ?ClearAttackedByCreature@CThingCreatureBase@@QAEXXZ
//   0x006691C0 (was the made-up ClearLastAttackedByCreature), ??0CCreatureAction_UnsheatheItemFromInventory@@QAE@AAVCThingCreatureBase@@AAVCThing@@PAVCCreatureActionBase@@J@Z
//   0x006A0150 (was by pointer), ?WorldUpdate@CThingGameObject@@UAE_NXZ 0x00838BA0, ?GetTurnSpeedAngleXY@CTCPhysicsBase@@QBEMXZ
//   0x006AFF40, ?UpdateBodyReorientation@CThingCreatureBase@@IAEXXZ 0x006622D0, ?PeekWorld@CThing@@IBEABVCWorld@@XZ 0x004C7AB0,
//   ?ConstantFPS@CMainGameComponent@@1JA 0x01375550 (was g_framesPerSecond). Self: the PDB virtual UAE_NXZ (was QAE_NXZ).
//   CTCCreatureHitNotification has no shared header: the one-member local model below.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_WORLDUPDATE
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY
#define FABLE_RETAIL_CTHINGCREATUREBASE_WORLDUPDATE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_GETTURNSPEEDANGLEXY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_UPDATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"

class CTCCreatureHitNotification {                       // no shared header
public:
    void UpdateHitNotification();                        // 0x008650E0 Ego_r ?UpdateHitNotification@CTCCreatureHitNotification@@QAEXXZ
};

bool CThingCreatureBase::WorldUpdate()
{
    TurnSpeed = PhysicsTC->GetTurnSpeedAngleXY();
    if (CThingGameObject::WorldUpdate())
        return true;
    UpdateBodyReorientation();
    if (PLastAttackedByCreature.GetPItem()) {
        const CWorld& world = PeekWorld();
        unsigned long attacked_frame = WFLastAttackedByCreature;
        unsigned long frames_since = world.GetFrame() - attacked_frame;
        if (frames_since > (unsigned long)(CMainGameComponent::GetConstantFPS() * 300))
            ClearAttackedByCreature();
    }
    NThingShot::CTCShotDefinitionBase* shot;
    if (GetTC(TCI_SHOT_DEFINITION, &shot))
        shot->Update();
    CTCCreatureHitNotification* hit_notification;
    if (GetTC(TCI_CREATURE_HIT_NOTIFICATION, &hit_notification))
        hit_notification->UpdateHitNotification();
    if (PItemToUnsheatheAfterCutscene.GetPItem() && !PItemToUnsheatheAfterCutscene.Get()->IsDead()
        && !GetPCurrentThingManager()->IsNonScriptedEntitesPauseMode() && !GetPCurrentThingManager()->IsAllEntitesPauseMode()) {
        SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory(*this, *PItemToUnsheatheAfterCutscene.Get(), 0, 150));
        PItemToUnsheatheAfterCutscene = CIntelligentPointer<CThing>(0);
    }
    return false;
}

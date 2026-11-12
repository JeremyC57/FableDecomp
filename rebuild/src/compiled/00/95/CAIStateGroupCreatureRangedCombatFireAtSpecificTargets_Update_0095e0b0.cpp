// CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Update() -- while the specific target stays alive: strafe to within 15
// units of it (running, projectile attacks on the way), then aim, fire and reload the missile weapon at it; the state group
// terminates on every path.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095E0B0, 428 bytes; thiscall, no arguments, plain /O2 (no frame pointer, no EH state).
// Vtable 0x0128C894 slot 12. Ego_r.pdb / FableWin.pdb ?Update@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@MAEXXZ.
// Leaf: ?Update@CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@MAEXXZ
// Flags: none
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10;
//   CAIStateGroupBase::AIAction_ProjectileAttackAndStrafeToThingProximity 0x0097EF60; CAIStateGroupBase::
//   AIAction_PerformActionTargetFaceAndWait 0x0097DCB0 (three times); CCreatureAction_AimMissileWeapon ctor 0x00718650;
//   CCreatureAction_FireMissileWeapon ctor 0x008D5110 / dtor 0x0078ADF0; CCreatureAction_ReloadMissileWeapon ctor 0x00718710 /
//   implicit dtor COMDAT 0x00929E60; ~CCreatureActionBase 0x00693EF0; IS_THING_ALIVE<CIntelligentPointer<const CThing> > COMDAT
//   0x0069CE20; CIntelligentPointer<const CThing> copy ctor COMDAT 0x00654D58; CAIStateGroupBase::Terminate 0x008FCF40.
// Shape: every failed step is its own `Terminate(); return;` exit (VC7.1 merges the copies into the one tail block); written
//   as nested ifs falling into one Terminate() the compiler keeps `this` in edi and &PTarget in esi, the reverse of retail.
//   The first two IS_THING_ALIVE tests and the first three by-value pointer copies are expanded inline, and so is the implicit
//   ~CCreatureAction_AimMissileWeapon; after that VC7.1 stops expanding in this function and calls the COMDATs (0x0069CE20,
//   0x00654D58, 0x0078ADF0, 0x00929E60) -- nothing in the source asks for that. The fire result is a named local (`mov ebx,eax`
//   kept across the out-of-line destructor; unnamed, the compiler stores `setne bl`). The 0xCC frame is the bool and one shared
//   0xC8 slot for the three action temporaries. Fire: the target item and the creature are read before the pointer copy.
// CCreatureAction_ReloadMissileWeapon has no engine/ header: modelled here from Ego_r.pdb / FableWin.pdb (ctor, PerformAction,
//   FinishAction, Clone, no declared destructor; PAmmo CIntelligentPointer<CThingObject>, retail +0xA8, sizeof 0xB0).
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_TARGETFACE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PROJECTILESTRAFETHING
#define FABLE_RETAIL_CAISTATEGROUP_CREATURERANGEDCOMBATFIREATSPECIFICTARGETS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_AIMMISSILEWEAPON
#define FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureAction_AimMissileWeapon.h"
#include "engine/CCreatureAction_FireMissileWeapon.h"
class CCreatureAction_ReloadMissileWeapon : public CCreatureActionBase {
public:
    CCreatureAction_ReloadMissileWeapon(CThingCreatureBase& creature);  // 0x00718710
    virtual void PerformAction();
    virtual void FinishAction();
    virtual CCreatureActionBase* Clone() const;
private:
    CIntelligentPointer<CThingObject> PAmmo;                             // +0xa8
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_ReloadMissileWeapon) == 0xb0);
void CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Update()
{
    if (!IS_THING_ALIVE(PTarget)) { Terminate(); return; }
    bool inRange;
    if (AIAction_ProjectileAttackAndStrafeToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PTarget, PTarget, 15.0f, inRange, AI_MOVEMENT_RUNNING, true) != AI_ACTION_FINISHED) { Terminate(); return; }
    if (!IS_THING_ALIVE(PTarget)) { Terminate(); return; }
    if (AIAction_PerformActionTargetFaceAndWait(CCreatureAction_AimMissileWeapon(GetCreature()), PTarget, false) != AI_ACTION_FINISHED) { Terminate(); return; }
    if (!IS_THING_ALIVE(PTarget)) { Terminate(); return; }
    EAIActionReturn fired = AIAction_PerformActionTargetFaceAndWait(CCreatureAction_FireMissileWeapon(GetCreature(), PTarget.Get()), PTarget, false);
    if (fired != AI_ACTION_FINISHED) { Terminate(); return; }
    if (!IS_THING_ALIVE(PTarget)) { Terminate(); return; }
    AIAction_PerformActionTargetFaceAndWait(CCreatureAction_ReloadMissileWeapon(GetCreature()), PTarget, false);
    Terminate();
}
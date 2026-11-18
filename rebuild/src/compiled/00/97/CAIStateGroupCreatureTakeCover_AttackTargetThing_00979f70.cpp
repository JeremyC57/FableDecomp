// CAIStateGroup_CreatureTakeCover::AttackTargetThing() -- draw the ranged weapon, look at the target, aim (into-aim then aim, with a combat
// aiming shout between), and fire while the target stays alive and visible
// Leaf: ?AttackTargetThing@CAIStateGroup_CreatureTakeCover@@AAE_NXZ
// Identity: Ego_r.pdb ?AttackTargetThing@CAIStateGroup_CreatureTakeCover@@AAE_NXZ (private). Retail 0x00979F70, 571 bytes.
// Port (NF68): on the 0x0097A630 block (the cover chain on the shared headers, my overlay opt-ins in hdr/README.md) plus
//   FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON, CWORLD_COMBATMANAGER, CCOMBATMANAGER (+ _UNSHEATH, _DIALOGUE), COMBATDIALOGUEMANAGER
//   (+ _SPEECH), engine/CCombatDialogueDef.h and engine/EUnsheatheWeaponType.h. Every local model of the landed row (intelligent pointers,
//   CCharString / CSoundPair, CThing / CThingCreatureBase, CDefPointer, CCombatDialogueDef, CombatDialogueManager, CCombatManager, CWorld,
//   the actions, CAIBrain, the AI state-group chain) is gone. The aiming speech group is read through the inline by-value GetPDialogueDef
//   into a local before the speech call (retail releases that temporary first). The CIntelligentPointer<const CThing> arguments are built
//   from PTargetThing by the converting constructor 0x0091F990 (nf41:e102, the queued spelling).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TYPED_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TAKECOVER
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_ISRUNNABLELEAF
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_UPDATE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_SETASDIRECTLYAWARE
#define FABLE_RETAIL_CAISTATEGROUPBASE_TARGETFACE
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CCREATUREACTION_FIREMISSILEWEAPON
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
#define FABLE_RETAIL_CCOMBATMANAGER_DIALOGUE
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER_SPEECH
#define FABLE_RETAIL_CCREATUREACTION_PROJECTILECROUCH_CLASS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "rebuild_abi.h"
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/NCreatureMode.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CAIStateGroup_CreatureTakeCover.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/GFRandom.h"
#include "fable_string.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CCreatureAction_FireMissileWeapon.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCombatManager.h"
#include "engine/CombatDialogueManager.h"
#include "engine/CCombatDialogueDef.h"
#include "engine/CCreatureAction_ProjectileCrouch.h"

bool CAIStateGroup_CreatureTakeCover::AttackTargetThing()
{
    CThingAICreature& creature = GetCreature();
    creature.GetCombatManager().CreatureUnsheathWeapon(creature, UNSHEATHE_RANGED_WEAPON);
    if (!IsStillRunnable())
        return false;
    if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PTargetThing, 0x21, 0.01f) != AI_ACTION_FINISHED)
        return false;
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    EAIActionReturn result = AIAction_PerformActionTargetFaceAndWait(
        CCreatureAction_PlayAnimation(GetCreature(), "PROJECTILE_WEAPON_INTO_AIM", CSoundPair(), false, true, false, 0, 0), PTargetThing, false);
    if (result != AI_ACTION_FINISHED) {
        Terminate();
        return false;
    }
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    unsigned long speech_group = GetWorld().GetCombatManager().GetCombatDialogueManager().GetPDialogueDef()->RangedCombatAiming;
    GetWorld().GetCombatManager().GetCombatDialogueManager().SaySpeech(GetCreature(), speech_group);
    result = AIAction_PerformActionTargetFaceAndWait(
        CCreatureAction_PlayAnimation(GetCreature(), "PROJECTILE_WEAPON_AIM", CSoundPair(), false, true, false, 0, 0), PTargetThing, false);
    if (result != AI_ACTION_FINISHED) {
        Terminate();
        return false;
    }
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return false;
    }
    if (!CanSeeTarget())
        return true;
    result = AIAction_PerformActionAndWait(CCreatureAction_FireMissileWeapon(GetCreature(), PTargetThing.Get()), false);
    if (result != AI_ACTION_FINISHED)
        return false;
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return false;
    }
    return true;
}

#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
// Leaf: ?Update@CAIStateGroup_SeekEnemy@@UAEXXZ
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_SEEK_ENEMY_UPDATE_PASS29
#define FABLE_RETAIL_CAISTATEGROUP_SEEKENEMY_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONWANDER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CTHINGCREATUREBASE_FREETOFINISH
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_DIALOGUE
#define FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER_SPEECH
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_NSTEALTH_EDANGERSTATUS
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_NAMED_SQUARED
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/GFRandom.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_SeekEnemy.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCombatDialogueDef.h"
#include "engine/CCombatManager.h"
#include "engine/CWorld.h"
#include "engine/CTCStealth.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/ThingAlive.h"

void CAIStateGroup_SeekEnemy::Update()
{
    if (!IsStillRunnable()) { Terminate(); return; }
    if (GetCreature().IsPerformingAnyAction() && GetCreature().IsFreeToFinishAction())
    GetCreature().FinishCurrentAction();
    const CTCPerceiveThing* ptc_perceive;
    GetCreature().PeekTC(&ptc_perceive);
    bool nav_route_calculated=false;
    bool HasSpoken=false;
    EAIActionReturn ret;
    do {
        if (!IS_THING_ALIVE(PEnemy)) { Terminate(); return; }
        C3DVector last_aware_pos;
        if (PEnemy==ptc_perceive->GetPerceivedThing() && !ptc_perceive->IsThingPerceived())
        last_aware_pos=ptc_perceive->GetAwarenessPosition();
        else {
            last_aware_pos=PEnemy->GetPos();
            if (!HasSpoken) {
                if (GFRandom(100,GetWorld().GetWorldSeed())>50) {
                    unsigned long speech_group_to_say=GetWorld().GetCombatManager().GetCombatDialogueManager().GetPDialogueDef()->EnemySighted;
                    GetWorld().GetCombatManager().GetCombatDialogueManager().SaySpeech(GetCreature(),speech_group_to_say);
                    HasSpoken=true;
                } else HasSpoken=GetWorld().GetCombatManager().GetCombatDialogueManager().PlayBattleCry(GetCreature());
            }
        }
        GetWorld().GetCombatManager().CreatureUnsheathWeapon(GetCreature(),UNSHEATHE_MELEE_WEAPON);
        static float dist_to_attack_from=1.0f;
        ret=AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,last_aware_pos,dist_to_attack_from,nav_route_calculated,AI_MOVEMENT_RUNNING,true,true,true,PEnemy.Get(),false,0.0f);
        if (ret==AI_ACTION_FAILED || !IsStillRunnable()) { Terminate(); return; }
        CTCStealth* ptc_stealth;
        if(PEnemy->GetTC(&ptc_stealth)) ptc_stealth->SetAsInDanger(GetCreature(),NStealth::DANGER_STATUS_CHASING);
    } while(ret==AI_ACTION_IN_PROGRESS);
    if(ret!=AI_ACTION_FINISHED) { Terminate(); return; }
    C3DVector last_aware_pos=ptc_perceive->GetAwarenessPosition();
    ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,last_aware_pos,1,0.125f);
    if (ret==AI_ACTION_TERMINATE_PROCESS || !IsStillRunnable()) { Terminate(); return; }
    const CAnimationEntry& anim_entry=GetCreature().GetRandomAnimFromName("WHATS_THAT",-1);
    do {
        ret=AIAction_PerformAction(CCreatureAction_PlayAnimation(GetCreature(),anim_entry,CSoundPair(0),false,true,false,0,0));
        if (!IsStillRunnable() || ret!=AI_ACTION_FINISHED) { Terminate(); return; }
        if ((ptc_perceive->GetAwarenessPosition()-last_aware_pos).IsNonZero()) { Terminate(); return; }
        ret=AIAction_MinionWander(AI_MOVEMENT_RUNNING,0);
    } while(ret==AI_ACTION_FINISHED || ret==AI_ACTION_IN_PROGRESS);
}
// CAIStateGroup_JackOfBladesStage1::Update() -- update the first Jack of Blades combat stage
// Grade: DIFFER(4612v4624) -- Frame4F8 exact. First nonzero square order and normalization-factor reload; combat-loop squared-magnitude product slots. Tail includes a16-byte switch table; no exact byte/operand audit yet.
// Inline-budget: GET_ROUNDED_ANGLE uses FABLE_FORCEINLINE only here. Plain cost75 exceeds threshold74; T76 also expands the shield pointer ctor; scoped depth255 leaves the cost rejection unchanged.
// Leaf: ?Update@CAIStateGroup_JackOfBladesStage1@@UAEXXZ
// Retail: 0x00975BB0, 4624 bytes, thiscall.
// Identity: both original PDBs; native vtable0128D6B4 slot12; donor02BE6F30.
// Extent: 4606 code bytes, two alignment bytes, sixteen switch-table bytes.
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCBoss.h"
#include "engine/CTCCombat.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CTCWillResponse.h"
#include "engine/CTCJackShield.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCSwitchableNavigation.h"
#include "engine/CTCHitLocations.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/NSpeechGainManager.h"
#include "engine/CTCTalk.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_LeapBack.h"
#include "engine/CCreatureAction_JackShotNear.h"
#include "engine/CCreatureAction_JackShotFar.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/NCreatureMode.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase_GetSubDef.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
// Original donor02BE5F40 forwards the target to NCreatureMode's named query.
inline bool CAIStateGroup_JackOfBladesBase::HasEnemyBeenKnockedDown()
{
    return NCreatureMode::IsKnockedDown(*PTargetThing);
}

// Original donor02BE5100; native971b00, also expanded at the combat-loop latch.
inline bool CAIStateGroup_JackOfBladesBase::IsStillRunnable()
{
    return IS_THING_ALIVE(PTargetThing) && !IsStopping() && IS_THING_ALIVE(PArenaCentre);
}

void CAIStateGroup_JackOfBladesStage1::Update()
{
    long minion_family_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("JACK_BATTLE_PHASE_1_MINIONS_FAMILY");
    long glow_def_num=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_FADE");
    GetWorld().GetEnvironment().TransitionToThemeAllChannels(3,glow_def_num,2.0f);
    CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_jack_def);
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);
    CTCCombat* ptc_combat;
    GetCreature().GetTC(&ptc_combat);
    CTCWillResponse::SetThingIsForcePushable(&GetCreature(),false);
    EAIActionReturn ret=AI_ACTION_FINISHED;
    long num_shot_types=3;
    long normal_shot_type=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES");
    long split_shot_type1=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES2");
    long split_shot_type2=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES3");
    if(!IsStillRunnable()){Terminate();return;}
    GetCreature().GetPhysicsTC()->ResetPosition(PArenaCentre->GetPos());
    if(!IsStillRunnable()){Terminate();return;}
    // Original unused local: FableWin PDB -0x34 and donor02BE777D.
    float forcefield_radius=3.0f;
    CIntelligentPointer<CThingObject> pshield_object;
    if(IS_THING_ALIVE(PShieldThing)) {
        CTCJackShield* ptc_jack_shield;
        if(PShieldThing->GetTC(&ptc_jack_shield)) {
            PShieldThing->GetPhysicsTC()->SetPosition(PArenaCentre->GetPos());
            ptc_jack_shield->EnableShield(true);
        }
    } else {
        long shield_object_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_JACK_OF_BLADES_SHIELD");
        CThingObject* pshield_object_temp=CThingObject::Create(shield_object_index,PArenaCentre->GetPos(),GetCreature().GetPlayerNumber(),0,0,"");
        if(pshield_object_temp) {
            pshield_object=CIntelligentPointer<CThingObject>(pshield_object_temp);
            CTCSolidToThing* ptc_solid_to_thing=static_cast<CTCSolidToThing*>(pshield_object->AddTC("CTCSolidToThing",false,0));
            ptc_solid_to_thing->AddThingNotSolidTo(&GetCreature());
            pshield_object->GetPhysicsTC()->SetPosition(PArenaCentre->GetPos());
            CTCSwitchableNavigation* ptc_switch_nav;
            if(pshield_object->GetTC(&ptc_switch_nav))ptc_switch_nav->SetAsBlockingNavigation(true);
            CTCJackShield* ptc_jack_shield;
            if(pshield_object->GetTC(&ptc_jack_shield))ptc_jack_shield->EnableShield(true);
        }
    }
    CTCHitLocations* p_TC_hit_locations;
    GetCreature().GetTC(&p_TC_hit_locations);
    NHitLocations::CHitLocation* p_hit_location_final=p_TC_hit_locations->GetPHitLocationWithDefName("HIT_LOCATION_JACK_OF_BLADES");
    p_hit_location_final->SetEnabled(false);
    CTCTargeted* ptc_targeted;
    GetCreature().GetTC(&ptc_targeted);
    ptc_targeted->SetTargetable(false);
    GetCreature().GetPhysicsTC()->ResetPosition(PArenaCentre->GetPos()+C3DVector(0,0,0));
    {
        C3DVector vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
        if(vector_to_target.IsNonZero()) {
            vector_to_target=vector_to_target.Normalise();
            GetCreature().GetPhysicsTC()->SetFacingAngleXY(vector_to_target.GetAngleXY());
        }
    }
    if(!IsStillRunnable()){Terminate();return;}
    Summon(minion_family_index,false);
    PowerUpSword();
    if(IS_THING_ALIVE(PShieldThing)) {
        CTCJackShield* ptc_jack_shield;
        if(PShieldThing->GetTC(&ptc_jack_shield))ptc_jack_shield->EnableShield(false);
    } else if(IS_THING_ALIVE(pshield_object)) {
        CTCSwitchableNavigation* ptc_switch_nav;
        if(pshield_object->GetTC(&ptc_switch_nav))ptc_switch_nav->SetAsBlockingNavigation(false);
        CTCJackShield* ptc_jack_shield;
        if(pshield_object->GetTC(&ptc_jack_shield))ptc_jack_shield->EnableShield(false);
        pshield_object->Kill(true);
    }
    p_hit_location_final->SetEnabled(true);
    ptc_targeted->SetTargetable(true);
    CTCWillResponse::SetThingIsForcePushable(&GetCreature(),true);
    float ideal_distance_for_shot=p_jack_def->IdealShootDistance;
    float max_distance_for_shot=p_jack_def->RushTriggerDistance;
    float time_between_shots=p_jack_def->TimeBetweenShots;
    long frame_of_last_shot=GetWorld().GetFrame();
    long shot_counter=0;
    while(IsStillRunnable()&&!ptc_boss->IsReadyForPhaseChange()) {
        C3DVector line_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
        float dis_to_target=line_to_target.GetMagnitude();
        bool moved=false;
        if(dis_to_target>max_distance_for_shot) {
            C3DVector dir_to_target=line_to_target;
            dir_to_target=dir_to_target.Normalise();
            C3DVector rush_to_pos=GetCreature().GetPos()+dir_to_target*p_jack_def->RushToDistance;
            RushToPos(rush_to_pos);
            if(!IsStillRunnable()){Terminate();return;}
        } else if(dis_to_target>ideal_distance_for_shot+3.0f) {
            bool nav;
            ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PTargetThing,ideal_distance_for_shot,nav,(EMovementType)3,true,false,false,false,0.0f);
            if(!IsStillRunnable()){Terminate();return;}
            if(ret!=AI_ACTION_FAILED)moved=true;
        }
        if(ptc_boss->IsReadyForPhaseChange())break;
        long current_frame=GetWorld().GetFrame();
        float time_since_last_shot=(float)(current_frame-frame_of_last_shot)/CMainGameComponent::GetConstantFPS();
        dis_to_target=(GetCreature().GetPos()-PTargetThing->GetPos()).GetMagnitude();
        if(dis_to_target<GetCreature().GetCombatTypeDef().FrontMiddleLimit) {
            if(HasEnemyBeenKnockedDown()) {
                AIAction_PerformActionAndWait(CCreatureAction_LeapBack(GetCreature(),*PTargetThing),false);
            } else {
                MeleeCombat();
                while(GetCreature().IsPerformingAnyAction()) {
                    AIAction_DoNothing();
                    if(!IsStillRunnable()){Terminate();return;}
                }
            }
        } else if(time_since_last_shot>time_between_shots) {
            const CTCPerceiveThing* ptc_perceive_thing;
            GetCreature().PeekTC(&ptc_perceive_thing);
            if(ptc_perceive_thing->IsThingLineOfSight()) {
                AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,35,0.1f);
                if(!IsStillRunnable()){Terminate();return;}
                if(!ptc_boss->IsReadyForPhaseChange()) {
                    switch(shot_counter%num_shot_types) {
                    case 0:ptc_combat->SetShotTypeOverride(normal_shot_type);break;
                    case 1:ptc_combat->SetShotTypeOverride(split_shot_type1);break;
                    case 2:ptc_combat->SetShotTypeOverride(split_shot_type2);break;
                    }
                    if(dis_to_target<ideal_distance_for_shot+3.0f) {
                        if(AIAction_PerformActionAndWait(CCreatureAction_JackShotNear(GetCreature(),*PTargetThing),false)==AI_ACTION_FINISHED) {
                            frame_of_last_shot=GetWorld().GetFrame();++shot_counter;
                        }
                    } else if(dis_to_target<max_distance_for_shot) {
                        if(AIAction_PerformActionAndWait(CCreatureAction_JackShotFar(GetCreature(),*PTargetThing),false)==AI_ACTION_FINISHED) {
                            frame_of_last_shot=GetWorld().GetFrame();++shot_counter;
                        }
                    }
                }
            } else {
                bool nav;
                ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PTargetThing,GetCreature().GetCombatTypeDef().FrontMiddleLimit,nav,(EMovementType)3,true,false,false,false,0.0f);
                if(!IsStillRunnable()){Terminate();return;}
                if(ret!=AI_ACTION_FAILED)moved=true;
            }
        } else if(HasEnemyBeenKnockedDown()) {
            CTCTalk* ptc_talk;
            if(GetCreature().GetTC(&ptc_talk)&&!ptc_talk->IsTalking()) {
                long random_int=SpeechCounter%6;
                switch(random_int) {
                case 0:ptc_talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FIGHT_TAUNT_10",true,(NSpeechGainManager::EDialogueLayer)0,false,true);break;
                case 1:ptc_talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FIGHT_TAUNT_20",true,(NSpeechGainManager::EDialogueLayer)0,false,true);break;
                case 2:ptc_talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FIGHT_TAUNT_30",true,(NSpeechGainManager::EDialogueLayer)0,false,true);break;
                case 3:ptc_talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FIGHT_TAUNT_40",true,(NSpeechGainManager::EDialogueLayer)0,false,true);break;
                }
                ++SpeechCounter;
            }
            AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(),*PTargetThing,"SPECIAL_BOAST",true),false);
        } else if(!moved) {
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,35,0.1f);
        }
    }
    if(ptc_boss->IsReadyForPhaseChange()) {
        if(IsStillRunnable()) {
            CTCWillResponse::SetThingAsNotBloodyWillableAtAll(&GetCreature(),true);
            CThing* p_obstruction_thing=CTCDPhysicalObstruction::Create(PArenaCentre->GetPos(),PArenaCentre->GetPos(),2.5f);
            if(p_obstruction_thing) {
                CTCDPhysicalObstruction* ptcd_obstruction;
                p_obstruction_thing->GetTCDriver(&ptcd_obstruction);
                ptcd_obstruction->SetLifeTimeSeconds(5.0f);
                ptcd_obstruction->SetThingToIgnoreObstruction(&GetCreature());
            }
            RushToPos(PArenaCentre->GetPos());
            if(IsStillRunnable()) {
                C3DVector vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
                if(vector_to_target.IsNonZero())GetCreature().GetPhysicsTC()->SetFacingAngleXY(vector_to_target.GetAngleXY());
            }
        }
        ptc_boss->SetNextBossPhase(1,false);
    }
    Terminate();
}

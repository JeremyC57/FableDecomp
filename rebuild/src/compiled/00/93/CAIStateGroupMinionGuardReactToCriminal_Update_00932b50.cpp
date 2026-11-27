// 00932b50 CAIStateGroup_MinionGuardReactToCriminal::Update
// Leaf: ?Update@CAIStateGroup_MinionGuardReactToCriminal@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_GUARD_CRIMINAL_UPDATE_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUP_MINIONGUARDREACTTOCRIMINAL_VIRTUAL_MODEL
#define FABLE_RETAIL_CAIBRAIN
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_INCREASEHEROENEMYOFGUARDS
#define FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_MinionGuardReactToCriminal.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CTCGuard.h"
#include "engine/NCreatureAI.h"
#include "engine/GFFloatRandom.h"

namespace {
CCharString HEED_STRING("TEXT_AI_GUARD_HEED_WARNING");
CCharString IGNORE_STRING("TEXT_AI_GUARD_IGNORE_WARNING");
CCharString RUN_AWAY_STRING("TEXT_AI_GUARD_RUN_AWAY_WHILE_WARNING");
CCharString SEEN_MINOR_CRIME_STRING("TEXT_AI_GUARD_SEEN_MINOR_CRIME");
CCharString SEEN_MODERATE_CRIME_STRING("TEXT_AI_GUARD_SEEN_MODERATE_CRIME");
CCharString SEEN_SERIOUS_CRIME_STRING("TEXT_AI_GUARD_SEEN_SERIOUS_CRIME");
}

// 00668b60 owns the speed-built pointer constructor. Its 0123e7d8
// vtable selects the size-built deleting destructor at 004d6915.
template<> inline CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase* pointer) { SetPointer(pointer); }
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
template CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase*);
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
void CAIStateGroup_MinionGuardReactToCriminal::Update()
{
    if (IS_THING_ALIVE(PHero)) {
        CTCVillageMember* ptc_village_member;
        GetCreature().GetTC(TCI_VILLAGE_MEMBER, &ptc_village_member);
        CTCVillage* pvillage = ptc_village_member->GetPVillage();
        if (pvillage) {
            long time_since_last_warnings = CWorld::GetFrame() - FrameWarningStarted;
            WarningsGiven += time_since_last_warnings / (CMainGameComponent::GetConstantFPS() * -20);
            if (WarningsGiven <= 0) WarningsGiven = 0;
            FrameWarningStarted = CWorld::GetFrame();
            bool must_not_run_when_warned = false;
            long deed_being_warned = -1;
            CCharString warning_string = pvillage->GetWarningStringForCurrentCrimes(0,must_not_run_when_warned,deed_being_warned);
            if (deed_being_warned != -1 && deed_being_warned != DeedLastWarnedAbout) {
                WarningsGiven=0;
                DeedLastWarnedAbout=deed_being_warned;
            }
            warning_string=pvillage->GetWarningStringForCurrentCrimes(WarningsGiven,must_not_run_when_warned,deed_being_warned);
            CCharString final_string;
            if (warning_string!="") {
                if (IS_THING_ALIVE(pvillage->GetGuardWarningHero())) { Terminate();return; }
                pvillage->SetGuardWarningHero(&GetCreature());
                PositionPlayerWasWhenWarned=PHero->GetPos();
                long frame_for_next_warning=CWorld::GetFrame();
                do {
                    bool finished_calculating_route;
                    EAIActionReturn ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PHero,3.0f,finished_calculating_route,AI_MOVEMENT_RUNNING,true,false,true,true,0.0f);
                    if(ret==AI_ACTION_FINISHED) {
                        C3DVector dir=GetCreature().GetPos()-PHero->GetPos();
                        if(dir.GetMagnitude2D()<2.39f) {
                            dir.Normalise();
                            C3DVector target=PHero->GetPos()+dir*2.4f*2.0f;
                            ret=AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,target,2.4f,finished_calculating_route,AI_MOVEMENT_WALKING,false,false,true,0,true,0.0f);
                        }
                        if(ret==AI_ACTION_FINISHED) ret=AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,PHero,11,0.0027777f);
                    }
                    if(ret==AI_ACTION_TERMINATE_PROCESS) {Terminate();return;}
                    if(must_not_run_when_warned&&NCreatureAI::IsThingRunningAwayFromThing(*PHero,GetCreature(),PositionPlayerWasWhenWarned)) {final_string=RUN_AWAY_STRING;break;}
                    long frames_overdue_from_warning=CWorld::GetFrame()-frame_for_next_warning;
                    if((ret==AI_ACTION_FINISHED&&frames_overdue_from_warning>=0)||frames_overdue_from_warning>CMainGameComponent::GetConstantFPS()*4) {
                        if(warning_string=="") {final_string=IGNORE_STRING;break;}
                        if(AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED,&GetCreature(),PHero,warning_string,true,147)==AI_ACTION_TERMINATE_PROCESS){Terminate();return;}
                        ++WarningsGiven;
                        frame_for_next_warning += CMainGameComponent::GetConstantFPS()*5;
                        frame_for_next_warning += GFFloatRandom(1.0f,GetPWorldSeed())*(CMainGameComponent::GetConstantFPS()*5);
                        deed_being_warned=-1;
                        warning_string=pvillage->GetWarningStringForCurrentCrimes(0,must_not_run_when_warned,deed_being_warned);
                        if(deed_being_warned!=-1 && deed_being_warned!=DeedLastWarnedAbout){WarningsGiven=0;DeedLastWarnedAbout=deed_being_warned;}
                        warning_string=pvillage->GetWarningStringForCurrentCrimes(WarningsGiven,must_not_run_when_warned,deed_being_warned);
                    }
                } while(pvillage->IsHeroCriminal());
                if(!pvillage->IsHeroCriminal()) final_string=HEED_STRING;
            } else {
                CTCGuard* ptc_guard;
                if(GetCreature().GetTC(TCI_GUARD,&ptc_guard)) {
                    long time_since_seen_crime=CWorld::GetFrame()-ptc_guard->GetFrameLastCrimeSeen();
                    if(time_since_seen_crime<CMainGameComponent::GetConstantFPS()*3) {
                        switch(ptc_guard->GetLastCrimeSeenSeverity()) {
                        case CRIME_SEVERITY_MINOR:final_string=SEEN_MINOR_CRIME_STRING;break;
                        case CRIME_SEVERITY_MODERATE:final_string=SEEN_MODERATE_CRIME_STRING;break;
                        case CRIME_SEVERITY_SERIOUS:final_string=SEEN_SERIOUS_CRIME_STRING;break;
                        }
                    }
                }
            }
            if(final_string!="") AIAction_SpeakAISpeechToTarget(ACTION_REPEAT_UNTIL_FINISHED,&GetCreature(),PHero,final_string,true,147);
            if(pvillage->IsHeroCriminal()) pvillage->SetHeroAsEnemyBecauseOfCrime();
        }
    }
    Terminate();
}
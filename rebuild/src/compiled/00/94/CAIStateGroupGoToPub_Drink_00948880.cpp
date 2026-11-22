// CAIStateGroup_GoToPub::Drink()
// Leaf: ?Drink@CAIStateGroup_GoToPub@@IAE_NXZ
// Flags: default
#define FABLE_RETAIL_GOTOPUB_DRINK_PASS27
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB_UPDATE_PASS24
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ACTIVEDIALOGUEINFO
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/CTCTavernTankard.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CTCTalk.h"
#include "engine/NSpeechGainManager.h"
#include "engine/CTCDrunkenness.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCreatureAction_Tankard.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CSoundPair.h"
#include "engine/GFRandom.h"

bool CAIStateGroup_GoToPub::Drink()
{
    if (!IS_THING_ALIVE(PTankard))
        return false;

    CTCTavernTankard* tc_tavern_tankard;
    PTankard->GetTC(&tc_tavern_tankard);
    if (tc_tavern_tankard->Fullness == 0.0f) {
        if (!tc_tavern_tankard->GetBarmaid() && TimeBeforeAskingForBeer <= GetWorld().GetFrame()) {
            static long time_to_next_ask_for_beer = 20;
            TimeBeforeAskingForBeer = GetWorld().GetFrame() + CMainGameComponent::GetConstantFPS() * time_to_next_ask_for_beer;
            CTCTalk* tc_talk;
            GetCreature().GetTC(&tc_talk);
            CTCAIScratchpad* tc_ai_scratchpad;
            GetCreature().GetTC(&tc_ai_scratchpad);
            CTCTalk::ESpeechPlaybackResult ret;
            do {
                ret = tc_talk->PlayAISpeech(tc_ai_scratchpad->GetBeerRequestPhrase(), false, NSpeechGainManager::DIALOGUE_LAYER_BACKGROUND, false, true);
                if (ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                    if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS)
                        return false;
                }
            } while (ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY);
            if (ret == CTCTalk::SPEECH_PLAYBACK_FAILED_FINAL
                || AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "SHOUT_FOR_BEER",
                    CSoundPair(0), false, true, false, 0, 0), false) != AI_ACTION_FINISHED)
                return false;
            return true;
        } else {
            return AIAction_DoNothing() != AI_ACTION_TERMINATE_PROCESS;
        }
    }

    if (AIAction_PerformActionAndWait(CCreatureAction_PickUpTankard(GetCreature(), *PTankard), false) != AI_ACTION_FINISHED) {
        CTCCreatureModeManager* ptc_mode_manager;
        GetCreature().GetTC(&ptc_mode_manager);
        if (ptc_mode_manager->IsModeActive(NCreatureMode::MODE_DRINKING_FROM_TANKARD))
            ptc_mode_manager->RemoveMode(NCreatureMode::MODE_DRINKING_FROM_TANKARD);
        return false;
    }

    long secs_to_drink = GFRandom(9, GetPWorldSeed()) + 2;
    long drink_until = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * secs_to_drink;
    CTCTavernGame* tc_tavern_game = 0;
    PTable->GetTC(&tc_tavern_game);
    do {
        if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
            CTCCreatureModeManager* ptc_mode_manager;
            GetCreature().GetTC(&ptc_mode_manager);
            if (ptc_mode_manager->IsModeActive(NCreatureMode::MODE_DRINKING_FROM_TANKARD))
                ptc_mode_manager->RemoveMode(NCreatureMode::MODE_DRINKING_FROM_TANKARD);
            return false;
        }
    } while (drink_until > CWorld::GetFrame() && (!tc_tavern_game || !tc_tavern_game->IsGameBeingPlayed()));

    tc_tavern_tankard->Fullness -= secs_to_drink * 0.1f;
    CTCDrunkenness* tc_drunkenness;
    GetCreature().GetTC(&tc_drunkenness);
    tc_drunkenness->DrankSecs((float)secs_to_drink);
    if (tc_tavern_tankard->Fullness < 0.0f)
        tc_tavern_tankard->Fullness = 0.0f;

    if (AIAction_PerformActionAndWait(CCreatureAction_PutDownTankard(GetCreature()), false) != AI_ACTION_FINISHED) {
        CTCCreatureModeManager* ptc_mode_manager;
        GetCreature().GetTC(&ptc_mode_manager);
        if (ptc_mode_manager->IsModeActive(NCreatureMode::MODE_DRINKING_FROM_TANKARD))
            ptc_mode_manager->RemoveMode(NCreatureMode::MODE_DRINKING_FROM_TANKARD);
        return false;
    }
    return true;
}
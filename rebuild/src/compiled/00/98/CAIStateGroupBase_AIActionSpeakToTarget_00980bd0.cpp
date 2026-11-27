// 00980bd0 CAIStateGroupBase::AIAction_SpeakToTarget
// Leaf: ?AIAction_SpeakToTarget@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingCreatureBase@@@@V?$CIntelligentPointer@$$CBVCThing@@@@K_NJ3@Z
// Flags: none
#define FABLE_RETAIL_AI_SPEECH_CONST_ALIVE_PASS29
#define FABLE_RETAIL_AI_SPEECH_COPY_INLINE_PASS29
#define FABLE_RETAIL_AI_SPEECH_ENUM_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_PLAYSPEECH_OVERLOADS
#define FABLE_RETAIL_NCREATUREAI_SPEAKING
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_FREETOFINISH
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ACTIVEDIALOGUEINFO
#include "engine/fable_ai_action_enums.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/NCreatureAI.h"
#include "engine/ThingAlive.h"
#include "engine/NSpeechGainManager.h"
#include "engine/CTCTalk.h"

EAIActionReturn CAIStateGroupBase::AIAction_SpeakToTarget(EAIActionRepeat type, CIntelligentPointer<CThingCreatureBase> speaker, CIntelligentPointer<const CThing> speakee, unsigned long text, bool speak_over_speakee, long look_flags, bool is_ai_speech)
{
    EAIActionReturn ret = AI_ACTION_IN_PROGRESS;
    if (type == ACTION_REPEAT_UNTIL_FINISHED) {
        if (IS_THING_ALIVE(speakee) && speakee->GetType() == 2 && GetCreature().IsFreeToFinishAction())
            GetCreature().FinishCurrentAction();
        if (IS_THING_ALIVE(speakee)) {
            ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, speakee, look_flags | 0x10, 0.0027777f);
            if (ret != AI_ACTION_IN_PROGRESS && ret != AI_ACTION_FINISHED)
                return ret;
        }
        while (IS_THING_ALIVE(speakee) && !speak_over_speakee && speakee->GetType() == 1 && NCreatureAI::IsSpeaking(*GFDebugPDynamicCast(speakee.Get(), (const CThingAICreature*)0))) {
            ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, speakee, look_flags | 0x10, 0.0027777f);
            if (ret != AI_ACTION_IN_PROGRESS && ret != AI_ACTION_FINISHED)
                return ret;
        }
        CTCTalk* tc_talk;
        GetCreature().GetTC(&tc_talk);
        for (;;) {
            CTCTalk::ESpeechPlaybackResult result = is_ai_speech ? tc_talk->PlayAISpeech(text, false, NSpeechGainManager::DIALOGUE_LAYER_MIDGROUND, false, true) : tc_talk->PlaySpeech(text, NSpeechGainManager::DIALOGUE_LAYER_FOREGROUND, false, true);
            if (result == CTCTalk::ESPR_WAITING) {
                PBrain->SuspendSuspendableProcess();
                if (IsStopping()) {
                    if (IS_THING_ALIVE(speaker))
                        NCreatureAI::StopSpeaking(*speaker);
                    return AI_ACTION_TERMINATE_PROCESS;
                }
                continue;
            }
            if (result == CTCTalk::ESPR_FAIL)
                return AI_ACTION_FAILED;
            break;
        }
        while (NCreatureAI::IsSpeaking(GetCreature()) && IS_THING_ALIVE(speakee)) {
            ret = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, speakee, look_flags | 0x10, 0.0027777f);
            if (ret != AI_ACTION_IN_PROGRESS && ret != AI_ACTION_FINISHED) {
                NCreatureAI::StopSpeaking(GetCreature());
                return ret;
            }
        }
        return AI_ACTION_FINISHED;
    }
    if (IsStopping()) {
        if (IS_THING_ALIVE(speaker))
            NCreatureAI::StopSpeaking(*speaker);
        return AI_ACTION_TERMINATE_PROCESS;
    }
    do {
        if (!NCreatureAI::IsSpeaking(GetCreature())) {
            if (IS_THING_ALIVE(speakee) && !speak_over_speakee && speakee->GetType() == 1 && NCreatureAI::IsSpeaking(*GFDebugPDynamicCast(speakee.Get(), (const CThingAICreature*)0))) {
                PBrain->SuspendSuspendableProcess();
                ret = AI_ACTION_IN_PROGRESS;
                break;
            }
            CTCTalk* tc_talk = 0;
            GetCreature().GetTC(&tc_talk);
            CTCTalk::ESpeechPlaybackResult result = is_ai_speech ? tc_talk->PlayAISpeech(text, false, NSpeechGainManager::DIALOGUE_LAYER_MIDGROUND, false, true) : tc_talk->PlaySpeech(text, NSpeechGainManager::DIALOGUE_LAYER_FOREGROUND, false, true);
            if (result == CTCTalk::ESPR_FAIL)
                return AI_ACTION_FAILED;
            else if (result == CTCTalk::ESPR_WAITING) {
                PBrain->SuspendSuspendableProcess();
                ret = AI_ACTION_IN_PROGRESS;
                break;
            }
        } else if (IS_THING_ALIVE(speakee))
            ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, speakee, look_flags | 0x10, 0.0027777f);
        if (IsStopping()) {
            if (IS_THING_ALIVE(speaker))
                NCreatureAI::StopSpeaking(*speaker);
            return AI_ACTION_TERMINATE_PROCESS;
        }
        if (!NCreatureAI::IsSpeaking(GetCreature()))
            ret = AI_ACTION_FINISHED;
    } while (type == ACTION_REPEAT_UNTIL_FINISHED && ret == AI_ACTION_IN_PROGRESS);
    return ret;
}
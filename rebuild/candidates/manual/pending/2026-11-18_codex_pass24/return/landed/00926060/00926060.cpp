// CAIStateGroup_WifeGiveYouGift::Update() -- Approach the hero and repeat the gift speech when the timer expires.
// Retail: 00926060, 386 bytes, thiscall.
// Leaf: ?Update@CAIStateGroup_WifeGiveYouGift@@UAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_MANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_WIFE_GIFT_UPDATE_PASS24
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/ThingAlive.h"
#include "engine/CIntelligentPointer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_WifeGiveYouGift.h"
#include "engine/CTCWife.h"
#include "engine/CTCTalk.h"

// Ego_r and FableWin name the TU-local object. Its native initializer at
// 01221980 constructs it from the wife gift speech key.
namespace { CCharString TEXT_TO_SAY("TEXT_AI_WIFE_GIFT_GIVE"); }

void CAIStateGroup_WifeGiveYouGift::Update()
{
    DeliveredGiveSpeech = false;
    CTCWife* ptc_wife;
    if (GetCreature().GetTC(TCI_WIFE, &ptc_wife)) {
        CThingPlayerCreature* pplayer = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(pplayer)) {
            CTCTalk* tc_talk = 0;
            GetCreature().GetTC(TCI_TALK, &tc_talk);
            const unsigned long text_index = CMainGameComponent::Get()->GetTextBank().FindIndexBySymbolCRC(TEXT_TO_SAY.GetCRC());
            bool finished_calulating_route;
            EAIActionReturn ret;
            do {
                ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, pplayer, 2.0f, finished_calulating_route, AI_MOVEMENT_WALKING, true, false, true, true, 0.0f);
                if (FrameToSpeakNext <= CWorld::GetFrame()) {
                    if (tc_talk && text_index > 0) {
                        CTCTalk::ESpeechPlaybackResult talk_ret;
                        do {
                            talk_ret = tc_talk->PlayAISpeech(text_index, false, (NSpeechGainManager::EDialogueLayer)1, false, true);
                            if (talk_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY && AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                                Terminate();
                                return;
                            }
                        } while (talk_ret == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY);
                    }
                    DeliveredGiveSpeech = true;
                    FrameToSpeakNext = CWorld::GetFrame() + 10 * CMainGameComponent::GetConstantFPS();
                }
            } while (ret == AI_ACTION_IN_PROGRESS);
        }
    }
    Terminate();
}

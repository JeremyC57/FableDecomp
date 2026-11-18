#include <stdio.h>
#include <stdlib.h>
#include "engine/fable_ai_action_enums.h"

static void check(bool b) { if (!b) abort(); }
enum ComponentId { TCI_WIFE, TCI_TALK };
class CTCWife {};
class CThingPlayerCreature { public: bool dead; };
static bool IS_THING_ALIVE(const CThingPlayerCreature* p) { return p && !p->dead; }
namespace NSpeechGainManager { enum EDialogueLayer { Silent, Normal }; }
class CTCTalk {
public:
    enum ESpeechPlaybackResult { SPEECH_PLAYBACK_STARTED, SPEECH_PLAYBACK_FAILED_FINAL, SPEECH_PLAYBACK_FAILED_RETRY };
    int plays, retry_count;
    bool final_failure;
    ESpeechPlaybackResult PlayAISpeech(unsigned long key,bool force,NSpeechGainManager::EDialogueLayer layer,bool x,bool y) {
        check(key==33 && !force && layer==NSpeechGainManager::Normal && !x && y);
        return plays++ < retry_count ? SPEECH_PLAYBACK_FAILED_RETRY : final_failure ? SPEECH_PLAYBACK_FAILED_FINAL : SPEECH_PLAYBACK_STARTED;
    }
};
class Creature { public:
    CTCWife* wife;
    CTCTalk* talk;
    bool GetTC(ComponentId id, CTCWife** out) {check(id==TCI_WIFE);*out=wife;return wife!=0;}
    bool GetTC(ComponentId id, CTCTalk** out) {check(id==TCI_TALK);*out=talk;return talk!=0;}
};
class CPlayerManager { public:
    CPlayerManager& GetMainPlayer(){return *this;}
    CThingPlayerCreature* controlled;
    CThingPlayerCreature* GetPControlledCreature(){return controlled;}
};
class CWorld { public: static long frame; static long GetFrame(){return frame;} };
long CWorld::frame;
struct SpeechKey { unsigned long GetCRC()const{return 5641;} };
namespace { SpeechKey TEXT_TO_SAY; }
class CMainGameComponent { public:
    unsigned long key; int lookups;
    static CMainGameComponent state;
    static CMainGameComponent* Get(){return &state;}
    CMainGameComponent& GetTextBank(){return *this;}
    unsigned long FindIndexBySymbolCRC(unsigned long crc){check(crc==5641);++lookups;return key;}
    static long GetConstantFPS(){return 8;}
};
CMainGameComponent CMainGameComponent::state;
class CAIStateGroup_WifeGiveYouGift { public:
    Creature creature; CPlayerManager players;
    long FrameToSpeakNext; bool DeliveredGiveSpeech;
    int moves,waits,exits,pending_moves; bool interrupt;
    Creature& GetCreature(){return creature;}
    CPlayerManager& GetPlayerManager(){return players;}
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat r,CThingPlayerCreature* p,float f,bool& route,EMovementType m,bool x,bool y,bool z,bool w,float t){
        check(r==ACTION_REPEAT_CALLBACK && p==players.controlled && f==2.f && m==AI_MOVEMENT_WALKING && x && !y && z && w && t==0.f);
        route=true; ++CWorld::frame; return moves++ < pending_moves ? AI_ACTION_IN_PROGRESS : AI_ACTION_FINISHED;
    }
    EAIActionReturn AIAction_DoNothing(){++waits;return interrupt ? AI_ACTION_TERMINATE_PROCESS : AI_ACTION_IN_PROGRESS;}
    void Terminate(){++exits;}
    void Update();
};

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

int main() {
    for (int c=0;c<12;++c) {
        CAIStateGroup_WifeGiveYouGift action; CTCWife wife; CThingPlayerCreature hero; CTCTalk talk;
        talk.plays=0; talk.retry_count=(c==8 || c==9)?2:0; talk.final_failure=c==10;
        action.creature.wife=c==0?0:&wife; action.creature.talk=c==4?0:&talk;
        hero.dead=c==2; action.players.controlled=c==1?0:&hero;
        action.pending_moves=c==7?2:c==11?1:0;
        action.moves=action.waits=action.exits=0; action.interrupt=c==9;
        action.DeliveredGiveSpeech=true; action.FrameToSpeakNext=c==3?200:c==11?102:101;
        const long prior=action.FrameToSpeakNext;
        CMainGameComponent::state.key=c==5?0:33; CMainGameComponent::state.lookups=0; CWorld::frame=100;
        action.Update();
        check(action.exits==1);
        check(action.moves==(c<=2?0:1+action.pending_moves));
        check(CMainGameComponent::state.lookups==(c<=2?0:1));
        check(action.waits==(c==9?1:c==8?2:0));
        check(talk.plays==(c<=5?0:c==8?3:1));
        bool delivered=c>3 && c!=9;
        check(action.DeliveredGiveSpeech==delivered);
        check(action.FrameToSpeakNext==(delivered?(c==11?182:181):prior));
    }
    puts("00926060_TEST PASS");
    return 0;
}

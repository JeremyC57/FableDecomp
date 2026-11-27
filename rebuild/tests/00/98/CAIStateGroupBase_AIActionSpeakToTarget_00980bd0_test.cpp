#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool value){if(!value) exit(1);}
enum EAIActionRepeat {ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK};
enum EAIActionReturn {AI_ACTION_NULL,AI_ACTION_TERMINATE_PROCESS,AI_ACTION_FINISHED,AI_ACTION_FAILED,AI_ACTION_IN_PROGRESS};
namespace NSpeechGainManager {enum EDialogueLayer {DIALOGUE_LAYER_FOREGROUND,DIALOGUE_LAYER_MIDGROUND};}
struct Scenario {int look_at,type,target,free,over,ai,look,play,stopping,speaker,speaking,target_speech;} cfg;
struct Counters {int look,play,stop,own,target;char events[128];int n;} observed,expected;
static void event(char c){check(observed.n<126);observed.events[observed.n++]=c;}
struct CTCTalk {
 enum ESpeechPlaybackResult {ESPR_OK,ESPR_FAIL,ESPR_WAITING};
 ESpeechPlaybackResult play(){int a=observed.play++;return a==0?(ESpeechPlaybackResult)cfg.play:ESPR_OK;}
 ESpeechPlaybackResult PlayAISpeech(unsigned long text,bool f,NSpeechGainManager::EDialogueLayer l,bool a,bool b){check(text==13579&&!f&&l==NSpeechGainManager::DIALOGUE_LAYER_MIDGROUND&&!a&&b);event('A');return play();}
 ESpeechPlaybackResult PlaySpeech(unsigned long text,NSpeechGainManager::EDialogueLayer l,bool a,bool b){check(text==13579&&l==NSpeechGainManager::DIALOGUE_LAYER_FOREGROUND&&!a&&b);event('P');return play();}
} talk;
struct CThing {int type;bool dead;unsigned long GetType()const{return type;}};
struct CThingCreatureBase:CThing {bool IsFreeToFinishAction(){event('F');return cfg.free!=0;}void FinishCurrentAction(){event('D');}bool GetTC(CTCTalk** out){event('G');*out=&talk;return true;}};
struct CThingAICreature:CThingCreatureBase{} own,target,speaker;
template<class T>struct CIntelligentPointer {T* p;CIntelligentPointer(T* a):p(a){}T* Get()const{return p;}T* operator->()const{return p;}T& operator*()const{return *p;}};
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>& p){return p.Get()&&!p->dead;}
template<class From,class To>To GFDebugPDynamicCast(From p,To){return static_cast<To>(p);}
namespace NCreatureAI {
 bool IsSpeaking(const CThingCreatureBase& c){if(&c==&target){event('T');return observed.target++<cfg.target_speech;}check(&c==&own);event('O');return observed.own++<cfg.speaking;}
 void StopSpeaking(CThingCreatureBase& c){check(&c==&own||&c==&speaker);event(&c==&own?'Q':'S');}
}
struct CAIBrain {virtual bool SuspendSuspendableProcess(){event('Y');return true;}} brain;
struct CAIStateGroupBase {
 CAIBrain* PBrain;CThingAICreature& GetCreature(){return own;}
 bool IsStopping(){event('X');return ++observed.stop==cfg.stopping;}
 EAIActionReturn AIAction_LookAtThing(EAIActionRepeat t,CIntelligentPointer<const CThing> p,long flags,float speed){check(t==cfg.type&&p.Get()==&target&&flags==0x93&&speed==0.0027777f);event('L');return observed.look++==cfg.look_at?(EAIActionReturn)cfg.look:AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_SpeakToTarget(EAIActionRepeat,CIntelligentPointer<CThingCreatureBase>,CIntelligentPointer<const CThing>,unsigned long,bool,long,bool);
};
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
static void e(char c){check(expected.n<126);expected.events[expected.n++]=c;}
static bool stops(){e('X');return ++expected.stop==cfg.stopping;}
static bool ownTalk(){e('O');return expected.own++<cfg.speaking;}
static bool targetTalk(){e('T');return expected.target++<cfg.target_speech;}
static int look(){e('L');return expected.look++==cfg.look_at?cfg.look:2;}
static int play(){e(cfg.ai?'A':'P');return expected.play++==0?cfg.play:0;}
static int terminate(){if(cfg.speaker==2)e('S');return 1;}
static int model(){
 bool alive=cfg.target>=2;int r=4;
 if(cfg.type==1){
  if(cfg.target==3){e('F');if(cfg.free)e('D');}
  if(alive){r=look();if(r!=2&&r!=4)return r;}
  while(cfg.target==2&&!cfg.over&&targetTalk()){r=look();if(r!=2&&r!=4)return r;}
  e('G');
  for(;;){int q=play();if(q==1)return 3;if(q==0)break;e('Y');if(stops())return terminate();}
  while(ownTalk()&&alive){r=look();if(r!=2&&r!=4){e('Q');return r;}}
  return 2;
 }
 if(stops())return terminate();
 if(ownTalk()){if(alive)r=look();}
 else{
  if(cfg.target==2&&!cfg.over&&targetTalk()){e('Y');return 4;}
  e('G');int q=play();if(q==1)return 3;if(q==2){e('Y');return 4;}
 }
 if(stops())return terminate();
 return ownTalk()?r:2;
}
int main(){
 CAIStateGroupBase group;group.PBrain=&brain;unsigned long count=0;
 for(cfg.type=1;cfg.type<=2;++cfg.type)
 for(cfg.target=0;cfg.target<5;++cfg.target)
 for(cfg.free=0;cfg.free<2;++cfg.free)
 for(cfg.over=0;cfg.over<2;++cfg.over)
 for(cfg.ai=0;cfg.ai<2;++cfg.ai)
 for(cfg.look_at=0;cfg.look_at<3;++cfg.look_at)
 for(cfg.look=0;cfg.look<5;++cfg.look)
 for(cfg.play=0;cfg.play<3;++cfg.play)
 for(cfg.stopping=0;cfg.stopping<3;++cfg.stopping)
 for(cfg.speaker=0;cfg.speaker<3;++cfg.speaker)
 for(cfg.speaking=0;cfg.speaking<3;++cfg.speaking)
 for(cfg.target_speech=0;cfg.target_speech<2;++cfg.target_speech){
  memset(&observed,0,sizeof observed);memset(&expected,0,sizeof expected);
  target.dead=cfg.target==1;target.type=cfg.target==2?1:cfg.target==3?2:3;
  speaker.dead=cfg.speaker==1;
  int a=group.AIAction_SpeakToTarget((EAIActionRepeat)cfg.type,cfg.speaker?&speaker:0,cfg.target?&target:0,13579,cfg.over!=0,0x83,cfg.ai!=0);
  int b=model();if(a!=b||strcmp(observed.events,expected.events)){printf("case %lu results %d %d events %s %s\n",count,a,b,observed.events,expected.events);return 3;}++count;
 }check(count==194400UL);puts("00980bd0_TEST PASS");return 0;
}
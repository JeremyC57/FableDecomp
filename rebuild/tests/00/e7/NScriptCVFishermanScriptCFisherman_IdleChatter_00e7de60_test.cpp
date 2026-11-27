#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool ok){if(!ok) exit(2);}
struct CScriptThing{int id;};
struct State{
 char steps[80];int used;int distance,initial,delay,task,stop,speech;
 void mark(char c){check(used<79);steps[used++]=c;steps[used]=0;}
} run;
struct TRetVoid{void Return(){run.mark('F');}};
struct Interface{
 CScriptThing hero;
 CScriptThing& GetHero(){return hero;}
 long GetTimer(long i){check(i==51);run.mark('t');return run.initial;}
 void SetTimer(long i,long t){check(i==51 && t==run.delay);run.mark('s');}
 long AddNewConversation(const CScriptThing& t,bool a,bool b){check(t.id==1&&!a&&!b);run.mark('c');return -73;}
 void AddPersonToConversation(long id,const CScriptThing& h){check(id==-73 && h.id==2);run.mark('p');}
 const char* GetValidTextEntryNameWithAttitude(const CScriptThing& t,const char* n){
  check(t.id==1);run.mark('n');
  if(!strcmp(n,"TEXT_QST_023_IDLE"))run.speech=4;else if(!strcmp(n,"TEXT_QST_023_TUTORIAL_IDLE"))run.speech=2;else exit(3);
  return "selected";
 }
 void AddLineToConversation(long id,const char* text,bool a,const CScriptThing& t,const CScriptThing& h){
  check(id==-73&&!strcmp(text,"selected")&&!a&&t.id==1&&h.id==2);run.mark('l');
 }
};
struct CTimer{
 long TimerIndex;
 CTimer& operator=(long time);
 bool operator>(long time) const;
};
struct ScriptDef{long F_FishermanIdleChatDelay;};
static Interface* pDSTGame;static ScriptDef* pGlobalScriptDef;
static bool IsDistanceBetweenThingsOver(const CScriptThing& one,const CScriptThing& two,float threshold){
 check(one.id==1&&two.id==2&&threshold==8.0f);run.mark('d');return run.distance>threshold;
}
namespace NScript{
struct CV_FishermanScript{
 long FishermanTasks;
 struct CFisherman{
  CScriptThing Me;CTimer ChatterTimer;CV_FishermanScript* ParentClass;Interface* Game;
  bool IsActiveThreadTerminating()const{run.mark('q');return run.stop!=0;}
  void IdleChatter();
 };
};}
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline bool CTimer::operator>(long time) const { return pDSTGame->GetTimer(TimerIndex)>time; }
void NScript::CV_FishermanScript::CFisherman::IdleChatter()
{
    TRetVoid _Fail_Val;
    if (IsDistanceBetweenThingsOver(Me, Game->GetHero(), 8.0f)) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        return;
    }
    if (ChatterTimer > 0) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        return;
    }
    ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
    long Conversation = Game->AddNewConversation(Me,false,false);
    Game->AddPersonToConversation(Conversation,Game->GetHero());
    switch(ParentClass->FishermanTasks) {
    case 4: {
        Game->AddLineToConversation(Conversation,Game->GetValidTextEntryNameWithAttitude(Me,"TEXT_QST_023_IDLE"),false,Me,Game->GetHero());
        break;
    }
    case 2: {
        Game->AddLineToConversation(Conversation,Game->GetValidTextEntryNameWithAttitude(Me,"TEXT_QST_023_TUTORIAL_IDLE"),false,Me,Game->GetHero());
        break;
    }
    }
}

int main(){
 Interface game;ScriptDef def;game.hero.id=2;pDSTGame=&game;pGlobalScriptDef=&def;
 NScript::CV_FishermanScript owner;NScript::CV_FishermanScript::CFisherman fisher;
 fisher.ParentClass=&owner;fisher.Me.id=1;fisher.Game=&game;fisher.ChatterTimer.TimerIndex=51;
 for(run.distance=7;run.distance<=9;++run.distance)
 for(run.initial=-1;run.initial<=1;++run.initial)
 for(run.delay=-3;run.delay<=3;++run.delay)
 for(run.task=-1;run.task<=6;++run.task)
 for(run.stop=0;run.stop<=1;++run.stop){
  run.used=run.speech=0;run.steps[0]=0;def.F_FishermanIdleChatDelay=run.delay;owner.FishermanTasks=run.task;
  fisher.IdleChatter();
  const char* expected=run.distance>8?(run.stop?"dqF":"dq"):
   run.initial>0?(run.stop?"dtqF":"dtq"):
   (run.task==2||run.task==4)?"dtscpnl":"dtscp";
  check(!strcmp(expected,run.steps));
  check(run.speech==(run.distance<=8 && run.initial<=0 && (run.task==2||run.task==4)?run.task:0));
 }
 puts("00e7de60_TEST PASS");return 0;
}
#include <stdio.h>
#include <string.h>
enum EPlayerMode { PLAYER_MODE_Z_TARGET=2 };
enum EGameAction { GAME_ACTION_LOCK_TARGET=1 };
static char trace[16];static int at,failures;
static void step(char c){trace[at++]=c;trace[at]=0;}
class CInputEvent {public:bool held,pressed;int action;};
static const CInputEvent* want_event;
class CGameEvent {public:long Type;signed char Player;unsigned char Data[32];unsigned char EndPos;bool Valid,Replacement;
 CGameEvent(long type):Type(type),Player(-1),EndPos(0),Valid(true),Replacement(false){}
};
class CProcessedInput {public:int added;void AddGameEvent(const CGameEvent& e){step('E');++added;if(e.Type!=0x15||e.Player!=-1||e.EndPos!=0||!e.Valid||e.Replacement)++failures;}};
class CPlayer {public:int mode;EPlayerMode GetCurrentMode()const{step('M');return (EPlayerMode)mode;}};
class CGamePlayerInterface {public:bool toggle;bool IsToggleZTarget()const{step('T');return toggle;}};
class CInputProcessGameBase {public:CPlayer* MyPlayer;CGamePlayerInterface* PGamePlayerInterface;
 bool IsEventGameAction(EGameAction a,const CInputEvent& e)const{step('A');if(&e!=want_event)++failures;return e.action==a;}
 protected:const CGamePlayerInterface& GetGamePlayerInterface()const{return *PGamePlayerInterface;}
 bool IsEventButtonHeld(const CInputEvent& e)const{step('H');if(&e!=want_event)++failures;return e.held;}
 bool IsEventButtonPressed(const CInputEvent& e)const{step('P');if(&e!=want_event)++failures;return e.pressed;}
};
class CInputProcessControlCreatureActivateZTarget:public CInputProcessGameBase {public:void ProcessInput(const CInputEvent&,CProcessedInput&);};
void CInputProcessControlCreatureActivateZTarget::ProcessInput(const CInputEvent& event,CProcessedInput& processed_input)
{
    if(MyPlayer->GetCurrentMode()==PLAYER_MODE_Z_TARGET)return;
    bool activate=false;
    if(GetGamePlayerInterface().IsToggleZTarget()) {
        if(IsEventButtonHeld(event))activate=true;
    } else {
        if(IsEventButtonPressed(event))activate=true;
    }
    if(activate && IsEventGameAction(GAME_ACTION_LOCK_TARGET,event)) {
        CGameEvent action(0x15);
        processed_input.AddGameEvent(action);
    }
}
int main(){
 for(int mode=0;mode<48;++mode)for(int toggle=0;toggle<2;++toggle)for(int h=0;h<2;++h)for(int p=0;p<2;++p)for(int action=0;action<3;++action){
  CPlayer player;player.mode=mode;CGamePlayerInterface game;game.toggle=toggle!=0;
  CInputProcessControlCreatureActivateZTarget row;row.MyPlayer=&player;row.PGamePlayerInterface=&game;
  CInputEvent event;event.held=h!=0;event.pressed=p!=0;event.action=action;want_event=&event;
  CProcessedInput processed;processed.added=0;at=0;trace[0]=0;row.ProcessInput(event,processed);
  bool selected=toggle?h!=0:p!=0;bool expected=mode!=2&&selected&&action==1;
  char wanted[16]="M";if(mode!=2){strcat(wanted,toggle?"TH":"TP");if(selected){strcat(wanted,"A");if(action==1)strcat(wanted,"E");}}
  if(processed.added!=(expected?1:0)||strcmp(trace,wanted)||failures||player.mode!=mode||event.action!=action)return 1;
 }
 puts("00688260_TEST PASS");return 0;
}
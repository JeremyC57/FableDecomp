#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <vector>
struct C3DVector {
 float X,Y,Z;
 C3DVector() {}
 bool operator!=(const C3DVector& v) const {return fabs(X-v.X)>0.0001f || fabs(Y-v.Y)>0.0001f || fabs(Z-v.Z)>0.0001f;}
};
static int checks,terminateAt,frames,timerCount,baseIndex;
static long commentaryTime;
static std::vector<long> released,queried;
class CScriptThing {public:const C3DVector& GetPos() const {assert(false);static C3DVector unreachable;return unreachable;}};
class CGameScriptInterfaceBase {
public:
 long RegisterTimer() {return ++timerCount;}
 void DeregisterTimer(long index) {released.push_back(index);}
 void SetTimer(long,long) {assert(false);}
 long GetTimer(long index) const {queried.push_back(index);return index==baseIndex?commentaryTime:0;}
 void NewScriptFrame() {++frames;}
 bool IsPlayerWieldingWeapon() const {assert(false);return false;}
 CScriptThing& GetHero() {assert(false);static CScriptThing unreachable;return unreachable;}
};
CGameScriptInterfaceBase* pDSTGame;
class CTimer {
public:
 CTimer();~CTimer();CTimer& operator=(long);long GetTime() const;long TimerIndex;
};
class CCounter {public:CCounter& operator=(float) {assert(false);return *this;}};
namespace NScript {
struct CQ_SunnyvaleMasterData {long HenchmanCommentaryState;};
class CV_BodyGuardScript {
public:
 CGameScriptInterfaceBase* Game;CQ_SunnyvaleMasterData* MasterScriptData;
 std::vector<CScriptThing> HenchmanList;
 bool HeroWieldingWeapon;CCounter HeroStoodStillCounter;CTimer CommentaryTimer;
 bool IsActiveThreadTerminating() {return ++checks==terminateAt;}
 bool MakeHenchmanPerformIdleComment() {assert(false);return false;}
 void HenchmanCommentary();
};
}
class TRetVoid { public: void Return() { } };
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
void NScript::CV_BodyGuardScript::HenchmanCommentary()
{
    CTimer IdleConversationTimer;
    CTimer CheckExpensiveThingsTimer;
    TRetVoid _Fail_Val;
    C3DVector HeroLastPosition;
    while (!IsActiveThreadTerminating()) {
        while (HenchmanList.size() == 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (MasterScriptData->HenchmanCommentaryState && CommentaryTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            MasterScriptData->HenchmanCommentaryState = 0;
        }
        if (CheckExpensiveThingsTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CheckExpensiveThingsTimer=1;
            HeroWieldingWeapon = Game->IsPlayerWieldingWeapon();
        }
        if (HeroLastPosition != Game->GetHero().GetPos()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            HeroStoodStillCounter = 0.0f;
        }
        HeroLastPosition = Game->GetHero().GetPos();
        if (IdleConversationTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (MakeHenchmanPerformIdleComment()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                IdleConversationTimer=5;
            }
        }
        Game->NewScriptFrame();
    }
}

int main() {
 CGameScriptInterfaceBase game;pDSTGame=&game;unsigned cases=0;
 for(int empty=0;empty!=2;++empty)for(int state=0;state!=3;++state)for(int time=0;time!=3;++time) {
  NScript::CV_BodyGuardScript script;NScript::CQ_SunnyvaleMasterData data;
  script.Game=&game;script.MasterScriptData=&data;baseIndex=script.CommentaryTimer.TimerIndex;
  if(!empty)script.HenchmanList.push_back(CScriptThing());
  commentaryTime=time==0?0:time==1?3:-1;
  const int maxExit=empty?8:(state&&commentaryTime==0?4:3);
  for(int exit=1;exit<=maxExit;++exit) {
   checks=frames=0;terminateAt=exit;data.HenchmanCommentaryState=state;
   script.HeroWieldingWeapon=true;released.clear();queried.clear();int first=timerCount+1;
   script.HenchmanCommentary();
   assert(checks==exit&&frames==(empty?exit-1:0));
   assert(released.size()==2&&released[0]==first+1&&released[1]==first);
   assert(script.HeroWieldingWeapon);
   bool clears=!empty&&state&&commentaryTime==0&&exit==4;
   assert(data.HenchmanCommentaryState==(clears?0:state));
   if(empty||exit<=2)assert(queried.empty());
   else if(state&&commentaryTime==0) {
    assert(queried.size()==(exit==3?1u:2u)&&queried[0]==baseIndex);
    if(exit==4)assert(queried[1]==first+1);
   } else {
    assert(queried.size()==(state?2u:1u));
    if(state)assert(queried[0]==baseIndex);
    assert(queried.back()==first+1);
   }
   ++cases;
  }
 }
 assert(cases==101);puts("00e5e350_TEST PASS");return 0;
}

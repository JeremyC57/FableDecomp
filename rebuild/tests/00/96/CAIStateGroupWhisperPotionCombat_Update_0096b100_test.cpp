#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
static void check(bool ok){if(!ok)abort();}
enum EAIActionReturn{AI_ACTION_NULL,AI_ACTION_TERMINATE_PROCESS,AI_ACTION_FINISHED,AI_ACTION_FAILED};
enum ETC{TCI_BOSS,TCI_STEALTH};
namespace NStealth{enum EDangerStatus{DANGER_STATUS_COMBAT=3};}
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector& rhs)const{return C3DVector(X-rhs.X,Y-rhs.Y,Z-rhs.Z);}float GetMagnitude2D()const{return (float)sqrt(X*X+Y*Y);}};
enum Event{DRAW,DANGER,VALIDJUMP,JUMP,THROW,BOAST,IDLE,COUNTER,PHASE,DONE};
static unsigned seed;
static unsigned pick(unsigned k,unsigned n){unsigned v=seed^(k*0x9e3779b9u);v^=v>>16;v*=2246822519u;v^=v>>13;return v%n;}
static bool stop,enemy,attackable,carried,stealth,playerValid,heroIsTarget,hasTarget;
static int jumps,validq,actions,returns,counter,phase,threshold,throwResult;static float initialDist,postJumpDist,postThrowDist,playerDist;
static std::vector<int> seen,want;
struct CWhisperBattleDef{long NumPhase2Potions;} definition;
static int defrefs;
template<class T>struct CDefPointer {T* p;CDefPointer():p(0){}~CDefPointer(){if(p)--defrefs;}T* operator->(){return p;}};
struct BaseDef{void GetSubDef(CDefPointer<const CWhisperBattleDef>* p){++defrefs;p->p=&definition;}} base;
struct CTCBoss {long GetBossPhase(){return phase;}long GetGenericCounter(){return counter;}void SetGenericCounter(long v){counter=v;seen.push_back(COUNTER);}void SetNextBossPhase(long v,bool b){check(v==phase+1&&!b);phase=v;seen.push_back(PHASE);}} boss;
struct CThingCreatureBase;
struct CTCStealth{void SetAsInDanger(const CThingCreatureBase&,NStealth::EDangerStatus); } stealthtc;
struct CT{float FrontFarLimit;} combat={5};
struct CThingCreatureBase {bool alive;C3DVector pos;C3DVector GetPos() const{return pos;}const CT& GetCombatTypeDef(){return combat;}BaseDef& PeekBaseDef(){return base;}
 void UnsheatheMeleeWeapon(){seen.push_back(DRAW);}bool GetTC(ETC t,CTCBoss** p){check(t==TCI_BOSS);*p=&boss;return true;}
 bool GetTC(ETC t,CTCStealth** p){check(t==TCI_STEALTH);if(stealth)*p=&stealthtc;return stealth;}} me,hero,target;
void CTCStealth::SetAsInDanger(const CThingCreatureBase& x,NStealth::EDangerStatus d){check(&x==&me&&d==NStealth::DANGER_STATUS_COMBAT);seen.push_back(DANGER);}
struct Ptr{CThingCreatureBase* p;CThingCreatureBase* Get(){return p;}CThingCreatureBase* operator->(){return p;}CThingCreatureBase& operator*(){return *p;}};
static bool IS_THING_ALIVE(Ptr& p){return p.Get()&&p.Get()->alive;}static bool IS_THING_ALIVE(CThingCreatureBase* p){return p&&p->alive;}
namespace NCreatureMode{bool IsAbleToBeAttacked(const CThingCreatureBase& t){check(&t==&target);return attackable;}}
struct CTCCarrying{static bool IsThingCarryingWeapon(CThingCreatureBase& x){check(&x==&me);return carried;}};
struct CTCEnemy{static bool AreThingsEnemies(CThingCreatureBase& p,CThingCreatureBase& a){check(&p==&hero&&&a==&me);return enemy;}};
struct Player{CThingCreatureBase* GetPControlledCreature(){return playerValid?(heroIsTarget?&target:&hero):0;}} player;
struct Players{Player& GetMainPlayer(){return player;}} players;
struct Act{int type;Act(int t,CThingCreatureBase& a,CThingCreatureBase& b):type(t){check(&a==&me&&&b==&target);}};
struct CCreatureAction_ThrowShotObject:Act{CCreatureAction_ThrowShotObject(CThingCreatureBase& a,CThingCreatureBase& b):Act(THROW,a,b){}};
struct CCreatureAction_CombatBoast:Act{CCreatureAction_CombatBoast(CThingCreatureBase& a,CThingCreatureBase& b):Act(BOAST,a,b){}};
struct CCreatureAction_CombatIdle:Act{CCreatureAction_CombatIdle(CThingCreatureBase& a,CThingCreatureBase& b):Act(IDLE,a,b){}};
struct CAIStateGroup_WhisperPotionCombat{
 Ptr PTargetThing;C3DVector Jump_to_pos;CThingCreatureBase& GetCreature(){return me;}Players& GetPlayerManager(){return players;}
 bool IsStopping(){return stop;}void Terminate(){seen.push_back(DONE);++returns;}
 bool GetValidJumpPosition(C3DVector& to){check(&to==&Jump_to_pos);seen.push_back(VALIDJUMP);return pick(60+validq++,3)!=0;}
 void DisappearJump(){seen.push_back(JUMP);++jumps;me.pos.X=postJumpDist;if(pick(62+jumps,7)==0)target.alive=false;if(pick(65+jumps,9)==0)stop=true;}
 EAIActionReturn AIAction_PerformActionAndWait(const Act& a,bool p){check(!p);seen.push_back(a.type);++actions;if(a.type==THROW){me.pos.X=postThrowDist;if(pick(69,6)==0)target.alive=false;if(pick(70,8)==0)stop=true;}return (EAIActionReturn)throwResult;}
 void Update();
};
void CAIStateGroup_WhisperPotionCombat::Update()
{
    if (!CTCCarrying::IsThingCarryingWeapon(GetCreature()))
        GetCreature().UnsheatheMeleeWeapon();
    EAIActionReturn ret = AI_ACTION_NULL;
    CTCBoss* pBossTC;
    GetCreature().GetTC(TCI_BOSS, &pBossTC);
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) { Terminate(); return; }
    CTCStealth* ptc_stealth;
    if (PTargetThing->GetTC(TCI_STEALTH, &ptc_stealth))
        ptc_stealth->SetAsInDanger(GetCreature(), NStealth::DANGER_STATUS_COMBAT);
    C3DVector vectorToTarget = GetCreature().GetPos() - PTargetThing->GetPos();
    bool can_jump = true;
    switch (pBossTC->GetBossPhase()) {
    case 1: can_jump = GetValidJumpPosition(Jump_to_pos); break;
    }
    if (can_jump && vectorToTarget.GetMagnitude2D() < GetCreature().GetCombatTypeDef().FrontFarLimit) {
        DisappearJump();
        if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) { Terminate(); return; }
    }
    if (!IS_THING_ALIVE(PTargetThing)) { Terminate(); return; }
    if (NCreatureMode::IsAbleToBeAttacked(*PTargetThing)) {
        vectorToTarget = GetCreature().GetPos() - PTargetThing->GetPos();
        if (vectorToTarget.GetMagnitude2D() > GetCreature().GetCombatTypeDef().FrontFarLimit) {
            CThingCreatureBase* p_hero=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (IS_THING_ALIVE(p_hero) &&
                (p_hero == PTargetThing.Get() ||
                 (p_hero->GetPos()-PTargetThing->GetPos()).GetMagnitude2D()>3.5f ||
                 CTCEnemy::AreThingsEnemies(*p_hero,GetCreature())))
                ret=AIAction_PerformActionAndWait(CCreatureAction_ThrowShotObject(GetCreature(),*PTargetThing),false);
            if (!IS_THING_ALIVE(PTargetThing) || IsStopping() || ret != AI_ACTION_FINISHED) { Terminate(); return; }
            switch (pBossTC->GetBossPhase()) {
            case 1: {
                CDefPointer<const CWhisperBattleDef> pdef;
                GetCreature().PeekBaseDef().GetSubDef(&pdef);
                pBossTC->SetGenericCounter(pBossTC->GetGenericCounter()+1);
                if (pBossTC->GetGenericCounter() >= pdef->NumPhase2Potions)
                    pBossTC->SetNextBossPhase(pBossTC->GetBossPhase()+1,false);
                break;
            }
            }
            vectorToTarget = GetCreature().GetPos() - PTargetThing->GetPos();
            if (vectorToTarget.GetMagnitude2D() < GetCreature().GetCombatTypeDef().FrontFarLimit && GetValidJumpPosition(Jump_to_pos)) {
                DisappearJump();
                if (!IS_THING_ALIVE(PTargetThing) || IsStopping()) { Terminate(); return; }
            }
        }
    } else {
        if((PTargetThing->GetPos()-GetCreature().GetPos()).GetMagnitude2D()>GetCreature().GetCombatTypeDef().FrontFarLimit)
            AIAction_PerformActionAndWait(CCreatureAction_CombatBoast(GetCreature(),*PTargetThing),false);
        else AIAction_PerformActionAndWait(CCreatureAction_CombatIdle(GetCreature(),*PTargetThing),false);
    }
    Terminate();
}
static float dist(unsigned n){const float values[]={3.4f,3.5f,4.9f,5,5.1f,12};return values[n%6];}
static void run(){
 seen.clear();want.clear();defrefs=jumps=validq=actions=returns=0;carried=pick(1,2)!=0;attackable=pick(2,3)!=0;stealth=pick(3,2)!=0;playerValid=pick(4,8)!=0;heroIsTarget=pick(5,3)==0;hasTarget=pick(6,12)!=0;
 stop=pick(7,10)==0;enemy=pick(8,2)!=0;phase=pick(9,3);counter=pick(10,4);threshold=definition.NumPhase2Potions=pick(11,5);throwResult=pick(12,3)?2:pick(13,4);
 initialDist=dist(pick(14,6));postJumpDist=dist(pick(15,6));postThrowDist=dist(pick(16,6));playerDist=dist(pick(17,6));target.alive=pick(18,8)!=0;hero.alive=pick(19,5)!=0;
 me.pos=C3DVector(initialDist,0,400);target.pos=C3DVector();hero.pos=C3DVector(playerDist,0,-400);
 bool alive=hasTarget&&target.alive,stopping=stop;float distance=initialDist;int p=phase,c=counter,var=0,jumpCount=0;
 if(!carried)want.push_back(DRAW);
 if(alive&&!stopping){
  if(stealth)want.push_back(DANGER);bool allowed=true;
  if(p==1){want.push_back(VALIDJUMP);allowed=pick(60+var++,3)!=0;}
  if(allowed&&distance<5){want.push_back(JUMP);++jumpCount;distance=postJumpDist;alive=pick(62+jumpCount,7)!=0;stopping=pick(65+jumpCount,9)==0;}
  if(alive&&!stopping){
   if(!attackable)want.push_back(distance>5?BOAST:IDLE);
   else if(distance>5&&playerValid&&(heroIsTarget||hero.alive)&&(heroIsTarget||playerDist>3.5f||enemy)){
    want.push_back(THROW);distance=postThrowDist;alive=pick(69,6)!=0;stopping=pick(70,8)==0;
    if(alive&&!stopping&&throwResult==2){
     if(p==1){++c;want.push_back(COUNTER);if(c>=threshold){++p;want.push_back(PHASE);}}
     if(distance<5){want.push_back(VALIDJUMP);if(pick(60+var++,3)!=0){want.push_back(JUMP);++jumpCount;}}
    }
   }
  }
 }
 want.push_back(DONE);CAIStateGroup_WhisperPotionCombat group;group.PTargetThing.p=hasTarget?&target:0;group.Update();
 check(seen==want&&returns==1&&counter==c&&phase==p&&jumps==jumpCount&&validq==var&&defrefs==0);
}
int main(){for(seed=0;seed<65536;++seed)run();puts("0096b100_TEST PASS");return 0;}
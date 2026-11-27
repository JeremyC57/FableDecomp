#include <stdio.h>
#include <stdlib.h>
#include <vector>
static void check(bool good){if(!good)abort();}
enum Tag {TCI_SPECIAL_ABILITIES,TCI_HERO_STATS,TCI_VILLAGE_MEMBER,TCI_TRESPASSER,TCI_VILLAGE,TCI_BULLET_TIME,TCI_HERO_EXPERIENCE};
enum EMessageEventType{MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO=16,MESSAGE_EVENT_HERO_SLEPT=66};
enum Record{FADE,USE,UNUSE,PAY,UNREGISTER,REMOVE,FORWARD,TRESPASS,SLEPT,POST,CANCEL,RESET,HEAL,STAMINA,SOBER,END,EVENT};
static std::vector<int> seen,want;
static void log(Record r){seen.push_back(r);}
static unsigned seed;
static unsigned pick(unsigned k,unsigned n){unsigned v=seed^(k*2654435761u);v^=v>>16;v*=2246822519u;v^=v>>13;return v%n;}
static bool hasMessage,night,screen,opening,closing,villageOwned;
static int cost,playernum,fps,nvill;
struct C3DVector {float x;};
struct CRGBColour {int r,g,b;CRGBColour(int x,int y,int z):r(x),g(y),b(z){}};
struct CTCSpecialAbilities{void SetUsingBed(bool b){log(b?USE:UNUSE);}} spec;
struct CTCHeroStats{void AddMoney(int v,bool b){check(v==-cost&&b);log(PAY);}void ForceStaminaTo(float f){check(f==1);log(STAMINA);}void SoberUp(){log(SOBER);}} stats;
struct CTCTrespasser{void ForceTrespass(){log(TRESPASS);}} trespass;
struct CTCVillage{void OnHeroSlept(){log(SLEPT);}} settlement;
struct CTCVillageMember{CTCVillage* GetPVillage(){return villageOwned?&settlement:0;}} member;
struct CTCBulletTime{void CancelMove(){log(CANCEL);}} bullet;
struct CTCHeroExperience{void ResetExperiencePickUpMultiplier(){log(RESET);}} experience;
struct Thing {bool alive,components[7];C3DVector pos;
 bool HasTC(Tag t){return components[t];}
 template<class T>bool take(Tag t,Tag correct,T** dst,T* val){check(t==correct);if(components[t])*dst=val;return components[t];}
 bool GetTC(Tag t,CTCSpecialAbilities** dst){return take(t,TCI_SPECIAL_ABILITIES,dst,&spec);}
 bool GetTC(Tag t,CTCHeroStats** dst){return take(t,TCI_HERO_STATS,dst,&stats);}
 bool GetTC(Tag t,CTCTrespasser** dst){return take(t,TCI_TRESPASSER,dst,&trespass);}
 bool GetTC(Tag t,CTCVillageMember** dst){return take(t,TCI_VILLAGE_MEMBER,dst,&member);}
 bool GetTC(Tag t,CTCVillage** dst){return take(t,TCI_VILLAGE,dst,&settlement);}
 bool GetTC(Tag t,CTCBulletTime** dst){return take(t,TCI_BULLET_TIME,dst,&bullet);}
 bool GetTC(Tag t,CTCHeroExperience** dst){return take(t,TCI_HERO_EXPERIENCE,dst,&experience);}
 C3DVector& GetPos(){return pos;}float GetMaxHealth(){return 42.5f;}void ModifyHealth(float amount,bool b){check(amount==42.5f&&!b);log(HEAL);}
};
typedef Thing CThingPlayerCreature;typedef Thing CThingVillage;
static Thing hero,controlled,building,sleeper,parent,vill;
struct Ptr{Thing* p;Thing* Get(){return p;}Thing* operator->(){return p;}};
static bool IS_THING_ALIVE(Thing* p){return p&&p->alive;}
static bool IS_THING_ALIVE(Ptr p){return IS_THING_ALIVE(p.Get());}
struct CMessageEvent{int ExtraData;} msg;
struct CMessageEventFilter_IsOfType{CMessageEventFilter_IsOfType(EMessageEventType t){check(t==MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO);}};
struct CMessageEventManager{CMessageEvent* FindMostRecentMessage(CMessageEventFilter_IsOfType){return hasMessage?&msg:0;}
 void RemoveMessage(const CMessageEvent* e){check(e==&msg);log(REMOVE);}
 void AddMessage(EMessageEventType t,int life,const C3DVector& pos,Thing* a,Thing* b,float x,float y,int z){check(t==MESSAGE_EVENT_HERO_SLEPT&&life==fps&&&pos==&parent.pos&&!a&&!b&&!x&&!y&&!z);log(POST);}} messages;
struct CGameTimeManager{bool IsNightTime(){return night;}void FastForwardTimeTo(float t,float f){check(t==(night?0.3375f:0.875f)&&f==0);log(FORWARD);}} clock1;
struct World{CMessageEventManager& GetMessageEventManager(){return messages;}CGameTimeManager& GetGameTimeManager(){return clock1;}} world;
struct Display{bool IsScreenFullyFadedOut(){return screen&&!opening&&!closing;}bool IsScreenFadeOut(){return screen;}
 void EndScreenFadeOut(){log(END);}void StartScreenFadeOut(float t,float f,CRGBColour c,float d){check(t==2.5f&&f==2&&!c.r&&!c.g&&!c.b&&d==-1);log(FADE);}} display;
struct Player{Thing* GetPControlledCreature(){return &controlled;} Thing* GetPPlayerCharacter(){return &hero;}} player;
struct Players{Player& GetMainPlayer(){return player;}} players;
struct CGameEvent{int param,number;CGameEvent(int id):param(-1),number(-1){check(id==9);}void SetPlayer(int p){number=p;}void AddUBYTE(unsigned char b){param=b;}};
struct CMainGameComponent{static CMainGameComponent* Get(){static CMainGameComponent obj;return &obj;}static int GetConstantFPS(){return fps;}
 Players& PeekPlayerManager(){return players;}Display& GetDisplayEngine(){return display;}void AddGameEvent(const CGameEvent& e){check(e.number==playernum&&e.param==0);log(EVENT);}};
struct CThingFilter_None{};
struct Search{template<class T>void GetAllLeafThings(CThingFilter_None,std::vector<T*>& v){check(v.empty());for(int i=0;i<nvill;++i)v.push_back(&vill);}} search;
struct CTCActionUseBed{
 bool WaitingForConfirmation,WaitingForFadeOut,WaitingForFadeIn,InAtomicAction,ForceTrespassOnceFadedOut;
 int Cost,PlayerNumberOfThingUsingBed;Ptr PThingUser,PBuilding;
 World& GetWorld(){return world;}Players& GetPlayerManager(){return players;}Search& GetThingSearchTools(){return search;}Thing& GetParentThing(){return parent;}
 void RemoveFromGlobalTCs(){log(UNREGISTER);}void GlobalUpdate(bool paused);
};
void CTCActionUseBed::GlobalUpdate(bool paused)
{
    if (WaitingForConfirmation) {
        CMessageEventManager& manager = GetWorld().GetMessageEventManager();
        const CMessageEvent* pevent = manager.FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO));
        if (pevent) {
            if (pevent->ExtraData == 1) {
                CMainGameComponent::Get()->GetDisplayEngine().StartScreenFadeOut(2.5f, 2.0f, CRGBColour(0,0,0), -1.0f);
                WaitingForFadeOut = true;
                InAtomicAction = true;
                CTCSpecialAbilities* ptc_spec;
                if (IS_THING_ALIVE(PThingUser) && PThingUser->GetTC(TCI_SPECIAL_ABILITIES, &ptc_spec))
                    ptc_spec->SetUsingBed(true);
                if (Cost > 0) {
                    CTCHeroStats* ptc_hero_stats;
                    CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_HERO_STATS, &ptc_hero_stats);
                    ptc_hero_stats->AddMoney(-Cost,true);
                } else if (Cost == -1 && PBuilding.Get()) {
                    ForceTrespassOnceFadedOut = true;
                }
            } else {
                CGameEvent pause_event(9);
                pause_event.SetPlayer(PlayerNumberOfThingUsingBed);
                pause_event.AddUBYTE(0);
                CMainGameComponent::Get()->AddGameEvent(pause_event);
                RemoveFromGlobalTCs();
            }
            manager.RemoveMessage(pevent);
            WaitingForConfirmation = false;
        }
    } else if (WaitingForFadeOut && CMainGameComponent::Get()->GetDisplayEngine().IsScreenFullyFadedOut()) {
        WaitingForFadeOut=false;
        CGameTimeManager& game_time_manager=GetWorld().GetGameTimeManager();
        if (game_time_manager.IsNightTime()) game_time_manager.FastForwardTimeTo(0.3375f,0.0f);
        else game_time_manager.FastForwardTimeTo(0.875f,0.0f);
        if (ForceTrespassOnceFadedOut) {
            CTCVillageMember* tc_village_member;
            if (IS_THING_ALIVE(PBuilding) && PBuilding->HasTC(TCI_VILLAGE_MEMBER) && PBuilding->GetTC(TCI_VILLAGE_MEMBER,&tc_village_member) && tc_village_member->GetPVillage()) {
                CTCTrespasser* tc_trespasser;
                if (CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_TRESPASSER,&tc_trespasser)) tc_trespasser->ForceTrespass();
            }
            ForceTrespassOnceFadedOut=false;
        }
        std::vector<CThingVillage*> villages;
        GetThingSearchTools().GetAllLeafThings<CThingVillage>(CThingFilter_None(),villages);
        if (false) {} else for (long c1=0;c1<villages.size();++c1) {
            CTCVillage* ptc_village=0;
            villages[c1]->GetTC(TCI_VILLAGE,&ptc_village);
            ptc_village->OnHeroSlept();
        }
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_HERO_SLEPT,CMainGameComponent::GetConstantFPS(),GetParentThing().GetPos(),0,0,0.0f,0.0f,0);
        CThingPlayerCreature* phero=GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
        CTCBulletTime* ptc_bullet_time;
        if(phero->GetTC(TCI_BULLET_TIME,&ptc_bullet_time)) ptc_bullet_time->CancelMove();
        if(IS_THING_ALIVE(phero)) {
            CTCHeroExperience* ptc_hero_experience;
            if(phero->GetTC(TCI_HERO_EXPERIENCE,&ptc_hero_experience)) ptc_hero_experience->ResetExperiencePickUpMultiplier();
            phero->ModifyHealth(phero->GetMaxHealth(),false);
            CTCHeroStats* ptc_hero_stats;
            if(phero->GetTC(TCI_HERO_STATS,&ptc_hero_stats)) {
                ptc_hero_stats->ForceStaminaTo(1.0f);
                ptc_hero_stats->SoberUp();
            }
        }
        CMainGameComponent::Get()->GetDisplayEngine().EndScreenFadeOut();
        CGameEvent pause_event(9);
        pause_event.SetPlayer(PlayerNumberOfThingUsingBed);
        pause_event.AddUBYTE(0);
        CMainGameComponent::Get()->AddGameEvent(pause_event);
        WaitingForFadeIn=true;
    } else if (WaitingForFadeIn && !CMainGameComponent::Get()->GetDisplayEngine().IsScreenFadeOut()) {
        RemoveFromGlobalTCs();
        WaitingForFadeIn=false;
        InAtomicAction=false;
        CTCSpecialAbilities* ptc_spec;
        if(IS_THING_ALIVE(PThingUser) && PThingUser->GetTC(TCI_SPECIAL_ABILITIES,&ptc_spec))
            ptc_spec->SetUsingBed(false);
    }
}
static void run(){
 seen.clear();want.clear();CTCActionUseBed b;
 b.WaitingForConfirmation=pick(1,3)==0;b.WaitingForFadeOut=pick(2,2)!=0;b.WaitingForFadeIn=pick(3,2)!=0;b.InAtomicAction=pick(4,2)!=0;b.ForceTrespassOnceFadedOut=pick(5,2)!=0;
 b.PThingUser.p=pick(6,3)?&sleeper:0;b.PBuilding.p=pick(7,3)?&building:0;cost=b.Cost=int(pick(8,5))-1;playernum=b.PlayerNumberOfThingUsingBed=pick(9,4);
 screen=pick(10,2)!=0;opening=pick(11,3)==0;closing=pick(12,3)==0;night=pick(13,2)!=0;fps=20+pick(14,20);hasMessage=pick(15,3)!=0;msg.ExtraData=pick(16,3);villageOwned=pick(17,2)!=0;nvill=pick(18,5);
 sleeper.alive=pick(19,2)!=0;building.alive=pick(20,2)!=0;hero.alive=pick(21,2)!=0;
 for(int i=0;i<7;++i){hero.components[i]=pick(30+i,2)!=0;controlled.components[i]=pick(40+i,2)!=0;building.components[i]=pick(50+i,2)!=0;sleeper.components[i]=pick(60+i,2)!=0;}
 controlled.components[TCI_HERO_STATS]=vill.components[TCI_VILLAGE]=true;
 bool confirm=b.WaitingForConfirmation,out=b.WaitingForFadeOut,in=b.WaitingForFadeIn,atomic=b.InAtomicAction,force=b.ForceTrespassOnceFadedOut;
 bool user=b.PThingUser.p && sleeper.alive && sleeper.components[TCI_SPECIAL_ABILITIES];
 if(confirm){if(hasMessage){confirm=false;if(msg.ExtraData==1){want.push_back(FADE);out=atomic=true;if(user)want.push_back(USE);if(cost>0)want.push_back(PAY);else if(cost==-1&&b.PBuilding.p)force=true;}else {want.push_back(EVENT);want.push_back(UNREGISTER);}want.push_back(REMOVE);}}
 else if(out&&screen&&!opening&&!closing){out=false;in=true;want.push_back(FORWARD);
 if(force&&b.PBuilding.p&&building.alive&&building.components[TCI_VILLAGE_MEMBER]&&villageOwned&&controlled.components[TCI_TRESPASSER])want.push_back(TRESPASS);
 force=false;for(int v=0;v<nvill;++v)want.push_back(SLEPT);want.push_back(POST);
 if(hero.components[TCI_BULLET_TIME])want.push_back(CANCEL);
 if(hero.alive){if(hero.components[TCI_HERO_EXPERIENCE])want.push_back(RESET);want.push_back(HEAL);if(hero.components[TCI_HERO_STATS]){want.push_back(STAMINA);want.push_back(SOBER);}}
 want.push_back(END);want.push_back(EVENT);}
 else if(in&&!screen){in=false;atomic=false;want.push_back(UNREGISTER);if(user)want.push_back(UNUSE);}
 b.GlobalUpdate(pick(90,2)!=0);
 check(want==seen&&confirm==b.WaitingForConfirmation&&out==b.WaitingForFadeOut&&in==b.WaitingForFadeIn&&atomic==b.InAtomicAction&&force==b.ForceTrespassOnceFadedOut);
}
int main(){for(seed=0;seed<65536;++seed)run();puts("007ec710_TEST PASS");return 0;}
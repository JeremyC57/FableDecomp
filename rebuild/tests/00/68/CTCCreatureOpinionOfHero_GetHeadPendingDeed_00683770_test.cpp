#include <list>
#include <stdio.h>
#include <stdlib.h>
static void check(bool b){if(!b)exit(2);}
enum {TCI_SHOP_KEEPER=144,OPINION_DEED_TYPE_CRIME_ASSAULT=7,OPINION_DEED_TYPE_CRIME_GBH=8};
namespace NCreatureMode{enum {MODE_CARRYING_GOODS_STAND=6, MODE_HOLD_JUG=41, MODE_ARMS_OFF_TABLE=45, MODE_SIT_TAVERN_STOOL=11,MODE_FILL_JUG=43,MODE_SERVE_CUSTOMER=42};}
struct CThing;
struct Target{CThing* p;CThing* Get()const{return p;}};
struct COpinionDeed{int DeedType,DeedHandle;Target PTarget;bool WasSearchingForWitnesses,IsStillHappening,IsCrime;};
struct Effects{float PreReactionDelayIgnoreOnDelay;bool AllowIndirectReactWhileCarrying;};
struct CTCHeroOpinionDeedLog{
 COpinionDeed deed;Effects effects;mutable int reads;
 const COpinionDeed* GetDeedFromHandle(long h)const{return h==deed.DeedHandle?&deed:0;}
 const Effects* PeekPDeedEffectsDef(int t)const{check(t==deed.DeedType);++reads;return &effects;}
};
struct CMode{long type;long GetType()const{return type;}};
struct CTCCreatureModeManager{CMode mode;mutable int reads;const CMode& GetCurrentMode()const{++reads;return mode;}};
struct CTCVillage{bool reported;int reports;bool HasCrimeBeenReported(long h){check(h==4);++reports;return reported;}};
struct CTCVillageMember{CTCVillage* village;CTCVillage* GetPVillage()const{return village;}};
struct CThing{
 const CTCCreatureModeManager* mode;const CTCVillageMember* village;bool shopkeeper;
 bool PeekTC(const CTCCreatureModeManager** out)const{if(mode)*out=mode;return mode!=0;}
 bool PeekTC(const CTCVillageMember** out)const{if(village)*out=village;return village!=0;}
 bool HasTC(int id)const{check(id==TCI_SHOP_KEEPER);return shopkeeper;}
};
struct CThingPlayerCreature{CTCHeroOpinionDeedLog* log;bool PeekTC(const CTCHeroOpinionDeedLog** out){*out=log;return true;}};
struct CPlayer{CThingPlayerCreature* hero;CThingPlayerCreature* GetPControlledCreature(){return hero;}};
struct CPlayerManager{CPlayer player;CPlayer& GetMainPlayer(){return player;}};
struct CMainGameComponent{static long GetConstantFPS(){return 30;}};
struct CWorld{long GetFrame()const{return 60;}};
struct CTCCreatureOpinionOfHero{
 CThing parent;CPlayerManager manager;CWorld world;long LastOpinionReactionFrame;bool guard;
 std::list<long> KnownDeeds[92], PendingDeeds;
 CThing& GetParentThing(){return parent;} CWorld& GetWorld(){return world;}
 CPlayerManager& GetPlayerManager(){return manager;} bool IsGuardOnLevel()const{return guard;}
 const COpinionDeed* GetHeadPendingDeed();
};
const COpinionDeed* CTCCreatureOpinionOfHero::GetHeadPendingDeed()
{
    const COpinionDeed* preturn_deed = 0;
    CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (player) {
        const CTCHeroOpinionDeedLog* ptc_deedlog;
        player->PeekTC(&ptc_deedlog);
        long frames_since_reacted = GetWorld().GetFrame() - LastOpinionReactionFrame;
        while (!preturn_deed && !PendingDeeds.empty()) {
            const CTCCreatureModeManager* ptc_mode_manager;
            const CTCVillageMember* tc_village_member;
            preturn_deed = ptc_deedlog->GetDeedFromHandle(*PendingDeeds.begin());
            if (!preturn_deed) {
                PendingDeeds.pop_front();
            }

            else if ((float)CMainGameComponent::GetConstantFPS() * ptc_deedlog->PeekPDeedEffectsDef(preturn_deed->DeedType)->PreReactionDelayIgnoreOnDelay > (float)frames_since_reacted) {
                KnownDeeds[preturn_deed->DeedType].push_front(preturn_deed->DeedHandle);
                PendingDeeds.pop_front();
                preturn_deed = 0;
            }
            else if (preturn_deed->WasSearchingForWitnesses && !preturn_deed->IsStillHappening) {
                KnownDeeds[preturn_deed->DeedType].push_front(preturn_deed->DeedHandle);
                PendingDeeds.pop_front();
                preturn_deed = 0;
            }
            else if (preturn_deed->PTarget.Get() != &GetParentThing() && !preturn_deed->IsCrime
                    && !ptc_deedlog->PeekPDeedEffectsDef(preturn_deed->DeedType)->AllowIndirectReactWhileCarrying
                    && GetParentThing().PeekTC(&ptc_mode_manager)
                    && (ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_CARRYING_GOODS_STAND || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_HOLD_JUG
                        || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_ARMS_OFF_TABLE || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_SIT_TAVERN_STOOL
                        || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_FILL_JUG || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_SERVE_CUSTOMER)) {
                KnownDeeds[preturn_deed->DeedType].push_front(preturn_deed->DeedHandle);
                PendingDeeds.pop_front();
                preturn_deed = 0;
            }
            else if (preturn_deed->IsCrime && (preturn_deed->DeedType == OPINION_DEED_TYPE_CRIME_ASSAULT || preturn_deed->DeedType == OPINION_DEED_TYPE_CRIME_GBH) && GetParentThing().PeekTC(&tc_village_member)
                && tc_village_member->GetPVillage() && IsGuardOnLevel() && !GetParentThing().HasTC(TCI_SHOP_KEEPER)
                && !tc_village_member->GetPVillage()->HasCrimeBeenReported(preturn_deed->DeedHandle)) {
                KnownDeeds[preturn_deed->DeedType].push_front(preturn_deed->DeedHandle);
                PendingDeeds.pop_front();
                preturn_deed = 0;
            }
        }
    }
    return preturn_deed;
}
int main(){
 for(int case_no=0;case_no<28;++case_no){
  CTCCreatureOpinionOfHero owner;CTCHeroOpinionDeedLog log={0};CTCCreatureModeManager mode={0};CTCVillage village={0};
  CTCVillageMember member={&village};CThingPlayerCreature hero={&log};
  owner.manager.player.hero=&hero;owner.LastOpinionReactionFrame=0;owner.guard=true;
  owner.parent.mode=&mode;owner.parent.village=&member;owner.parent.shopkeeper=false;
  COpinionDeed& deed=log.deed;deed.DeedHandle=4;deed.DeedType=3;log.effects.PreReactionDelayIgnoreOnDelay=2;
  bool drop=false;int modes=-1,reports=0;bool pending=true;
  if(case_no!=1)owner.PendingDeeds.push_back(4);
  if(case_no==0){owner.manager.player.hero=0;}
  if(case_no==2){owner.PendingDeeds.push_front(999);}
  if(case_no==3){log.effects.PreReactionDelayIgnoreOnDelay=2.1f;drop=true;}
  if(case_no==4){deed.WasSearchingForWitnesses=true;drop=true;}
  if(case_no==5){deed.WasSearchingForWitnesses=true;deed.IsStillHappening=true;}
  const int types[]={6,41,45,11,43,42};
  if(case_no>=6&&case_no<=11){mode.mode.type=types[case_no-6];drop=true;modes=case_no-5;}
  if(case_no==12){owner.parent.mode=0;modes=0;}
  if(case_no==13){deed.PTarget.p=&owner.parent;modes=0;}
  if(case_no==14){log.effects.AllowIndirectReactWhileCarrying=true;modes=0;}
  if(case_no>=15&&case_no<=23){
   deed.IsCrime=true;deed.DeedType=case_no==16?8:7;modes=0;
   if(case_no==17)deed.DeedType=9;
   if(case_no==18)owner.parent.village=0;
   if(case_no==19)member.village=0;
   if(case_no==20)owner.guard=false;
   if(case_no==21)owner.parent.shopkeeper=true;
   if(case_no==22)village.reported=true;
   if(case_no==15||case_no==16||case_no==23)drop=true;
   if(drop||case_no==22)reports=1;
  }
  if(case_no==24){owner.PendingDeeds.clear();owner.PendingDeeds.push_back(999);pending=false;}
  if(case_no==25){owner.LastOpinionReactionFrame=1;drop=true;}
  if(case_no==26){deed.IsStillHappening=true;}
  if(case_no==27){owner.PendingDeeds.push_front(999);owner.PendingDeeds.push_back(999);deed.WasSearchingForWitnesses=true;drop=true;}
  owner.KnownDeeds[deed.DeedType].push_back(123);
  bool no_result=case_no==0||case_no==1||case_no==24||drop;
  check(owner.GetHeadPendingDeed()==(no_result?0:&deed));
  if(drop||case_no==1||!pending)check(owner.PendingDeeds.empty());
  else check(owner.PendingDeeds.size()==1&&owner.PendingDeeds.front()==4);
  check(owner.KnownDeeds[deed.DeedType].size()==(drop?2u:1u));
  check(owner.KnownDeeds[deed.DeedType].front()==(drop?4:123));
  if(modes>=0)check(mode.reads==modes);
  check(village.reports==reports);
 }
 puts("00683770_TEST PASS");
}
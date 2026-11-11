#include <cstdio>
#include <cstdlib>
#include <string>
static void check(bool ok){if(!ok)std::abort();}
static std::string trace;static long frame,fps;static bool killDuringAppearance,clockBack;
static int homes,moves,adjustments,marriages,renowns,checks,shopDetaches,villageDetaches,workSets,miniUpdates;
class CThing;class CThingAICreature;class CThingPlayerCreature;class CThingBuilding;class CTCWife;class CTCBase;class CTCVillage;class CTCCreatureOpinionOfHero;
static CThingAICreature* expectedCreature;static CThingBuilding* expectedHome;static CThingPlayerCreature* expectedPlayer;static CTCWife* active;
enum EOpinion{OPINION_TEST=1};enum EMarriageState{MARRIAGE_TEST=1};enum EOpinionAttitudeType{ATTITUDE_TEST=18};
class CTCBase{ public:CThing* parent;CThing& GetParentThing(){return *parent;}};
class CThing{ public:bool dead;CTCBase* components[5];CThing():dead(false){for(int i=0;i<5;++i)components[i]=0;}bool IsDead()const{return dead;}template<class T>bool GetTC(T** out){if(!components[T::Index])return false;*out=static_cast<T*>(components[T::Index]);return true;}};
class CThingBuilding:public CThing{ public:};
class CThingPlayerCreature:public CThing{ public:};
class CThingAICreature:public CThing{ public:CThingBuilding* home;CThingBuilding* work;void SetHomeBuilding(CThingBuilding* p){check(this==expectedCreature&&p==expectedHome);home=p;++homes;trace+='H';}void SetWorkBuilding(CThingBuilding* p){check(this==expectedCreature&&p==0);work=p;++workSets;trace+='X';if(clockBack)--frame;}};
class CTCVillageMember:public CTCBase{ public:enum{Index=0};CTCVillage* village;CTCVillage* GetPVillage()const{return village;}void SetPVillage(CTCVillage* p){check(p==0);village=p;trace+='0';}};
class CTCCreatureOpinionOfHero:public CTCBase{ public:
 enum{Index=1};EOpinionAttitudeType ForcedAttitude;bool HeroOpinionEnemy;
 void AdjustOpinionTransient(EOpinion op,float peak,long runin,long capped,float out,bool flag){check(op==OPINION_TEST&&peak==0.5f&&runin==0&&capped==fps*300&&out==0.2f&&!flag);++adjustments;trace+='V';}
 void SetOpinionReactionMask(const std::string& s){check(s=="OPINION_REACTION_MASK_WIFE");trace+='r';}
 void SetOpinionDeedMask(const std::string& s){check(s=="OPINION_DEED_MASK_WIFE");trace+='d';}
 void SetPersonalityOverride(const std::string& s,bool flag){check(s=="OPINION_PERSONALITY_WIFE_NORMAL"&&flag);trace+='p';}
 void SetForcedAttitude(EOpinionAttitudeType a){ForcedAttitude=a;}
 void ClearAllKnownDeeds(){check(ForcedAttitude==ATTITUDE_TEST&&HeroOpinionEnemy);trace+='K';}
};
class CTCHeroStats:public CTCBase{ public:enum{Index=2};long lastRenown;void AddNewMarriage(const CThingAICreature* wife,const CThingBuilding* home,EMarriageState state){check(wife==expectedCreature&&home==expectedHome&&state==MARRIAGE_TEST);++marriages;trace+='J';}void AddRenown(long n){lastRenown=n;++renowns;trace+='R';}};
class CTCShopKeeper:public CTCBase{ public:enum{Index=3};void DetachFromShop(){++shopDetaches;trace+='S';}};
class CTCBuyableHouse:public CTCBase{ public:enum{Index=4};CThing* wife;void SetWifeLivingHere(CThing* p){check(p==expectedCreature);wife=p;trace+='B';}};
class CTCVillage:public CTCBase{ public:void AddThingToVillage(CThing& t){check(&t==expectedCreature);CTCVillageMember* member;check(t.GetTC(&member)&&member->village==0);member->village=this;++moves;trace+='M';}void DetachFromDummyVillager(CThing* p){check(p==expectedCreature);++villageDetaches;trace+='D';}};
class COpinionReactionManager{ public:EOpinionAttitudeType GetAttitudeType(const CThing&,const void*)const;};
class CPlayer{ public:long number;CThingPlayerCreature* controlled;long GetNumber()const{return number;}CThingPlayerCreature* GetPControlledCreature()const{trace+='C';return controlled;}};
class CPlayerManager{ public:CPlayer* player;CPlayer& GetMainPlayer(){trace+='P';return *player;}};
class CWorld{ public:COpinionReactionManager* opinion;CPlayer* player;static long GetFrame(){trace+='f';return frame;}const CPlayer* PeekMyPlayer()const{return player;}COpinionReactionManager& GetOpinionReactionManager(){trace+='W';return *opinion;}};
static CWorld world;static CPlayerManager players;
class CRGBColour{ public:unsigned char R,G,B,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b):R(r),G(g),B(b),A(255){}};
class CHeroMarriageDef{ public:float WeddingTimeForScreenToFadeOut,WeddingTimeForScreenToFadeIn,HeroStatRenownIncreasePerMarriage;};
class CGameEvent{ public:long Type;signed char Player;unsigned char Data[32],EndPos;bool Valid,Replacement;CGameEvent(long type):Type(type),Player(-1),EndPos(0),Valid(true),Replacement(false){}void SetPlayer(long n){Player=(signed char)n;}void AddUBYTE(unsigned char v){check(EndPos==0);Data[EndPos++]=v;}};
class CDisplayEngine{ public:void StartScreenFadeOut(float out,float in,CRGBColour c,float extra);};
class CMainGameComponent{ public:CDisplayEngine display;static CMainGameComponent* Get();static long GetConstantFPS(){return fps;}CDisplayEngine& GetDisplayEngine(){return display;}void AddGameEvent(const CGameEvent& e){check(e.Type==9&&e.Player==7&&e.EndPos==1&&e.Data[0]==1&&e.Valid&&!e.Replacement);trace+='E';}};
static CMainGameComponent mainComponent;CMainGameComponent* CMainGameComponent::Get(){return &mainComponent;}
class CPlayerGui{ public:static CPlayerGui& Get(){static CPlayerGui gui;return gui;}void UpdateMiniMapMarkers(bool flag){check(!flag);++miniUpdates;trace+='U';}};
template<class T>static bool IS_THING_ALIVE(const T& thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
class CTCWife:public CTCBase{ public:
 const CHeroMarriageDef* PHeroMarriageDef;long FrameLastAwareOfHusband,FrameGotMarriedToThePlayer,FrameLastReducedOpinion,FrameLastEvaluatedGiftOpinion,FrameLastConsideredGivingGift,HouseDressingLevelLastCommentedOn;
 long FrameNoticedChangeInAppearance[16];EOpinionAttitudeType OpinionAttitude;unsigned char AttitudeUpdateCounter;bool NeedsToChangeBrain,WaitingForFadeOut;
 CThingBuilding* FindMaritalHome(){trace+='F';return expectedHome;}
 CPlayerManager& GetPlayerManager(){return players;}
 CWorld& GetWorld()const{return world;}const CWorld& PeekWorld()const{return world;}
 void ClearAllAppearanceChangedNoticed(){for(long i=0;i<16;++i)FrameNoticedChangeInAppearance[i]=0;}
 void CheckAppearanceChanges(CThingPlayerCreature& player){check(&player==expectedPlayer);++checks;trace+='A';for(int i=0;i<16;++i)FrameNoticedChangeInAppearance[i]=i+1;if(killDuringAppearance)player.dead=true;}
 void SetAttractionIcon(bool show,float size,unsigned char alpha){check(show&&size==0.5f&&alpha==255);trace+='a';}
 void InitWifeNonpersistentState();void SendPauseEvent(bool);
 void InitWifeAtEndOfMarriageFrescoStartFadeIn(){trace+='i';}void InitWifeAtEndOfMarriageFrescoEndFadeIn(){trace+='e';}
 void AddToGlobalTCs(){trace+='G';}void Marry(bool);
};
EOpinionAttitudeType COpinionReactionManager::GetAttitudeType(const CThing& thing,const void* source)const{check(&thing==expectedCreature&&source==0);CTCCreatureOpinionOfHero* op;check(expectedCreature->GetTC(&op)&&op->ForcedAttitude==ATTITUDE_TEST&&!op->HeroOpinionEnemy);trace+='O';return (EOpinionAttitudeType)12;}
void CDisplayEngine::StartScreenFadeOut(float out,float in,CRGBColour c,float extra){check(out==active->PHeroMarriageDef->WeddingTimeForScreenToFadeOut&&in==active->PHeroMarriageDef->WeddingTimeForScreenToFadeIn&&c.R==0&&c.G==0&&c.B==0&&c.A==255&&extra==-1);trace+='Z';}
inline void CTCWife::SendPauseEvent(bool pause)
{
 CGameEvent event(9);
 event.SetPlayer(PeekWorld().PeekMyPlayer()->GetNumber());
 event.AddUBYTE(pause?1:0);
 CMainGameComponent::Get()->AddGameEvent(event);
}
inline void CTCWife::InitWifeNonpersistentState()
{
 if(FrameGotMarriedToThePlayer<=CWorld::GetFrame()){
  SetAttractionIcon(true,0.5f,255);
  NeedsToChangeBrain=true;
 }
}
void CTCWife::Marry(bool instant_with_no_cutscene)
{
 CThingAICreature& creature=static_cast<CThingAICreature&>(GetParentThing());
 CThingBuilding* marital_home=FindMaritalHome();
 if(marital_home) {
  CTCVillageMember* ptc_village_member;
  GetParentThing().GetTC(&ptc_village_member);
  if(!ptc_village_member->GetPVillage()) {
   CTCVillageMember* ptc_home_village_member=0;
   if(marital_home->GetTC(&ptc_home_village_member)) {
    CTCVillage* ptc_village=ptc_home_village_member->GetPVillage();
    if(ptc_village) {
     ptc_village_member->SetPVillage(0);
     ptc_village->AddThingToVillage(GetParentThing());
    }
   }
  }
  creature.SetHomeBuilding(marital_home);
  CTCBuyableHouse* ptc_buyable_house=0;
  marital_home->GetTC(&ptc_buyable_house);
  ptc_buyable_house->SetWifeLivingHere(&creature);
 }
 long cur_frame=CWorld::GetFrame();
 FrameLastAwareOfHusband=cur_frame;
 FrameGotMarriedToThePlayer=cur_frame;
 FrameLastReducedOpinion=cur_frame;
 FrameLastEvaluatedGiftOpinion=cur_frame+5;
 FrameLastConsideredGivingGift=cur_frame+13;
 CThingPlayerCreature* pplayer=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 HouseDressingLevelLastCommentedOn=-1;
 if(IS_THING_ALIVE(pplayer)) {
  CheckAppearanceChanges(*pplayer);
  ClearAllAppearanceChangedNoticed();
 }
 if(IS_THING_ALIVE(pplayer)) {
  CTCHeroStats* ptc_hero_stats;
  pplayer->GetTC(&ptc_hero_stats);
  ptc_hero_stats->AddNewMarriage(static_cast<CThingAICreature*>(&GetParentThing()),marital_home,(EMarriageState)1);
  ptc_hero_stats->AddRenown((long)PHeroMarriageDef->HeroStatRenownIncreasePerMarriage);
 }
 CTCVillageMember* ptc_village_member=0;
 if(GetParentThing().GetTC(&ptc_village_member)) {
  CTCVillage* ptc_village=0;
  ptc_village=ptc_village_member->GetPVillage();
  if(ptc_village) {
   CTCCreatureOpinionOfHero* ptc_village_opinion=0;
   if(ptc_village->GetParentThing().GetTC(&ptc_village_opinion))
    ptc_village_opinion->AdjustOpinionTransient((EOpinion)1,0.5f,0,CMainGameComponent::GetConstantFPS()*300,0.2f,false);
   ptc_village->DetachFromDummyVillager(&GetParentThing());
  }
 }
 CTCShopKeeper* tc_shopkeeper;
 if(GetParentThing().GetTC(&tc_shopkeeper))tc_shopkeeper->DetachFromShop();
 CTCCreatureOpinionOfHero* ptc_opinion=0;
 if(GetParentThing().GetTC(&ptc_opinion)) {
  ptc_opinion->SetOpinionReactionMask("OPINION_REACTION_MASK_WIFE");
  ptc_opinion->SetOpinionDeedMask("OPINION_DEED_MASK_WIFE");
  ptc_opinion->SetPersonalityOverride("OPINION_PERSONALITY_WIFE_NORMAL",true);
  ptc_opinion->SetForcedAttitude((EOpinionAttitudeType)18);
  ptc_opinion->ClearAllKnownDeeds();
  ptc_opinion->HeroOpinionEnemy=false;
  OpinionAttitude=GetWorld().GetOpinionReactionManager().GetAttitudeType(GetParentThing(),0);
  AttitudeUpdateCounter=0;
 }
 creature.SetWorkBuilding(0);
 if(instant_with_no_cutscene) {
  InitWifeNonpersistentState();
  InitWifeAtEndOfMarriageFrescoStartFadeIn();
  InitWifeAtEndOfMarriageFrescoEndFadeIn();
 } else {
  AddToGlobalTCs();
  CMainGameComponent::Get()->GetDisplayEngine().StartScreenFadeOut(PHeroMarriageDef->WeddingTimeForScreenToFadeOut,PHeroMarriageDef->WeddingTimeForScreenToFadeIn,CRGBColour(0,0,0),-1.0f);
  SendPauseEvent(true);
  WaitingForFadeOut=true;
 }
 CPlayerGui::Get().UpdateMiniMapMarkers(false);
}

int main(){
 const float renown[]={-2.5f,0.0f,128.5f};int cases=0;
 for(int home=0;home<2;++home)for(int vm=0;vm<3;++vm)for(int hv=0;hv<3;++hv)for(int ps=0;ps<4;++ps)for(int flags=0;flags<16;++flags)for(int r=0;r<3;++r)for(int back=0;back<2;++back){
  if(home&&vm==0)continue;
  homes=moves=adjustments=marriages=renowns=checks=shopDetaches=villageDetaches=workSets=miniUpdates=0;
  CThingAICreature creature;CThingBuilding house,oldWork;CThingPlayerCreature player;CTCVillageMember member,homeMember;CTCBuyableHouse buyable;CTCHeroStats stats;CTCShopKeeper shop;CTCCreatureOpinionOfHero wifeOpinion,villageOpinion;CThing existingVillageThing,homeVillageThing;CTCVillage existingVillage,homeVillage;COpinionReactionManager opinions;CPlayer mainPlayer,myPlayer;
  existingVillage.parent=&existingVillageThing;homeVillage.parent=&homeVillageThing;member.parent=&creature;homeMember.parent=&house;member.village=vm==2?&existingVillage:0;homeMember.village=hv==2?&homeVillage:0;
  if(vm)creature.components[0]=&member;if(hv)house.components[0]=&homeMember;house.components[4]=&buyable;buyable.wife=0;
  if(flags&1){existingVillageThing.components[1]=&villageOpinion;homeVillageThing.components[1]=&villageOpinion;}
  if(flags&2)creature.components[1]=&wifeOpinion;if(flags&4)creature.components[3]=&shop;player.components[2]=&stats;
  player.dead=ps==1;mainPlayer.controlled=ps==0?0:&player;mainPlayer.number=5;myPlayer.number=7;players.player=&mainPlayer;world.player=&myPlayer;world.opinion=&opinions;
  expectedCreature=&creature;expectedHome=home?&house:0;expectedPlayer=&player;creature.home=0;creature.work=&oldWork;killDuringAppearance=ps==3;clockBack=back!=0;frame=100;fps=30;
  wifeOpinion.HeroOpinionEnemy=true;wifeOpinion.ForcedAttitude=(EOpinionAttitudeType)0;
  CHeroMarriageDef def;def.WeddingTimeForScreenToFadeOut=2.0f;def.WeddingTimeForScreenToFadeIn=4.0f;def.HeroStatRenownIncreasePerMarriage=renown[r];CTCWife wife;active=&wife;wife.parent=&creature;wife.PHeroMarriageDef=&def;wife.OpinionAttitude=(EOpinionAttitudeType)0;wife.AttitudeUpdateCounter=8;wife.NeedsToChangeBrain=false;wife.WaitingForFadeOut=false;for(int i=0;i<16;++i)wife.FrameNoticedChangeInAppearance[i]=-1;
  trace="";bool instant=(flags&8)!=0;wife.Marry(instant);
  bool relocated=home&&vm==1&&hv==2;bool hasVillage=vm==2||relocated;bool checked=ps==2||ps==3;bool liveAfter=ps==2;
  std::string expected="F";if(home){if(relocated)expected+="0M";expected+="HB";}expected+="fPC";if(checked)expected+='A';if(liveAfter)expected+="JR";if(hasVillage){if(flags&1)expected+='V';expected+='D';}if(flags&4)expected+='S';if(flags&2)expected+="rdpKWO";expected+='X';if(instant){expected+='f';if(!back)expected+='a';expected+="ie";}else expected+="GZE";expected+='U';check(trace==expected);
  check(wife.FrameLastAwareOfHusband==100&&wife.FrameGotMarriedToThePlayer==100&&wife.FrameLastReducedOpinion==100&&wife.FrameLastEvaluatedGiftOpinion==105&&wife.FrameLastConsideredGivingGift==113&&wife.HouseDressingLevelLastCommentedOn==-1);
  check(homes==home&&moves==(relocated?1:0)&&creature.home==expectedHome&&creature.work==0&&workSets==1);
  check(checks==(checked?1:0)&&marriages==(liveAfter?1:0)&&renowns==marriages);if(liveAfter)check(stats.lastRenown==(long)renown[r]);
  check(villageDetaches==(hasVillage?1:0)&&adjustments==(hasVillage&&(flags&1)?1:0)&&shopDetaches==((flags&4)?1:0)&&miniUpdates==1);
  for(int i=0;i<16;++i)check(wife.FrameNoticedChangeInAppearance[i]==(checked?0:-1));
  check(wife.OpinionAttitude==(EOpinionAttitudeType)((flags&2)?12:0)&&wife.AttitudeUpdateCounter==((flags&2)?0:8));
  check(wife.WaitingForFadeOut==!instant&&wife.NeedsToChangeBrain==(instant&&!back));if(home)check(buyable.wife==&creature);++cases;
 }
 check(cases==5760);std::puts("006d18a0_TEST PASS");return 0;
}
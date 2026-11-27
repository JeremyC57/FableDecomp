#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <list>
enum EOpinionDeedType { OPINION_DEED_TYPE_NONE, OPINION_DEED_TYPE_CRIME_WEAPON_OUT, OPINION_DEED_TYPE_CRIME_ASSAULT=7, OPINION_DEED_TYPE_CRIME_GBH, OPINION_DEED_TYPE_CRIME_MURDER, OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH=53, other };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_NULL, post };
enum {TCI_CREATURE_OPINION_OF_HERO=79};
struct C3DVector {float x,y,z;};
struct CFaction {};
struct CThingAICreature {};
struct CRegionDef {bool WeaponOutCrime,UseCrimes,DisplayCrimes;};
template<class T> struct CDefPointer {T* p; CDefPointer(T* q=0):p(q){} T* Get()const{return p;} T* operator->()const {assert(p);return p;}};
struct CEnemyDef {int Faction;};
struct Base {CEnemyDef* enemy;bool GetSubDef(CDefPointer<const CEnemyDef>* out)const{out->p=enemy;return enemy!=0;}};
struct CTCVillage {bool limbo;bool GetVillageLimbo()const{return limbo;}};
struct CTCHero {int calls;void GiveTutorialForCategory(int n,bool b){assert(n==19&&!b);++calls;}};
struct CThing {bool opinion,alive;Base base;CTCVillage* village;CTCHero* hero;C3DVector pos;
 bool HasTC(int k)const {assert(k==79);return opinion;} const Base* GetPBaseDef()const{return &base;}
 const C3DVector& GetPos()const{return pos;}bool GetTC(CTCVillage** p){return (*p=village)!=0;}bool GetTC(CTCHero** p){return (*p=hero)!=0;}};
static bool IS_THING_ALIVE(CThing* t){return t&&t->alive;}
struct CRegion {CRegionDef* def;CDefPointer<const CRegionDef> GetPRegionDef()const{return def;}};
struct Map {CRegion region;int calls;int GetMapNumberAt(const C3DVector& p){assert(p.x==2);++calls;return 5;}int GetRegionNumberMapIsIn(int n){assert(n==5);return 9;}const CRegion& GetRegion(int n){assert(n==9);return region;}} world;
static Map& GetWorldMap(){return world;}
struct CGameDefinitionManager {static CGameDefinitionManager& Get(){static CGameDefinitionManager o;return o;}int GetDefGlobalIndexFromName(const char* s){assert(!strcmp(s,"FACTION_VILLAGERS"));return 13;}};
namespace NGlobalConsole {bool GEnableTutorials;}
struct CPlayerGui {bool cut;static CPlayerGui& Get(){static CPlayerGui o;return o;}CPlayerGui& GetSubtitleRenderer(){return *this;}bool IsInCutscene()const{return cut;}};
struct COpinionDeed { static int deleted;EOpinionDeedType type;long h;C3DVector pos;const CThing* target;bool search,crime;long extra;
 COpinionDeed(EOpinionDeedType t,long a,const C3DVector& p,const CThing* v,bool s,bool c,long e):type(t),h(a),pos(p),target(v),search(s),crime(c),extra(e){}~COpinionDeed(){++deleted;}};
int COpinionDeed::deleted;
struct CWitnessesSearchingDeed {long h;EOpinionDeedType t;CThingAICreature* r;EOpinionPostDeedType mode;const CFaction* f;bool reverse;CWitnessesSearchingDeed(long h_,EOpinionDeedType t_,CThingAICreature* r_,EOpinionPostDeedType m,const CFaction* f_,bool v):h(h_),t(t_),r(r_),mode(m),f(f_),reverse(v){}};
struct Effects {int Tutorial;};
struct CTCHeroOpinionDeedLog {
 CThing parent,*VillageForCrimes;long NextHandle;bool guards,inform;int effectsCalls,informs;Effects effects;std::map<long,COpinionDeed*> OpinionDeedMap;std::list<CWitnessesSearchingDeed> OpinionDeedsSearchingForWitnesses;
 CThing& GetParentThing(){return parent;}bool GetGuardsIgnoreCrimes(){return guards;}
 COpinionDeed snapshot;CTCHeroOpinionDeedLog():snapshot(other,0,C3DVector(),0,false,false,0){}
 bool InformRelevantThingsOfDeed(long h,CThingAICreature*,EOpinionPostDeedType,const CFaction*,bool){++informs;assert(OpinionDeedMap.find(h)!=OpinionDeedMap.end());snapshot=*OpinionDeedMap[h];return inform;}
 Effects* PeekPDeedEffectsDef(EOpinionDeedType){++effectsCalls;return &effects;}
 long PostDeed(EOpinionDeedType,const C3DVector&,const CThing*,CThingAICreature*,EOpinionPostDeedType,bool,bool,const CFaction*,bool,long);
};
long CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType deed_type,const C3DVector& position,const CThing* target,CThingAICreature* recipient,EOpinionPostDeedType post_type,bool is_crime,bool keep_searching_for_witnesses,const CFaction* pfaction,bool reverse_faction_search,long extra_data)
{
 if(deed_type==OPINION_DEED_TYPE_NONE || post_type==OPINION_POST_DEED_TYPE_NULL)return -1;
 bool allow_tutorial=true;
 const CRegion& region=GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(GetWorldMap().GetMapNumberAt(GetParentThing().GetPos())));
 CDefPointer<const CRegionDef> pregion_def=region.GetPRegionDef();
 if(pregion_def.Get()){
  if(deed_type==OPINION_DEED_TYPE_CRIME_WEAPON_OUT){
   if(!pregion_def.Get()->WeaponOutCrime || !pregion_def.Get()->UseCrimes)return -1;
  }
  if(!pregion_def.Get()->UseCrimes){
   allow_tutorial=false;is_crime=false;
   if(deed_type==OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH)extra_data=-1;
  }
  if(!pregion_def.Get()->DisplayCrimes)allow_tutorial=false;
 }else{
  if(deed_type==OPINION_DEED_TYPE_CRIME_WEAPON_OUT)return -1;
  allow_tutorial=false;is_crime=false;
  if(deed_type==OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH)extra_data=-1;
 }
 if(deed_type==OPINION_DEED_TYPE_CRIME_ASSAULT || deed_type==OPINION_DEED_TYPE_CRIME_GBH || deed_type==OPINION_DEED_TYPE_CRIME_MURDER){
  if(target && !target->HasTC(TCI_CREATURE_OPINION_OF_HERO)){allow_tutorial=false;is_crime=false;}
  CDefPointer<const CEnemyDef> enemy_def;
  if(target && target->GetPBaseDef()->GetSubDef(&enemy_def) && enemy_def->Faction!=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FACTION_VILLAGERS"))allow_tutorial=false;
 }
 if(GetGuardsIgnoreCrimes()==true){is_crime=false;allow_tutorial=false;if(deed_type==OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH)extra_data=-1;}
 CTCVillage* tc_village;
 if(IS_THING_ALIVE(VillageForCrimes) && VillageForCrimes->GetTC(&tc_village) && tc_village->GetVillageLimbo())allow_tutorial=false;
 long new_handle=NextHandle++;
 COpinionDeed* new_deed=new COpinionDeed(deed_type,new_handle,position,target,keep_searching_for_witnesses,is_crime,extra_data);
 OpinionDeedMap.insert(std::make_pair(new_handle,new_deed));
 if(keep_searching_for_witnesses)
  OpinionDeedsSearchingForWitnesses.push_back(CWitnessesSearchingDeed(new_handle,deed_type,recipient,post_type,pfaction,reverse_faction_search));
 if(!InformRelevantThingsOfDeed(new_handle,recipient,post_type,pfaction,reverse_faction_search)&&!keep_searching_for_witnesses){
  delete new_deed;OpinionDeedMap.erase(new_handle);--NextHandle;new_handle=-1;
 }
 if(allow_tutorial && NGlobalConsole::GEnableTutorials && !CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()){
  CTCHero* tc_hero=0;
  if(PeekPDeedEffectsDef(deed_type)->Tutorial && GetParentThing().GetTC(&tc_hero))
   tc_hero->GiveTutorialForCategory(PeekPDeedEffectsDef(deed_type)->Tutorial,false);
 }
 return new_handle;
}
int main(){
 const EOpinionDeedType types[]={OPINION_DEED_TYPE_NONE,OPINION_DEED_TYPE_CRIME_WEAPON_OUT,OPINION_DEED_TYPE_CRIME_ASSAULT,OPINION_DEED_TYPE_CRIME_GBH,OPINION_DEED_TYPE_CRIME_MURDER,OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH,other};
 for(unsigned i=0;i<65536;++i){
  unsigned x=i*2654435761u;
  EOpinionDeedType type=types[i%7];EOpinionPostDeedType mode=(x&31)?post:OPINION_POST_DEED_TYPE_NULL;
  CTCHeroOpinionDeedLog log;CThing target={},village={};CFaction faction;CThingAICreature rec;CTCHero hero={};CEnemyDef enemy;CTCVillage tc;
  CRegionDef rd={(x&128)!=0,(x&256)!=0,(x&512)!=0};world.region.def=(x&1024)?&rd:0;world.calls=0;
  target.opinion=(x&2048)!=0;target.base.enemy=(x&4096)?&enemy:0;enemy.Faction=(x&8192)?13:2;
  const CThing* who=(x&16384)?&target:0;log.guards=(x&32768)!=0;village.alive=(x&65536)!=0;village.village=(x&131072)?&tc:0;tc.limbo=(x&262144)!=0;log.VillageForCrimes=(x&524288)?&village:0;
  bool search=(x&1048576)!=0,crime=(x&2097152)!=0;log.inform=(x&4194304)!=0;NGlobalConsole::GEnableTutorials=(x&8388608)!=0;CPlayerGui::Get().cut=(x&16777216)!=0;log.effects.Tutorial=(x&33554432)?19:0;
  log.parent.hero=(x&67108864)?&hero:0;log.parent.pos.x=2;log.NextHandle=17;log.informs=log.effectsCalls=0;bool reverse=(x&134217728)!=0;
  C3DVector pos={3,5,7};COpinionDeed::deleted=0;long handle=log.PostDeed(type,pos,who,&rec,mode,crime,search,&faction,reverse,123);
  bool invalid=!mode||!type,blocked=type==OPINION_DEED_TYPE_CRIME_WEAPON_OUT&&(!world.region.def||!rd.UseCrimes||!rd.WeaponOutCrime);bool processed=!invalid&&!blocked,keep=processed&&(search||log.inform);
  assert(world.calls==!invalid&&handle==(keep?17:-1)&&log.NextHandle==(keep?18:17)&&log.OpinionDeedMap.size()==unsigned(keep)&&log.informs==processed&&COpinionDeed::deleted==int(processed&&!keep));
  assert(log.OpinionDeedsSearchingForWitnesses.size()==unsigned(processed&&search));
  if(processed&&search){const CWitnessesSearchingDeed& w=log.OpinionDeedsSearchingForWitnesses.front();assert(w.h==17&&w.t==type&&w.r==&rec&&w.mode==mode&&w.f==&faction&&w.reverse==reverse);}
  bool attack=type>=7&&type<=9,safe=world.region.def&&rd.UseCrimes&&!log.guards;
  if(processed){const COpinionDeed& d=log.snapshot;assert(d.type==type&&d.h==17&&d.pos.x==3&&d.pos.y==5&&d.pos.z==7&&d.target==who&&d.search==search);assert(d.crime==(crime&&safe&&!(attack&&who&&!target.opinion)));assert(d.extra==(type==53&&!safe?-1:123));}
  bool tutorial=processed&&safe&&rd.DisplayCrimes&&!(attack&&who&&(!target.opinion||(target.base.enemy&&enemy.Faction!=13)))&&!(log.VillageForCrimes&&village.alive&&village.village&&tc.limbo)&&NGlobalConsole::GEnableTutorials&&!CPlayerGui::Get().cut;
  bool given=tutorial&&log.effects.Tutorial&&log.parent.hero;assert(hero.calls==given&&log.effectsCalls==int(tutorial)+int(given));
  if(keep)delete log.OpinionDeedMap[17];
 }
 puts("006c6990_TEST PASS");return 0;
}
// CTCDivineWrath::CancelForCutscene 0x0078F8F0 -- self-contained C++03 fixture with the production body unchanged (stubbed callees,
// StopSound inlined as the FABLE_RETAIL_CTCDIVINEWRATH_STOPSOUND_INLINE opt-in).
#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
static long cases=0;
#define CHECK(x)do{if(!(x)){std::fprintf(stderr,"case %ld line %d: %s\n",cases,__LINE__,#x);std::exit(1);}}while(0)
static std::vector<long> trace;
static bool fade_kills;
enum ETCInterfaceType{TCI_SOUND_PLAYER=16,TCI_DIVINE_WRATH=133};
class CThing{ public:int id,refs;bool dead;int kills,fades;CThing():id(0),refs(0),dead(false),kills(0),fades(0){}bool IsDead()const{return dead;}void Kill(bool immediately){CHECK(immediately);trace.push_back(300+id);++kills;dead=true;}};
template<class T>class CIntelligentPointer{ public:T*p;CIntelligentPointer(T*q=0):p(q){if(p)++p->refs;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p){if(p)++p->refs;}~CIntelligentPointer(){if(p)--p->refs;}CIntelligentPointer&operator=(const CIntelligentPointer&o){if(o.p)++o.p->refs;if(p)--p->refs;p=o.p;return*this;}T*Get()const{return p;}T*operator->()const{return p;}};
template<class T>bool operator!=(const CIntelligentPointer<T>&p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T&p){return p!=0&&!p->IsDead();}
class CTCSoundPlayer{ public:unsigned long last;int stops;CTCSoundPlayer():last(0),stops(0){}void StopAttachedSound(unsigned long id){trace.push_back(500);last=id;++stops;}};
class CTCSpecialAbilities{ public:bool AbilityMutexLock;long CurrentAbility;CTCSpecialAbilities():AbilityMutexLock(true),CurrentAbility(123){}void UnLockAbilitiyMutex(){trace.push_back(210);AbilityMutexLock=false;CurrentAbility=0;}static void FadeOrKillEffect(CIntelligentPointer<CThing>&p){CHECK(p.Get()!=0);trace.push_back(400+p->id);++p->fades;if(fade_kills)p->dead=true;}};
class CThingCreatureBase:public CThing{ public:CTCSpecialAbilities*abilities;CTCSoundPlayer*sound;int finished,removed;CThingCreatureBase():abilities(0),sound(0),finished(0),removed(0){}void FinishCurrentAction(){trace.push_back(200);++finished;}bool GetTC(CTCSpecialAbilities**p){if(!abilities)return false;*p=abilities;return true;}bool GetTC(ETCInterfaceType type,CTCSoundPlayer**p){CHECK(type==TCI_SOUND_PLAYER);if(!sound)return false;*p=sound;return true;}void RemoveTC(ETCInterfaceType type){CHECK(type==TCI_DIVINE_WRATH);trace.push_back(600);++removed;}};
class CEnvironment{ public:int changes;CEnvironment():changes(0){}void TransitionToDefaultThemeAllChannels(long priority,float duration){CHECK(priority==2&&duration==0.001f);trace.push_back(100);++changes;}};
class CWorld{ public:CEnvironment env;CEnvironment&GetEnvironment(){return env;}};
class CTCDivineWrath{ public:struct PerCreatureInfo{CIntelligentPointer<CThingCreatureBase>ActualCreature;};std::map<CThingCreatureBase*,PerCreatureInfo>LastFrameCreaturesHit;bool CutSceneCancel;CIntelligentPointer<CThing>BuildUpEffect,HandEffect1,HandEffect2;unsigned long LoopingSound;CThingCreatureBase&parent;CWorld&world;CTCDivineWrath(CThingCreatureBase&p,CWorld&w):CutSceneCancel(false),LoopingSound(0),parent(p),world(w){}CThingCreatureBase&GetParentThing(){return parent;}CWorld&GetWorld(){return world;}void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase>p){CHECK(p.Get()!=0&&p->refs==3);trace.push_back(10+p->id);}void StopSound(unsigned long sound){CTCSoundPlayer*ptc_sound_player=0;if(GetParentThing().GetTC(TCI_SOUND_PLAYER,&ptc_sound_player))ptc_sound_player->StopAttachedSound(sound);}void CancelForCutscene();};

void CTCDivineWrath::CancelForCutscene()
{
 std::map<CThingCreatureBase*,PerCreatureInfo>::iterator endit=LastFrameCreaturesHit.end();
 for(std::map<CThingCreatureBase*,PerCreatureInfo>::iterator it=LastFrameCreaturesHit.begin();it!=endit;++it) {
  CIntelligentPointer<CThingCreatureBase> something=it->second.ActualCreature;
  ResetCreatureHitState(something);
 }
 CutSceneCancel=true;
 GetWorld().GetEnvironment().TransitionToDefaultThemeAllChannels(2,0.001f);
 CThingCreatureBase& parent_creature=static_cast<CThingCreatureBase&>(GetParentThing());
 parent_creature.FinishCurrentAction();
 CTCSpecialAbilities* ptc_spec_abils;
 if(parent_creature.GetTC(&ptc_spec_abils))ptc_spec_abils->UnLockAbilitiyMutex();
 if(IS_THING_ALIVE(BuildUpEffect))BuildUpEffect->Kill(true);
 if(IS_THING_ALIVE(HandEffect1))CTCSpecialAbilities::FadeOrKillEffect(HandEffect1);
 if(IS_THING_ALIVE(HandEffect2))CTCSpecialAbilities::FadeOrKillEffect(HandEffect2);
 StopSound(LoopingSound);
 parent_creature.RemoveTC(TCI_DIVINE_WRATH);
}
static void run_case(const int indices[4],unsigned dead_mask,int components,int n,unsigned long sample,bool kill_on_fade){
 ++cases;trace.clear();fade_kills=kill_on_fade;
 CThing effects[4];for(int i=0;i<4;++i){effects[i].id=i;effects[i].dead=(dead_mask&(1u<<i))!=0;}
 CThingCreatureBase parent,creatures[5];CTCSpecialAbilities abilities;CTCSoundPlayer sound;CWorld world;CTCDivineWrath power(parent,world);
 parent.abilities=(components&1)?&abilities:0;parent.sound=(components&2)?&sound:0;power.LoopingSound=sample;
 for(int i=n-1;i>=0;--i){creatures[i].id=i;power.LastFrameCreaturesHit[&creatures[i]].ActualCreature=CIntelligentPointer<CThingCreatureBase>(&creatures[i]);}
 CIntelligentPointer<CThing>*slots[3]={&power.BuildUpEffect,&power.HandEffect1,&power.HandEffect2};
 int expected_refs[4]={0,0,0,0};for(int i=0;i<3;++i)if(indices[i]>=0){*slots[i]=CIntelligentPointer<CThing>(&effects[indices[i]]);++expected_refs[indices[i]];}
 std::vector<long>wanted;for(int i=0;i<n;++i)wanted.push_back(10+i);wanted.push_back(100);wanted.push_back(200);if(components&1)wanted.push_back(210);
 bool dead[4];int kills[4]={0,0,0,0},fades[4]={0,0,0,0};for(int i=0;i<4;++i)dead[i]=effects[i].dead;
 for(int i=0;i<3;++i){int k=indices[i];if(k<0||dead[k])continue;if(i==0){wanted.push_back(300+k);++kills[k];dead[k]=true;}else{wanted.push_back(400+k);++fades[k];if(kill_on_fade)dead[k]=true;}}
 if(components&2)wanted.push_back(500);wanted.push_back(600);
 power.CancelForCutscene();
 CHECK(trace==wanted);CHECK(power.CutSceneCancel);CHECK(world.env.changes==1&&parent.finished==1&&parent.removed==1);CHECK(abilities.AbilityMutexLock==((components&1)==0));CHECK(abilities.CurrentAbility==((components&1)?0:123));CHECK(sound.stops==((components&2)?1:0));if(components&2)CHECK(sound.last==sample);
 CHECK(power.LastFrameCreaturesHit.size()==(unsigned)n);for(int i=0;i<n;++i)CHECK(creatures[i].refs==1);
 for(int i=0;i<4;++i)CHECK(effects[i].dead==dead[i]&&effects[i].kills==kills[i]&&effects[i].fades==fades[i]&&effects[i].refs==expected_refs[i]);
 for(int i=0;i<3;++i){CHECK(slots[i]->Get()==(indices[i]<0?0:&effects[indices[i]]));}
}
int main(){
 const unsigned long samples[3]={0,17,0xffffffffUL};
 for(int pattern=0;pattern<81;++pattern)for(int components=0;components<4;++components)for(int n=0;n<5;++n)for(int sample=0;sample<3;++sample)for(int fk=0;fk<2;++fk){int q=pattern,indices[4];unsigned dead=0;for(int i=0;i<4;++i){int state=q%3;q/=3;indices[i]=state?i:-1;if(state==1)dead|=1u<<i;}run_case(indices,dead,components,n,samples[sample],fk!=0);}
 for(int pattern=0;pattern<81;++pattern)for(unsigned dead=0;dead<4;++dead)for(int components=0;components<4;++components)for(int n=0;n<3;++n)for(int sample=0;sample<3;++sample)for(int fk=0;fk<2;++fk){int q=pattern,indices[4];for(int i=0;i<4;++i){indices[i]=q%3-1;q/=3;}run_case(indices,dead,components,n,samples[sample],fk!=0);}
 std::puts("0078f8f0_TEST PASS");return 0;
}
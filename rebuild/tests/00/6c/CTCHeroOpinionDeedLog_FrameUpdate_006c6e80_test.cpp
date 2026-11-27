#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <map>
#include <vector>

static int assertArms;
#define FABLE_RETAIL_ASSERT_ARM(c) ((void)(++assertArms))
typedef unsigned __int64 U64;
enum EOpinionDeedType {ordinary,OPINION_DEED_TYPE_CRIME_LOCKPICKING=4,OPINION_DEED_TYPE_CRIME_THEFT=6};
struct CMainGameComponent {static long fps;static long GetConstantFPS(){return fps;}};
long CMainGameComponent::fps;
struct CWorld {static long frame;static long GetFrame(){return frame;}};
long CWorld::frame;
struct COpinionDeedEffectsDef {long KnowledgeLifeInSeconds;};
struct COpinionDeed {
 long DeedHandle,FrameHappened;bool IsStillHappening,IsCrimeReportedToVillage;EOpinionDeedType DeedType;
 static unsigned deleted;
 COpinionDeed(long h,long f,bool s,bool crime,EOpinionDeedType type):DeedHandle(h),FrameHappened(f),IsStillHappening(s),IsCrimeReportedToVillage(crime),DeedType(type){}
 ~COpinionDeed(){deleted|=1u<<DeedHandle;}
};
unsigned COpinionDeed::deleted;
struct CFaction {};
struct CThingAICreature {};
struct RecipientPointer {CThingAICreature* p;CThingAICreature* Get()const{return p;}};
struct CWitnessesSearchingDeed {long DeedHandle;RecipientPointer Recipient;int PostType;const CFaction* PFaction;bool ReverseFactionSearch;};
struct CTCVillage {long seen;long GetFramePlayerLastSeenByGuard()const{return seen;}};
namespace NEntityEvents {
 enum EEventType {EVENT_HERO_HAS_BEEN_SPOTTED_STEALING=21};
 class CEventHeroHasBeenSpottedPickpocketing;
 class CEventHeroHasBeenSpottedStealing {};
 struct CIsEventOfType {EEventType type;CIsEventOfType(EEventType t):type(t){}};
}
struct CTCEntityEvents {
 NEntityEvents::CEventHeroHasBeenSpottedStealing ev;bool containsSteal;unsigned queries,removed;
 bool GetEvent(NEntityEvents::CEventHeroHasBeenSpottedStealing** out,const NEntityEvents::CIsEventOfType& filter){
  ++queries;assert(filter.type==NEntityEvents::EVENT_HERO_HAS_BEEN_SPOTTED_STEALING);if(containsSteal)*out=&ev;return containsSteal;}
 void RemoveEvent(NEntityEvents::CEventHeroHasBeenSpottedStealing& e){assert(&e==&ev&&containsSteal);++removed;containsSteal=false;}
};
struct CThing {
 U64 uid;CTCVillage* village;CTCEntityEvents* events;U64 GetUniqueID()const{return uid;}
 bool GetTC(CTCVillage** p)const{return (*p=village)!=0;}
 bool GetTC(CTCEntityEvents** p)const{return (*p=events)!=0;}
};
struct CThingPlayerCreature {bool alive,weapon;};
static bool IS_THING_ALIVE(const CThingPlayerCreature* p){return p&&p->alive;}
struct CTCCarrying {static unsigned count;static bool IsThingCarryingWeapon(const CThingPlayerCreature& p){++count;return p.weapon;}};
unsigned CTCCarrying::count;
struct CPlayer {CThingPlayerCreature* player;CThingPlayerCreature* GetPControlledCreature(){return player;}};
struct CPlayerManager {CPlayer main;CPlayer& GetMainPlayer(){return main;}};
struct CQuestManager {
 bool steal,pick;int sq,pq;static CQuestManager& Get(){static CQuestManager q;return q;}
 bool IsQuestActive(const char* name){if(!strcmp(name,"Expression_Steal")){++sq;return steal;}assert(!strcmp(name,"Expression_Picklock"));++pq;return pick;}
};
static CFaction faction;static CThingAICreature recipient;
struct CTCHeroOpinionDeedLog {
 CThing parent,*VillageForCrimes;long WeaponOutDeedHandle;
 std::map<long,COpinionDeed*> OpinionDeedMap;std::list<CWitnessesSearchingDeed> OpinionDeedsSearchingForWitnesses;
 CPlayerManager pm;COpinionDeedEffectsDef effects;
 unsigned ended,putAway,scabbard,unsheathed,defCalls;std::vector<long> informed;
 CTCHeroOpinionDeedLog():VillageForCrimes(0),WeaponOutDeedHandle(-1),ended(0),putAway(0),scabbard(0),unsheathed(0),defCalls(0){}
 void FrameUpdate();CThing& GetParentThing(){return parent;}CPlayerManager& GetPlayerManager(){return pm;}
 COpinionDeedEffectsDef* PeekPDeedEffectsDef(EOpinionDeedType){++defCalls;return &effects;}
 void RemoveDeedStillSearchingForWitnesses(long handle){
  ended|=1u<<handle;
  if(OpinionDeedMap.count(handle)&&OpinionDeedMap[handle])OpinionDeedMap[handle]->IsStillHappening=false;
  for(std::list<CWitnessesSearchingDeed>::iterator i=OpinionDeedsSearchingForWitnesses.begin();i!=OpinionDeedsSearchingForWitnesses.end();++i)
   if(i->DeedHandle==handle){OpinionDeedsSearchingForWitnesses.erase(i);break;}}
 void InformRelevantThingsOfDeed(long h,CThingAICreature* r,int post,const CFaction* f,bool reverse){
  assert(r==&recipient&&f==&faction&&post==h+20&&reverse==bool(h%2));informed.push_back(h);}
 const COpinionDeed* GetDeedFromHandle(long h) const {
  std::map<long,COpinionDeed*>::const_iterator i=OpinionDeedMap.find(h);return i==OpinionDeedMap.end()?0:i->second;}
 void PlayerTakenOutWeapon(){++unsheathed;WeaponOutDeedHandle=9;}
 void PlayerPutWeaponAway(){++putAway;if(WeaponOutDeedHandle!=-1){++scabbard;WeaponOutDeedHandle=-1;}}
};
void CTCHeroOpinionDeedLog::FrameUpdate()
{
    const long SECONDS_BETWEEN_CLEAROUTS=3;
    long constant_fps=CMainGameComponent::GetConstantFPS();
    unsigned __int64 val=GetParentThing().GetUniqueID()+CWorld::GetFrame();
    if (!(val % static_cast<unsigned long>(SECONDS_BETWEEN_CLEAROUTS*constant_fps))) {
        typedef std::map<long,COpinionDeed*>::iterator Iter;
        if (false) {} else for (Iter it=OpinionDeedMap.begin(),end_it=OpinionDeedMap.end(),next_it=it;it!=end_it;it=next_it) {
            ++next_it;
            COpinionDeed* opinion_deed=it->second;
            if (!opinion_deed->IsStillHappening) {
                long effective_frame_happened=opinion_deed->FrameHappened;
                if(opinion_deed->IsCrimeReportedToVillage && VillageForCrimes!=0) {
                    CTCVillage* ptc_village=0;
                    if(!VillageForCrimes->GetTC(&ptc_village)) { FABLE_RETAIL_ASSERT_ARM(false); }
                    if(ptc_village->GetFramePlayerLastSeenByGuard()>effective_frame_happened)
                        effective_frame_happened=ptc_village->GetFramePlayerLastSeenByGuard();
                }
                if(effective_frame_happened+PeekPDeedEffectsDef(opinion_deed->DeedType)->KnowledgeLifeInSeconds*constant_fps<CWorld::GetFrame()) {
                    delete it->second;
                    OpinionDeedMap.erase(it);
                }
            } else if ((opinion_deed->DeedType==OPINION_DEED_TYPE_CRIME_THEFT && !CQuestManager::Get().IsQuestActive("Expression_Steal"))
                   || (opinion_deed->DeedType==OPINION_DEED_TYPE_CRIME_LOCKPICKING && !CQuestManager::Get().IsQuestActive("Expression_Picklock"))) {
                FABLE_RETAIL_ASSERT_ARM(false);
                RemoveDeedStillSearchingForWitnesses(opinion_deed->DeedHandle);
            }
        }
    }
    const long SECONDS_BETWEEN_WITNESS_CHECKS=1;
    val+=5;
    if (!(val % static_cast<unsigned long>(constant_fps*SECONDS_BETWEEN_WITNESS_CHECKS))) {
        typedef std::list<CWitnessesSearchingDeed>::iterator Iter;
        if (false) {} else for (Iter it=OpinionDeedsSearchingForWitnesses.begin(),end_it=OpinionDeedsSearchingForWitnesses.end();it!=end_it;++it)
            InformRelevantThingsOfDeed(it->DeedHandle,it->Recipient.Get(),it->PostType,it->PFaction,it->ReverseFactionSearch);
    }
    const long SECONDS_BETWEEN_WITNESS_CHECK_CLEAROUT=300;
    val+=5;
    if (!(val % static_cast<unsigned long>(constant_fps*SECONDS_BETWEEN_WITNESS_CHECK_CLEAROUT))) {
        typedef std::list<CWitnessesSearchingDeed>::iterator Iter;
        if (false) {} else for (Iter it=OpinionDeedsSearchingForWitnesses.begin(),end_it=OpinionDeedsSearchingForWitnesses.end(),next_it=it;it!=end_it;it=next_it) {
            ++next_it;
            if(!GetDeedFromHandle(it->DeedHandle))
                OpinionDeedsSearchingForWitnesses.erase(it);
        }
    }
    CTCEntityEvents* ptc_entity_events;
    if(GetParentThing().GetTC(&ptc_entity_events)) {
        NEntityEvents::CEventHeroHasBeenSpottedPickpocketing* pevent=0;
        NEntityEvents::CEventHeroHasBeenSpottedStealing* psteal_event=0;
        if(ptc_entity_events->GetEvent(&psteal_event,NEntityEvents::CIsEventOfType(NEntityEvents::EVENT_HERO_HAS_BEEN_SPOTTED_STEALING)))
        {
            FABLE_RETAIL_ASSERT_ARM(false);
            ptc_entity_events->RemoveEvent(*psteal_event);
        }
    }
    CThingPlayerCreature* pplayer=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if(IS_THING_ALIVE(pplayer)) {
        bool has_weapon_out=CTCCarrying::IsThingCarryingWeapon(*pplayer);
        if (WeaponOutDeedHandle!=-1 && OpinionDeedMap.find(WeaponOutDeedHandle)==OpinionDeedMap.end())
            WeaponOutDeedHandle=-1;
        if(has_weapon_out) {
            if(WeaponOutDeedHandle==-1)PlayerTakenOutWeapon();
        } else PlayerPutWeaponAway();
    }
}
static void run(unsigned bits,unsigned tick){
 CTCHeroOpinionDeedLog log;CThing village={};CTCVillage tc={};CThingPlayerCreature hero={};CTCEntityEvents ev={};
 CQuestManager& q=CQuestManager::Get();q.sq=q.pq=0;q.steal=(bits&1)!=0;q.pick=(bits&2)!=0;
 bool reported=(bits&4)!=0,hasVillage=(bits&8)!=0,validHero=(bits&16)!=0,alive=(bits&32)!=0,weapon=(bits&64)!=0;
 bool hasEvents=(bits&128)!=0,hasSteal=(bits&256)!=0;hero.alive=alive;hero.weapon=weapon;
 log.pm.main.player=validHero?&hero:0;log.parent.events=hasEvents?&ev:0;ev.containsSteal=hasSteal;
 CMainGameComponent::fps=tick%5+1;long fps=CMainGameComponent::fps;
 CWorld::frame=6000;long now=CWorld::frame;log.effects.KnowledgeLifeInSeconds=2;
 U64 val;
 switch(tick%8){
 case 0:val=U64(fps)*300*8-10;break;
 case 1:val=U64(fps)*300*8-5;break;
 case 2:val=U64(fps)*300*8;break;
 case 3:val=U64(-1)-4;break;
 case 4:val=U64(-1)-9;break;
 case 5:val=(U64(1)<<40)+fps*300-10;break;
 case 6:val=(U64(1)<<40)+2;break;
 default:val=tick;break;
 }
 log.parent.uid=val-U64(now);tc.seen=(bits&512)?now:now-5000;village.village=&tc;
 log.VillageForCrimes=hasVillage?&village:0;CTCCarrying::count=0;assertArms=0;
 const long when[]={0,now-4000,now-fps*2,now-5000,now-5000,now-5000};
 const EOpinionDeedType types[]={ordinary,ordinary,ordinary,OPINION_DEED_TYPE_CRIME_THEFT,OPINION_DEED_TYPE_CRIME_LOCKPICKING,ordinary};
 for(int k=1;k<=5;++k)log.OpinionDeedMap[k]=new COpinionDeed(k,when[k],k>=3,reported,types[k]);
 bool clear=(val%unsigned(fps*3))==0,notify=((val+5)%unsigned(fps))==0,prune=((val+10)%unsigned(fps*300))==0;
 // Null data can occur in a stale map entry. The periodic knowledge walk requires live deeds.
 if(!clear)log.OpinionDeedMap[6]=0;
 for(int h=1;h<=7;++h){CWitnessesSearchingDeed w;w.DeedHandle=h;w.Recipient.p=&recipient;w.PostType=h+20;w.PFaction=&faction;w.ReverseFactionSearch=(h%2)!=0;log.OpinionDeedsSearchingForWitnesses.push_back(w);}
 int initialHandle=(tick%3==0)?-1:(tick%3==1?1:8);log.WeaponOutDeedHandle=initialHandle;COpinionDeed::deleted=0;
 log.FrameUpdate();
 long effective=(reported&&hasVillage&&tc.seen>when[1])?tc.seen:when[1];
 bool expired=clear&&effective+2*fps<now;
 unsigned removals=(clear&&!q.steal?1u<<3:0)|(clear&&!q.pick?1u<<4:0);
 assert(log.defCalls==(clear?2u:0u)&&q.sq==int(clear)&&q.pq==int(clear));
 assert(COpinionDeed::deleted==(expired?2u:0u)&&log.OpinionDeedMap.count(1)==unsigned(!expired)&&log.OpinionDeedMap.count(2)==1);
 assert(log.ended==removals);
 size_t ordinal=0;std::list<CWitnessesSearchingDeed>::const_iterator it=log.OpinionDeedsSearchingForWitnesses.begin();
 for(int h=1;h<=7;++h){if(removals&(1u<<h))continue;
  if(notify){assert(ordinal<log.informed.size()&&log.informed[ordinal]==h);++ordinal;}
  bool keep=!prune||(h<=5&&!(h==1&&expired));
  if(keep){assert(it!=log.OpinionDeedsSearchingForWitnesses.end()&&it->DeedHandle==h);++it;}}
 assert(ordinal==log.informed.size()&&it==log.OpinionDeedsSearchingForWitnesses.end());
 assert(ev.queries==unsigned(hasEvents)&&ev.removed==unsigned(hasEvents&&hasSteal));
 assert(CTCCarrying::count==unsigned(validHero&&alive));
 bool handlePresent=initialHandle==1&&!expired;unsigned draw=validHero&&alive&&weapon&&!handlePresent;
 assert(log.unsheathed==draw&&log.putAway==unsigned(validHero&&alive&&!weapon)&&log.scabbard==unsigned(validHero&&alive&&!weapon&&handlePresent));
 assert(log.WeaponOutDeedHandle==(!validHero||!alive?initialHandle:weapon?(handlePresent?1:9):-1));
 for(std::map<long,COpinionDeed*>::iterator jt=log.OpinionDeedMap.begin();jt!=log.OpinionDeedMap.end();++jt)delete jt->second;
}
int main(){for(unsigned b=0;b<1024;++b)for(unsigned t=0;t<40;++t)run(b,t);puts("006c6e80_TEST PASS");return 0;}
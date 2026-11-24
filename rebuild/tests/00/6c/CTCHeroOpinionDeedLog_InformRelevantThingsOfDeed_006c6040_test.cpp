// This fixture exercises the selected function with observable component and search-service doubles.
#include <stdio.h>
#include <string.h>
#include <vector>
#include <list>
#include <algorithm>
#include "engine/EOpinionDeedType.h"
#include "engine/EOpinionPostDeedType.h"
const int TCI_GUARD=9;
struct CThing{
 void* components[12];int kind;bool dead;int pos;
 CThing():kind(1),dead(false),pos(1){memset(components,0,sizeof(components));}
 int GetType()const{return kind;}int GetPos()const{return pos;}
 template<class T>bool GetTC(T** p)const{*p=(T*)components[T::id];return *p!=0;}
 template<class T>bool PeekTC(const T** p)const{*p=(T*)components[T::id];return *p!=0;}
};
struct CThingBuilding:CThing{bool IsPointInside(int p)const{return p==1;}};
struct CThingAICreature:CThing{CThingBuilding* home;CThingBuilding* work;CThingAICreature():home(0),work(0){} CThingBuilding* GetHomeBuilding()const{return home;}CThingBuilding* GetWorkBuilding()const{return work;}};
template<class T>struct CIntelligentPointer{T* p;T* Get()const{return p;}T* operator->()const{return p;}};
template<class T>bool IS_THING_ALIVE(T* p){return p&&!p->dead;}
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>& h){return IS_THING_ALIVE(h.Get());}
struct CFaction{};
struct CTCOwnedEntity{enum{id=1};CThing* owner;CThing* GetPOwner()const{return owner;}};
struct CTCVolumeContainmentTracker{enum{id=2};CThing* inside;CThing* GetPThingInside()const{return inside;}};
struct CTCVillage{std::list<CIntelligentPointer<CThingAICreature> > VillagePeople;CThing parent; const std::list<CIntelligentPointer<CThingAICreature> >& PeekVillagePeople()const{return VillagePeople;} CThing& GetParentThing(){return parent;}};
struct CTCVillageMember{enum{id=3};CTCVillage* village;CTCVillage* GetPVillage()const{return village;}};
struct CTCSleep{enum{id=4};bool awake;void SetIsNotReadyToSleep(){awake=true;}};
struct CTCEnemy{enum{id=5};bool enemy,ally;bool IsEnemyOfFaction(const CFaction*)const{return enemy;}bool IsAlliedWithFaction(const CFaction*)const{return ally;}};
struct Effects{bool CanBeHeard;};
struct CTCCreatureOpinionOfHero{enum{id=6};unsigned char mask[100];int calls;bool unknown;
 CTCCreatureOpinionOfHero():calls(0),unknown(false){memset(mask,1,sizeof(mask));}
 bool IsDeedTypeEnabled(int type) const {return mask[type]!=0;}
 bool KnowsOfDeed(long,int,bool)const{return calls>0;}
 void InformOfDeed(int,long,const Effects&,bool u){++calls;unknown=u;}
};
struct COpinionDeed{EOpinionDeedType DeedType;CIntelligentPointer<const CThing> PTarget;};
static bool recipient_eligible;
namespace NGlobalConsole {bool EnableUnknownThreatDeedPostings;}
struct CThingFilter_IsEligibleWitness{
 bool unknown;
 CThingFilter_IsEligibleWitness(const COpinionDeed*,CThing&,bool,CThingAICreature*,const CFaction*,bool,bool u):unknown(u){}
 bool operator()(const CThing&)const{return recipient_eligible;}
};
struct CThingFilter_HasTC{explicit CThingFilter_HasTC(int){}};
struct Services{
 std::list<CThingAICreature*> regular,unknown;std::vector<CThing*> guards;
 Services& GetThingSearchTools(){return *this;}
 template<class T>void GetAllThings(const CThingFilter_IsEligibleWitness& f,std::list<T*>& dst){dst=f.unknown?unknown:regular;}
 template<class T>void GetAllThings(const CThingFilter_HasTC&,std::vector<T*>& dst){dst=guards;}
};
struct CTCHeroOpinionDeedLog{
 COpinionDeed deed;CThing self;CThing* ParentThing;CTCHeroOpinionDeedLog():ParentThing(&self){} Services search;Effects effects;
 const COpinionDeed* GetDeedFromHandle(long h)const{return h==7?&deed:0;}
 CThing& GetParentThing(){return self;}Services& GetWorld(){return search;}Services& PeekThingSearchTools(){return search;}
 const Effects* PeekPDeedEffectsDef(int)const{return &effects;}
 bool InformRelevantThingsOfDeed(long,CThingAICreature*,EOpinionPostDeedType,const CFaction*,bool);
};
bool CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed(long deed_handle, CThingAICreature* recipient, EOpinionPostDeedType post_type,
                                                       const CFaction* pfaction, bool reverse_faction_search)
{
    bool ret = false;
    const COpinionDeed* opinion_deed = GetDeedFromHandle(deed_handle);
    if (opinion_deed) {
        EOpinionDeedType deed_type = opinion_deed->DeedType;
        std::list<CThingAICreature*> informees;
        std::list<CThingAICreature*> unknown_threat_informees;
        if (post_type & OPINION_POST_DEED_TYPE_WITNESSES) {
            PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_IsEligibleWitness(opinion_deed, *ParentThing,
                PeekPDeedEffectsDef(deed_type)->CanBeHeard, recipient, pfaction, reverse_faction_search, false), informees);
            CThingBuilding* inform_buildings_villagers = 0;
            if (deed_type == OPINION_DEED_TYPE_CRIME_VANDALISM) {
                const CTCOwnedEntity* ptc_owned;
                if (opinion_deed->PTarget.Get() && opinion_deed->PTarget->PeekTC(&ptc_owned) &&
                    ptc_owned->GetPOwner() && ptc_owned->GetPOwner()->GetType() == 3)
                    inform_buildings_villagers = static_cast<CThingBuilding*>(ptc_owned->GetPOwner());
            } else if (deed_type == OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH || deed_type == OPINION_DEED_TYPE_CRIME_MURDER) {
                const CTCVolumeContainmentTracker* tracker;
                if (IS_THING_ALIVE(opinion_deed->PTarget) && opinion_deed->PTarget->PeekTC(&tracker) &&
                    IS_THING_ALIVE(tracker->GetPThingInside()) && tracker->GetPThingInside()->GetType() == 3)
                    inform_buildings_villagers = static_cast<CThingBuilding*>(tracker->GetPThingInside());
            }
            CTCVillageMember* ptc_village_member;
            if (inform_buildings_villagers && inform_buildings_villagers->GetTC(&ptc_village_member)) {
                CTCVillage* ptc_village = ptc_village_member->GetPVillage();
                if (ptc_village) {
                    const std::list<CIntelligentPointer<CThingAICreature> >& peeps = ptc_village->PeekVillagePeople();
                    for (std::list<CIntelligentPointer<CThingAICreature> >::const_iterator it = peeps.begin();
                         it != peeps.end(); ++it) {
                        CThingAICreature* creature = it->Get();
                        if (IS_THING_ALIVE(creature)
                            && (creature->GetWorkBuilding() == inform_buildings_villagers || creature->GetHomeBuilding() == inform_buildings_villagers)
                            && inform_buildings_villagers->IsPointInside(creature->GetPos())) {
                            if (std::find(informees.begin(), informees.end(), creature) == informees.end())
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                                informees.push_back(creature);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                            CTCSleep* tc_sleep;
                            if (creature->GetTC(&tc_sleep))
                                tc_sleep->SetIsNotReadyToSleep();
                        }
                    }
                }
            }
        }
        bool recipient_receiving_unknown_threat_posting = false;
        if (NGlobalConsole::EnableUnknownThreatDeedPostings && (deed_type == OPINION_DEED_TYPE_CRIME_ASSAULT || deed_type == OPINION_DEED_TYPE_CRIME_GBH || deed_type == OPINION_DEED_TYPE_CRIME_MURDER
                                            || deed_type == OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH)) {
            if (post_type & OPINION_POST_DEED_TYPE_WITNESSES)
                PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_IsEligibleWitness(opinion_deed, *ParentThing,
                    PeekPDeedEffectsDef(deed_type)->CanBeHeard, recipient, pfaction, reverse_faction_search, true), unknown_threat_informees);
            if ((post_type & OPINION_POST_DEED_TYPE_RECIPIENT) && IS_THING_ALIVE(recipient)) {
                CThingFilter_IsEligibleWitness filter(opinion_deed, *ParentThing, PeekPDeedEffectsDef(deed_type)->CanBeHeard, NULL, pfaction,
                                                      reverse_faction_search, true);
                if (filter(*recipient)) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    unknown_threat_informees.push_back(recipient);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    recipient_receiving_unknown_threat_posting = true;
                }
            }
        }
        if (IS_THING_ALIVE(recipient) && !recipient_receiving_unknown_threat_posting) {
            bool faction_ok = true;
            if (pfaction) {
                faction_ok = false;
                const CTCEnemy* ptc_enemy;
                if (recipient->PeekTC(&ptc_enemy))
                    faction_ok = reverse_faction_search ? ptc_enemy->IsEnemyOfFaction(pfaction) : ptc_enemy->IsAlliedWithFaction(pfaction);
            }
            if (faction_ok) {
                CTCCreatureOpinionOfHero* ptc_opinion;
                if (recipient->GetTC(&ptc_opinion) && ptc_opinion && ptc_opinion->IsDeedTypeEnabled(deed_type))
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    informees.push_back(recipient);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        if (post_type & OPINION_POST_DEED_TYPE_GUARDS) {
            std::vector<CThing*> things;
            GetWorld().GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(TCI_GUARD), things);
            for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
                if (IS_THING_ALIVE(*it) && (*it)->GetType() == 1) {
                    CThingAICreature* guard = static_cast<CThingAICreature*>(*it);
                    if (std::find(informees.begin(), informees.end(), guard) == informees.end())
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        informees.push_back(guard);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
            }
        }
        if (!informees.empty()) {
            CThing* pvillage = 0;
            for (std::list<CThingAICreature*>::iterator it = informees.begin(), end_it = informees.end(); it != end_it; ++it) {
                if (((post_type & OPINION_POST_DEED_TYPE_RECIPIENT) && *it == recipient) || ((post_type & OPINION_POST_DEED_TYPE_WITNESSES) && *it != recipient)) {
                    CTCCreatureOpinionOfHero* ptc_opinion;
                    if ((*it)->GetTC(&ptc_opinion) && !ptc_opinion->KnowsOfDeed(deed_handle, deed_type, true)) {
                        ptc_opinion->InformOfDeed(deed_type, deed_handle, *PeekPDeedEffectsDef(deed_type), false);
                        ret = true;
                    }
                }
                if (post_type & OPINION_POST_DEED_TYPE_VILLAGE) {
                    const CTCVillageMember* ptc_village_member;
                    if ((*it)->PeekTC(&ptc_village_member)) {
                        CTCVillage* ptc_village = ptc_village_member->GetPVillage();
                        if (ptc_village) {
                            CTCCreatureOpinionOfHero* ptc_opinion;
                            if (ptc_village->GetParentThing().GetTC(&ptc_opinion) && !ptc_opinion->KnowsOfDeed(deed_handle, deed_type, true)) {
                                ptc_opinion->InformOfDeed(deed_type, deed_handle, *PeekPDeedEffectsDef(deed_type), false);
                                ret = true;
                            }
                        }
                    }
                }
            }
        }
        if (NGlobalConsole::EnableUnknownThreatDeedPostings && !unknown_threat_informees.empty()) {
            for (std::list<CThingAICreature*>::iterator it = unknown_threat_informees.begin(), end_it = unknown_threat_informees.end(); it != end_it; ++it) {
                CTCCreatureOpinionOfHero* ptc_opinion;
                if ((*it)->GetTC(&ptc_opinion) && !ptc_opinion->KnowsOfDeed(deed_handle, deed_type, true)) {
                    ptc_opinion->InformOfDeed(deed_type, deed_handle, *PeekPDeedEffectsDef(deed_type), true);
                    ret = true;
                }
            }
        }
    }
    return ret;
}
int main(){
 CTCHeroOpinionDeedLog log;CThingAICreature recipient,witness;CThing target;
 CTCCreatureOpinionOfHero r,w,v;recipient.components[6]=&r;witness.components[6]=&w;
 CTCSleep sleep={false};witness.components[4]=&sleep;CTCEnemy e={false,false};recipient.components[5]=&e;
 CFaction faction;CTCVillage village;CTCVillageMember member={&village};village.parent.components[6]=&v;
 CThingBuilding building;building.kind=3;building.components[3]=&member;witness.components[3]=&member;
 CIntelligentPointer<CThingAICreature> person={&witness};village.VillagePeople.push_back(person);witness.home=&building;
 CTCOwnedEntity owned={&building};CTCVolumeContainmentTracker inside={&building};target.components[1]=&owned;target.components[2]=&inside;
 log.deed.PTarget.p=&target;log.deed.DeedType=static_cast<EOpinionDeedType>(1);log.effects.CanBeHeard=true;
 if(log.InformRelevantThingsOfDeed(-1,0,static_cast<EOpinionPostDeedType>(31),0,false))return 1;
 if(log.InformRelevantThingsOfDeed(7,&recipient,static_cast<EOpinionPostDeedType>(1),&faction,false))return 2;
 e.ally=true;if(!log.InformRelevantThingsOfDeed(7,&recipient,static_cast<EOpinionPostDeedType>(1),&faction,false)||r.calls!=1)return 3;
 if(log.InformRelevantThingsOfDeed(7,&recipient,static_cast<EOpinionPostDeedType>(1),0,false))return 4;
 r.calls=0;r.mask[1]=0;if(log.InformRelevantThingsOfDeed(7,&recipient,static_cast<EOpinionPostDeedType>(1),0,false))return 5;
 log.deed.DeedType=OPINION_DEED_TYPE_CRIME_VANDALISM;
 if(!log.InformRelevantThingsOfDeed(7,0,static_cast<EOpinionPostDeedType>(6),0,false)||!sleep.awake||w.calls!=1||v.calls!=1)return 6;
 w.calls=0;sleep.awake=false;log.deed.DeedType=static_cast<EOpinionDeedType>(9);
 if(!log.InformRelevantThingsOfDeed(7,0,static_cast<EOpinionPostDeedType>(2),0,false)||!sleep.awake)return 7;
 w.calls=0;witness.pos=0;if(log.InformRelevantThingsOfDeed(7,0,static_cast<EOpinionPostDeedType>(2),0,false))return 8;
 log.deed.PTarget.p=0;witness.pos=1;log.search.guards.push_back(&witness);log.search.guards.push_back(&witness);
 if(!log.InformRelevantThingsOfDeed(7,0,static_cast<EOpinionPostDeedType>(18),0,false)||w.calls!=1)return 9;
 log.search.guards.clear();w.calls=0;log.deed.DeedType=static_cast<EOpinionDeedType>(7);log.search.unknown.push_back(&witness);
 NGlobalConsole::EnableUnknownThreatDeedPostings=true;recipient_eligible=true;
 if(!log.InformRelevantThingsOfDeed(7,&recipient,static_cast<EOpinionPostDeedType>(3),0,false)||!w.unknown||!r.unknown)return 10;
 NGlobalConsole::EnableUnknownThreatDeedPostings=false;r.calls=0;recipient.dead=true;
 if(log.InformRelevantThingsOfDeed(7,&recipient,static_cast<EOpinionPostDeedType>(1),&faction,true))return 11;
 recipient.dead=false;e.enemy=true;if(!log.InformRelevantThingsOfDeed(7,&recipient,static_cast<EOpinionPostDeedType>(1),&faction,true))return 12;
 puts("006c6040_TEST PASS");return 0;
}
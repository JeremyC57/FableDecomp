#include <list>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void ck(bool b){if(!b)exit(2);}
struct CThing {bool dead;CThing():dead(false){}};
struct CThingPlayerCreature:CThing{};
template<class T> struct CIntelligentPointer{T* p;CIntelligentPointer(T* v=0):p(v){}T* Get()const{return p;}};
template<class T>bool IS_THING_ALIVE(T* t){return t&&!t->dead;}
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>& t){return IS_THING_ALIVE(t.Get());}
struct CThingAICreature:CThing {CThing* link;};
struct CThingBuildingDef {bool IsPartOfHeroGuild;};
struct CSimBuildingDef {int Property;};
template<class T>struct CDefPointer{T* p;CDefPointer(T* v=0):p(v){}void SetDynamicCast(const CDefPointer<T>& b){p=b.p;}T* Get(){return p;}T* operator->(){return p;}};
struct CTCTrespassable{bool value;int writes;bool GetTrespassable(){return value;}void SetTrespassable(bool b){value=b;++writes;}};
struct CTCBuyableHouse{CThing* wife;bool owned,rented;CThing* GetWifeLivingHere(){return wife;}bool GetOwnedByPlayer(){return owned;}bool GetRented(){return rented;}};
enum{TCI_SHOP=83,TCI_TAVERN=171};
struct CThingBuilding:CThing{
 CThingBuildingDef* def;CSimBuildingDef* sim;CTCTrespassable* tres;CTCBuyableHouse* buy;
 bool tavern,shop,over,force,unlocked;CThing* blocker;int locks;bool locked;
 CDefPointer<const CThingBuildingDef> GetPBaseDef(){return def;}CDefPointer<const CSimBuildingDef> PeekPSimDefinition(){return sim;}
 bool GetTC(CTCBuyableHouse** p){if(buy)*p=buy;return buy!=0;}bool GetTC(CTCTrespassable** p){if(tres)*p=tres;return tres!=0;}
 bool HasTC(int t){return t==TCI_SHOP?shop:tavern;}bool GetForceOpen(){return force;}bool GetOverrideAutomaticLocking(){return over;}bool IsUnlocked(){return unlocked;}
 bool IsDoorBlockedBy(CThing* p){return p==blocker;}void LockAllDoors(bool b,bool immediate){ck(immediate);++locks;locked=b;}
};
struct HasBuildingAsHomeOrWork{const CThing* b;HasBuildingAsHomeOrWork(const CThing& v):b(&v){}bool operator()(const CIntelligentPointer<CThingAICreature>& t)const{return IS_THING_ALIVE(t)&&t.Get()->link==b;}};
struct Player{CThingPlayerCreature* p;CThingPlayerCreature* GetPControlledCreature(){return p;}};
struct PM{Player player;Player& GetMainPlayer(){return player;}};
struct CTCVillage{
 std::list<CIntelligentPointer<CThingBuilding> > VillageBuildings;
 std::list<CIntelligentPointer<CThingAICreature> > VillagePeople;
 int HouseCurrentlyChecking;bool inside,asleep,correct,curfew;PM pm;
 const std::list<CIntelligentPointer<CThingAICreature> >& PeekVillagePeople()const{return VillagePeople;}
 bool BuildingContainsPlayer(CThingBuilding*){return inside;}bool HomeUsersAreAsleep(CThingBuilding*){return asleep;}bool BuildingContainsCorrectVillagers(CThingBuilding*){return correct;}bool IsCurfewTime(){return curfew;}PM& GetPlayerManager(){return pm;}void UpdateCurfew();
};
void CTCVillage::UpdateCurfew()
{
    if (VillageBuildings.size()==0) return;
    ++HouseCurrentlyChecking;
    if (HouseCurrentlyChecking>=VillageBuildings.size()) HouseCurrentlyChecking=0;
    std::list<CIntelligentPointer<CThingBuilding> >::iterator i=VillageBuildings.begin();
    std::advance(i,HouseCurrentlyChecking);
    CThingBuilding* building=i->Get();
    CDefPointer<const CThingBuildingDef> pbuilding_def;
    pbuilding_def.SetDynamicCast(building->GetPBaseDef());
    if (pbuilding_def.Get() && pbuilding_def->IsPartOfHeroGuild) return;
    bool is_a_house=true;
    CDefPointer<const CSimBuildingDef> sim_building_def=building->PeekPSimDefinition();
    if (sim_building_def.Get() && !(sim_building_def->Property&1)) is_a_house=false;
    long num_people_linked_to_building=std::count_if(PeekVillagePeople().begin(),PeekVillagePeople().end(),HasBuildingAsHomeOrWork(*building));
    bool house_being_used=num_people_linked_to_building>0;
    bool player_considered_to_be_in_building=BuildingContainsPlayer(building);
    if (player_considered_to_be_in_building && HomeUsersAreAsleep(building)) player_considered_to_be_in_building=false;
    CTCBuyableHouse* buyable_house=0;
    building->GetTC(&buyable_house);
    bool desire_lock_doors;
    if (buyable_house && buyable_house->GetWifeLivingHere()) desire_lock_doors=false;
    else if (house_being_used) {
        if (building->HasTC(TCI_TAVERN)) desire_lock_doors=false;
        else if (building->HasTC(TCI_SHOP)) desire_lock_doors=true;
        else desire_lock_doors=IsCurfewTime() && is_a_house;
    } else desire_lock_doors=!(buyable_house && buyable_house->GetOwnedByPlayer() && !buyable_house->GetRented());
    if (building->GetOverrideAutomaticLocking()) {
        CTCTrespassable* tc_trespassable;
        if (!building->HasTC(TCI_SHOP) || !building->GetTC(&tc_trespassable) || !tc_trespassable->GetTrespassable()) return;
        desire_lock_doors=true;
    }
    bool able_to_lock_doors=BuildingContainsCorrectVillagers(building) && !player_considered_to_be_in_building;
    if (building->GetForceOpen()) able_to_lock_doors=false;
    CThingPlayerCreature* phero=GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (building->IsUnlocked() && desire_lock_doors && able_to_lock_doors) {
        if (IS_THING_ALIVE(phero) && building->IsDoorBlockedBy(phero)) able_to_lock_doors=false;
        for (std::list<CIntelligentPointer<CThingAICreature> >::const_iterator it=VillagePeople.begin(); it!=VillagePeople.end(); ++it) {
            if (IS_THING_ALIVE(*it) && building->IsDoorBlockedBy(it->Get())) { able_to_lock_doors=false;break; }
        }
    }
    building->LockAllDoors(desire_lock_doors && able_to_lock_doors,true);
    CTCTrespassable* ptc_trespassable;
    if (building->GetTC(&ptc_trespassable)) {
        if (!ptc_trespassable->GetTrespassable()) ptc_trespassable->SetTrespassable(desire_lock_doors && BuildingContainsCorrectVillagers(building) && house_being_used);
        else ptc_trespassable->SetTrespassable(desire_lock_doors && house_being_used);
    }
}
int main(){
 unsigned long seed=175823;bool covered[16]={false};
 for(int k=0;k<25000;++k){
  seed=seed*1664525UL+1013904223UL;unsigned long f=seed;
  CTCVillage v;CThingBuilding a,b;CTCBuyableHouse buy;CTCTrespassable tres;CThingPlayerCreature hero;CThingAICreature person;CThingBuildingDef def;CSimBuildingDef sim;
  bool buildings=k%23!=0;v.HouseCurrentlyChecking=k%2;
  if(buildings){v.VillageBuildings.push_back(&a);v.VillageBuildings.push_back(&b);}
  CThingBuilding* chosen=v.HouseCurrentlyChecking?&a:&b;
  def.IsPartOfHeroGuild=(f&3)==1;sim.Property=(f>>2)&1;chosen->def=f&8?&def:0;chosen->sim=f&16?&sim:0;
  chosen->buy=f&32?&buy:0;chosen->tres=f&64?&tres:0;tres.value=(f&128)!=0;bool initialTres=tres.value;tres.writes=0;
  chosen->shop=f&256;chosen->tavern=f&512;chosen->force=f&1024;chosen->over=f&2048;chosen->unlocked=f&4096;
  buy.wife=f&8192?&hero:0;buy.owned=f&16384;buy.rented=f&32768;v.pm.player.p=f&65536?&hero:0;
  hero.dead=f&131072;person.dead=f&262144;person.link=f&524288?chosen:0;v.VillagePeople.push_back(0);v.VillagePeople.push_back(&person);
  v.inside=f&1048576;v.asleep=f&2097152;v.correct=f&4194304;v.curfew=f&8388608;
  chosen->blocker=(f&16777216)?(CThing*)&person:(f&33554432)?(CThing*)&hero:0;
  chosen->locks=0;chosen->locked=false;int old=v.HouseCurrentlyChecking;v.UpdateCurfew();
  ck(v.HouseCurrentlyChecking==(buildings?1-old:old));
  bool skip=!buildings||(chosen->def&&def.IsPartOfHeroGuild)||(chosen->over&&(!chosen->shop||!chosen->tres||!initialTres));
  ck(chosen->locks==!skip);
  if(skip){ck(tres.writes==0);covered[0]=true;continue;}
  bool linked=!person.dead&&person.link==chosen,isHouse=!chosen->sim||(sim.Property&1);
  bool desire=chosen->over?true:chosen->buy&&buy.wife?false:linked?(!chosen->tavern&&(chosen->shop||(v.curfew&&isHouse))):!(chosen->buy&&buy.owned&&!buy.rented);
  bool able=v.correct&&(!v.inside||v.asleep)&&!chosen->force;
  bool blocked=chosen->unlocked&&((chosen->blocker==&person&&!person.dead)||(chosen->blocker==&hero&&v.pm.player.p&&!hero.dead));
  ck(chosen->locked==(desire&&able&&!blocked));ck(tres.writes==(chosen->tres?1:0));
  if(chosen->tres)ck(tres.value==(desire&&linked&&(initialTres||v.correct)));
  covered[1+desire+2*able+4*blocked]=true;
 }
 for(int i=0;i<=8;++i)ck(covered[i]);puts("0065ad37_TEST PASS");return 0;
}
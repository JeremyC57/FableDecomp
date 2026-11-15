// Full Init body copied verbatim; component lookups and ring placement are observable portable fixtures.
#include <cstdio>
#include <vector>
#include <map>
#include <list>
enum EUnsheatheWeaponType {UNSHEATHE_NULL=0,UNSHEATHE_MELEE_WEAPON=4,UNSHEATHE_RANGED_WEAPON=5};
struct CCombatTypeDef {std::vector<long> DefenderCombatRingOccupationNumbers;long DefenderWheelOuterRingStartIndex;bool PreferOuterRing;};
struct CTCCombat {EUnsheatheWeaponType type;EUnsheatheWeaponType GetUnsheathingWeaponType()const{return type;}};
struct CTCWeapon {bool melee;bool IsMeleeWeapon()const{return melee;}};
class CThing;
struct CTCCarrying {CThing* weapon;CThing* GetThingInPrimarySlot()const{return weapon;}};
class CThing {public:bool dead;CTCWeapon* weaponTC;CThing():dead(false),weaponTC(0){}bool IsDead()const{return dead;}bool PeekTC(const CTCWeapon** out)const{*out=weaponTC;return weaponTC!=0;}};
class CThingCreatureBase:public CThing {public:int id;CCombatTypeDef def;CTCCombat* combat;CTCCarrying* carrying;CThingCreatureBase():id(-1),combat(0),carrying(0){def.PreferOuterRing=false;}
 const CCombatTypeDef& GetCombatTypeDef()const{return def;}bool PeekTC(const CTCCombat** out)const{*out=combat;return combat!=0;}bool PeekTC(const CTCCarrying** out)const{*out=carrying;return carrying!=0;}
};
struct CAttackingCombatantInfo {long CombatWheelRingIndex;};
struct CWorldMap{};struct CCombatManager{};
static int reset_calls,add_calls,assign_calls;
class CDefendingCombatantInfo {public:class CCombatWheel;
 class CCombatRing {public:long Index,Capacity;std::vector<int> assigned;CCombatRing(long index=0,long cap=1):Index(index),Capacity(cap){}void Reset(){++reset_calls;assigned.erase(assigned.begin(),assigned.end());}
  void AssignAttackersToRing(const CWorldMap&,const CCombatWheel&,std::map<CThingCreatureBase*,CAttackingCombatantInfo*>& attackers,const CThingCreatureBase&){++assign_calls;long count=0;while(!attackers.empty()&&count<Capacity){std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator i=attackers.begin();assigned.push_back(i->first->id);i->second->CombatWheelRingIndex=Index;attackers.erase(i);++count;}}
 };
 class CCombatWheel {public:std::vector<CCombatRing> CombatRings;
  CCombatRing* GetRing(long index){if(index>=0&&(unsigned long)index<CombatRings.size())return &CombatRings[index];return 0;}
  CCombatRing& AddRing(const CCombatTypeDef& def){++add_calls;long index=(long)CombatRings.size();CombatRings.push_back(CCombatRing(index,def.DefenderCombatRingOccupationNumbers[index]));return CombatRings.back();}
  void Init(CCombatManager&,CWorldMap&,const std::map<CThingCreatureBase*,CAttackingCombatantInfo*>&,const CThingCreatureBase&);
 };
};
void CDefendingCombatantInfo::CCombatWheel::Init(CCombatManager&, CWorldMap& world,
                                                 const std::map<CThingCreatureBase*,CAttackingCombatantInfo*>& attackers,
                                                 const CThingCreatureBase& defender)
{
    std::map<CThingCreatureBase*,CAttackingCombatantInfo*> inner_attackers;
    std::map<CThingCreatureBase*,CAttackingCombatantInfo*> outer_attackers;
    std::list<CThingCreatureBase*> outer_attackers_list;
    const CCombatTypeDef& combat_type = defender.GetCombatTypeDef();
    for (std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::const_iterator it = attackers.begin(); it != attackers.end(); ++it) {
        CThingCreatureBase* attacker = it->first;
        bool melee = false;
        const CTCCombat* combat;
        if (attacker->PeekTC(&combat) && combat->GetUnsheathingWeaponType() != UNSHEATHE_NULL) {
            if (combat->GetUnsheathingWeaponType() == UNSHEATHE_MELEE_WEAPON)
                melee = true;
        } else {
            const CTCCarrying* carrying;
            if (attacker->PeekTC(&carrying)) {
                CThing* weapon = carrying->GetThingInPrimarySlot();
                const CTCWeapon* weapon_tc;
                if (weapon && !weapon->IsDead() && weapon->PeekTC(&weapon_tc) && weapon_tc->IsMeleeWeapon())
                    melee = true;
            }
        }
        if (attacker->GetCombatTypeDef().PreferOuterRing) {
            outer_attackers.insert(*it);
            if (melee)
                outer_attackers_list.push_front(attacker);
            else
                outer_attackers_list.push_back(attacker);
        } else {
            inner_attackers.insert(*it);
        }
    }
    long num_to_move = 0;
    if (inner_attackers.size() == 0 && outer_attackers.size() != 0)
        num_to_move = 1;
    if (combat_type.DefenderWheelOuterRingStartIndex == 0)
        num_to_move = outer_attackers.size();
    for (long i = num_to_move; i > 0; --i) {
        std::list<CThingCreatureBase*>::iterator first = outer_attackers_list.begin();
        std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator found = outer_attackers.find(*first);
        inner_attackers.insert(*found);
        outer_attackers.erase(found);
        outer_attackers_list.erase(first);
    }
    for (std::vector<CCombatRing>::iterator ring = CombatRings.begin(); ring != CombatRings.end(); ++ring)
        ring->Reset();
    long num_rings = combat_type.DefenderCombatRingOccupationNumbers.size();
    long ring_index = 0;
    while (inner_attackers.size() != 0 && ring_index < num_rings) {
        CCombatRing* ring = GetRing(ring_index++);
        if (!ring)
            ring = &AddRing(combat_type);
        ring->AssignAttackersToRing(world, *this, inner_attackers, defender);
    }
    if (combat_type.DefenderWheelOuterRingStartIndex > ring_index)
        ring_index = combat_type.DefenderWheelOuterRingStartIndex;
    while (outer_attackers.size() != 0 && ring_index < num_rings) {
        CCombatRing* ring = GetRing(ring_index++);
        if (!ring)
            ring = &AddRing(combat_type);
        ring->AssignAttackersToRing(world, *this, outer_attackers, defender);
    }
    long total = CombatRings.size();
    if (ring_index < total) {
        long extra = total - ring_index;
        do {
            if (!CombatRings.empty())
                CombatRings.erase(CombatRings.end() - 1);
        } while (--extra);
    }
    if (inner_attackers.size() != 0 || outer_attackers.size() != 0) {
        for (std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator it = inner_attackers.begin(); it != inner_attackers.end(); ++it)
            it->second->CombatWheelRingIndex = -1;
        for (std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator it = outer_attackers.begin(); it != outer_attackers.end(); ++it)
            it->second->CombatWheelRingIndex = -1;
    }
}

static int fail(int line){std::printf("006ec850_TEST FAIL %d\n",line);return 1;}
#define CHECK(x) do{if(!(x))return fail(__LINE__);}while(0)
static void Setup(CThingCreatureBase* a,CAttackingCombatantInfo* info,int n,std::map<CThingCreatureBase*,CAttackingCombatantInfo*>& map){for(int i=0;i<n;++i){a[i].id=i;info[i].CombatWheelRingIndex=99;map[&a[i]]=&info[i];}}
static void ClearCounters(){reset_calls=add_calls=assign_calls=0;}
int main(){CCombatManager manager;CWorldMap world;
 // Empty input resets and removes all old rings.
 {CThingCreatureBase defender;defender.def.DefenderWheelOuterRingStartIndex=0;defender.def.DefenderCombatRingOccupationNumbers.resize(3,1);std::map<CThingCreatureBase*,CAttackingCombatantInfo*> map;CDefendingCombatantInfo::CCombatWheel wheel;for(int i=0;i<3;++i)wheel.CombatRings.push_back(CDefendingCombatantInfo::CCombatRing(i));ClearCounters();wheel.Init(manager,world,map,defender);CHECK(wheel.CombatRings.empty()&&reset_calls==3&&assign_calls==0&&add_calls==0);}
 // All outer: move the last melee-prioritized creature inward. Exercise both direct combat mode and carrying fallback.
 for(int scenario=0;scenario<6;++scenario){int dead_weapon=scenario==1;CThingCreatureBase a[3],defender;CAttackingCombatantInfo info[3];std::map<CThingCreatureBase*,CAttackingCombatantInfo*> map;Setup(a,info,3,map);for(int i=0;i<3;++i)a[i].def.PreferOuterRing=true;
  CTCCombat mode;mode.type=UNSHEATHE_MELEE_WEAPON;a[1].combat=&mode;CTCWeapon weapon_tc;weapon_tc.melee=true;CThing weapon;weapon.weaponTC=&weapon_tc;weapon.dead=dead_weapon!=0;if(scenario==3)weapon_tc.melee=false;if(scenario==5)weapon.weaponTC=0;CTCCarrying carrying;carrying.weapon=scenario==4?0:&weapon;a[2].carrying=&carrying;CTCCombat nullmode;nullmode.type=scenario==2?UNSHEATHE_RANGED_WEAPON:UNSHEATHE_NULL;a[2].combat=&nullmode;
  defender.def.DefenderWheelOuterRingStartIndex=2;defender.def.DefenderCombatRingOccupationNumbers.resize(4,1);CDefendingCombatantInfo::CCombatWheel wheel;wheel.CombatRings.push_back(CDefendingCombatantInfo::CCombatRing(0));wheel.CombatRings.push_back(CDefendingCombatantInfo::CCombatRing(1));ClearCounters();wheel.Init(manager,world,map,defender);
  int inward=scenario?1:2;CHECK(wheel.CombatRings.size()==4&&reset_calls==2&&add_calls==2&&assign_calls==3);CHECK(wheel.CombatRings[0].assigned.size()==1&&wheel.CombatRings[0].assigned[0]==inward&&wheel.CombatRings[1].assigned.empty());CHECK(wheel.CombatRings[2].assigned[0]==0&&wheel.CombatRings[3].assigned[0]==(scenario?2:1));CHECK(info[inward].CombatWheelRingIndex==0&&info[0].CombatWheelRingIndex==2&&map.size()==3);
 }
 // Mixed inner and outer attackers: full rings leave excess attackers unplaced and trim unused existing rings.
 {CThingCreatureBase a[4],defender;CAttackingCombatantInfo info[4];std::map<CThingCreatureBase*,CAttackingCombatantInfo*> map;Setup(a,info,4,map);a[2].def.PreferOuterRing=a[3].def.PreferOuterRing=true;defender.def.DefenderWheelOuterRingStartIndex=2;defender.def.DefenderCombatRingOccupationNumbers.resize(3,1);CDefendingCombatantInfo::CCombatWheel wheel;for(int i=0;i<5;++i)wheel.CombatRings.push_back(CDefendingCombatantInfo::CCombatRing(i));ClearCounters();wheel.Init(manager,world,map,defender);CHECK(wheel.CombatRings.size()==3&&reset_calls==5&&add_calls==0&&assign_calls==3);CHECK(info[0].CombatWheelRingIndex==0&&info[1].CombatWheelRingIndex==1&&info[2].CombatWheelRingIndex==2&&info[3].CombatWheelRingIndex==-1);}
 // No inner ring: every outer attacker becomes an inner candidate, ordered by the typed map.
 {CThingCreatureBase a[3],defender;CAttackingCombatantInfo info[3];std::map<CThingCreatureBase*,CAttackingCombatantInfo*> map;Setup(a,info,3,map);for(int i=0;i<3;++i)a[i].def.PreferOuterRing=true;defender.def.DefenderWheelOuterRingStartIndex=0;defender.def.DefenderCombatRingOccupationNumbers.resize(3,1);CDefendingCombatantInfo::CCombatWheel wheel;ClearCounters();wheel.Init(manager,world,map,defender);CHECK(wheel.CombatRings.size()==3&&add_calls==3&&assign_calls==3);for(int i=0;i<3;++i)CHECK(info[i].CombatWheelRingIndex==i);}
 // No ring capacity: both nonempty sets are marked unplaced, with no ring assignment call.
 {CThingCreatureBase a[2],defender;CAttackingCombatantInfo info[2];std::map<CThingCreatureBase*,CAttackingCombatantInfo*> map;Setup(a,info,2,map);a[1].def.PreferOuterRing=true;defender.def.DefenderWheelOuterRingStartIndex=0;CDefendingCombatantInfo::CCombatWheel wheel;ClearCounters();wheel.Init(manager,world,map,defender);CHECK(wheel.CombatRings.empty()&&assign_calls==0&&info[0].CombatWheelRingIndex==-1&&info[1].CombatWheelRingIndex==-1);}
 std::printf("006ec850_TEST PASS\n");return 0;}
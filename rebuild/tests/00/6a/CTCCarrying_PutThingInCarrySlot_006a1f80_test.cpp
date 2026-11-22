#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
// Leaf: ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z
// Flags: (none)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_SLOTFREE
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CTCCARRYABLE_SETASCARRIEDBY
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_ISPROJECTILE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCCreatureModeManager.h"
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "engine/CDefPointer.h"
#include "engine/CCarrySlotDef.h"
#include "engine/CGameDefinitionManager.h"

#include <cstdio>
static int errors=0,reset_calls=0,carry_calls=0,carry_queries=0,weapon_queries=0,mode_queries=0,remove_calls=0;static void*carrier=0;
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d %s\n",__LINE__,#x);}}while(0)
struct FixturePhysics{void ResetInterpolation(){++reset_calls;}};
struct FixtureCarryable{void SetAsCarriedBy(void*p){++carry_calls;carrier=p;}};
struct FixtureWeapon{bool projectile;bool IsProjectile()const{return projectile;}};
struct FixtureMode{NCreatureMode::EMode mode;NCreatureMode::EMode GetType()const{return mode;}};
struct FixtureModes{FixtureMode current;unsigned long added[3],removed[2];int adds,removes;FixtureModes():adds(0),removes(0){}FixtureMode&GetCurrentMode()const{return const_cast<FixtureMode&>(current);}bool AddMode(unsigned long m){CHECK(adds<3);added[adds++]=m;return true;}void RemoveMode(unsigned long m){CHECK(removes<2);removed[removes++]=m;}};
struct FixtureThing{FixturePhysics phys;FixtureCarryable carry;FixtureWeapon weapon;FixtureModes modes;bool hasweapon,hasmodes,limbo;FixtureThing():hasweapon(true),hasmodes(true),limbo(true){}bool GetTC(FixtureCarryable**p){++carry_queries;*p=&carry;return true;}bool GetTC(FixtureModes**p){++mode_queries;*p=hasmodes?&modes:0;return hasmodes;}bool PeekTC(const FixtureWeapon**p)const{++weapon_queries;*p=hasweapon?&weapon:0;return hasweapon;}FixturePhysics*GetPhysicsTC(){return &phys;}void SetInLimbo(bool b){limbo=b;}};
template<class T>struct FixtureIP{T*p;FixtureIP(T*q=0):p(q){}FixtureIP&operator=(const FixtureIP&q){p=q.p;return*this;}bool operator==(const FixtureIP&q)const{return p==q.p;}T*Get()const{return p;}};
template<class K,class V>struct FixtureMap{typedef std::pair<K,V>value_type;typedef value_type*iterator;value_type entries[2];int count;FixtureMap():count(0){}iterator end(){return entries+count;}iterator Find(const K&k){for(int i=0;i<count;++i)if(entries[i].first==k)return entries+i;return end();}};
struct FixtureList{FixtureIP<FixtureThing>entries[5];int count;FixtureList():count(0){}void remove(const FixtureIP<FixtureThing>&v){for(int i=0;i<count;)if(entries[i]==v){for(int j=i;j+1<count;++j)entries[j]=entries[j+1];--count;}else++i;}void push_back(const FixtureIP<FixtureThing>&v){CHECK(count<5);entries[count++]=v;}};
struct FixtureCarrying{FixtureMap<long,FixtureIP<FixtureThing> >CarrySlots;FixtureList ThingsInPrimarySlots;FixtureThing parent;bool primary;bool IsCarrySlotFree(long s){return CarrySlots.Find(s)!=CarrySlots.end()&&CarrySlots.Find(s)->second.p==0;}FixtureThing*RemoveThingInCarrySlot(long,bool b){++remove_calls;CHECK(!b);return 0;}bool IsSlotPrimarySlot(long s)const{CHECK(s==9);return primary;}FixtureThing&GetParentThing(){return parent;}void PutThingInCarrySlot(FixtureThing&,long,bool);};
#define CTCCarrying FixtureCarrying
#define CThing FixtureThing
#define CVectorMap FixtureMap
#define CIntelligentPointer FixtureIP
#define CTCCarryable FixtureCarryable
#define CTCWeapon FixtureWeapon
#define CTCCreatureModeManager FixtureModes
void CTCCarrying::PutThingInCarrySlot(CThing& thing, long slot, bool set_up_weapon_mode)
{
    if (!IsCarrySlotFree(slot))
        RemoveThingInCarrySlot(slot, false);

    CVectorMap<long, CIntelligentPointer<CThing> >::iterator slot_it = CarrySlots.Find(slot);
    if (slot_it != CarrySlots.end()) {
        slot_it->second = CIntelligentPointer<CThing>(&thing);
        CTCCarryable* ptc_carryable;
        thing.GetTC(&ptc_carryable);
        ptc_carryable->SetAsCarriedBy(this);
        thing.GetPhysicsTC()->ResetInterpolation();
    }

    ThingsInPrimarySlots.remove(CIntelligentPointer<CThing>(0));
    thing.SetInLimbo(false);
    if (IsSlotPrimarySlot(slot)) {
        ThingsInPrimarySlots.push_back(CIntelligentPointer<CThing>(&thing));
        const CTCWeapon* ptc_weapon;
        if (set_up_weapon_mode && thing.PeekTC(&ptc_weapon) && !ptc_weapon->IsProjectile()) {
            CTCCreatureModeManager* ptc_modes;
            if (GetParentThing().GetTC(&ptc_modes)) {
                NCreatureMode::EMode old_mode_type = ptc_modes->GetCurrentMode().GetType();
                if (old_mode_type != NCreatureMode::MODE_CARRYING_WEAPON_STAND)
                    ptc_modes->AddMode(NCreatureMode::MODE_CARRYING_WEAPON_STAND);
                if (old_mode_type == NCreatureMode::MODE_STRAFE) {
                    ptc_modes->RemoveMode(old_mode_type);
                    ptc_modes->AddMode(old_mode_type);
                    return;
                }
                if (old_mode_type == NCreatureMode::MODE_BERSERK) {
                    ptc_modes->RemoveMode(old_mode_type);
                    ptc_modes->AddMode(old_mode_type);
                    return;
                }
                if (old_mode_type == NCreatureMode::MODE_BLOCK) {
                    ptc_modes->RemoveMode(old_mode_type);
                    ptc_modes->AddMode(old_mode_type);
                }
            }
        }
    }
}

#undef CTCCarrying
#undef CThing
#undef CVectorMap
#undef CIntelligentPointer
#undef CTCCarryable
#undef CTCWeapon
#undef CTCCreatureModeManager
int main(){long modes[]={4,25,36,12,1};for(int mode=0;mode<25;++mode){FixtureCarrying owner;FixtureThing thing,old;bool present=mode!=0,occupied=mode==1,setmode=mode>=6;owner.primary=mode>=4;owner.parent.hasmodes=mode!=7;thing.hasweapon=mode!=8;thing.weapon.projectile=mode==9;long oldmode=modes[mode%5];owner.parent.modes.current.mode=(NCreatureMode::EMode)oldmode;if(present){owner.CarrySlots.count=1;owner.CarrySlots.entries[0].first=9;owner.CarrySlots.entries[0].second.p=occupied?&old:0;}owner.ThingsInPrimarySlots.entries[0].p=0;owner.ThingsInPrimarySlots.entries[1].p=&old;owner.ThingsInPrimarySlots.entries[2].p=0;owner.ThingsInPrimarySlots.count=3;reset_calls=carry_calls=carry_queries=weapon_queries=mode_queries=remove_calls=0;carrier=0;owner.PutThingInCarrySlot(thing,9,setmode);CHECK(remove_calls==(!present||occupied?1:0));CHECK(carry_calls==(present?1:0)&&carry_queries==carry_calls&&reset_calls==carry_calls&&(!present||carrier==&owner));CHECK(!thing.limbo&&(!present||owner.CarrySlots.entries[0].second.p==&thing));CHECK(owner.ThingsInPrimarySlots.count==(owner.primary?2:1)&&owner.ThingsInPrimarySlots.entries[0].p==&old);if(owner.primary)CHECK(owner.ThingsInPrimarySlots.entries[1].p==&thing);bool armed=owner.primary&&setmode&&thing.hasweapon&&!thing.weapon.projectile&&owner.parent.hasmodes;bool cycle=oldmode==25||oldmode==36||oldmode==12;CHECK(weapon_queries==((owner.primary&&setmode)?1:0));CHECK(mode_queries==((owner.primary&&setmode&&thing.hasweapon&&!thing.weapon.projectile)?1:0));CHECK(owner.parent.modes.adds==(armed?((oldmode!=4?1:0)+(cycle?1:0)):0));CHECK(owner.parent.modes.removes==((armed&&cycle)?1:0));if(armed&&oldmode!=4)CHECK(owner.parent.modes.added[0]==4);if(armed&&cycle)CHECK(owner.parent.modes.removed[0]==(unsigned long)oldmode&&owner.parent.modes.added[owner.parent.modes.adds-1]==(unsigned long)oldmode);}std::puts(errors?"006a1f80_TEST FAIL":"006a1f80_TEST PASS");return errors?1:0;}
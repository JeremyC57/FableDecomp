#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCREATUREACTION_SPIRITHITRESPONSE_PASS26
// Leaf: ?OnHit@CThingPlayerCreature@@UAEXAAVCHitParameters@@@Z
// Flags: (none)
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_ONHIT_PASS26
#define FABLE_RETAIL_CPLAYERGUI_ONHEROHIT_PASS26
#define FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26
#define FABLE_RETAIL_CJOYSTICK_INITRUMBLE_BODY_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETATTACKEDBYCREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CJOYSTICK
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#define FABLE_RETAIL_CINPUTMANAGER_JOYSTICKS
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_NENTITYEVENTS_BASE_ORIGINAL_PASS26
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#include "engine/CThingPlayerCreature.h"
#include "engine/CInputManager.h"
#include "engine/CHitParameters.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitThing.h"
#include "engine/CCreatureAction_SpiritHitResponse.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayerGui.h"
extern CInputManager* PInputManager;

#include <cstdio>
#include <cmath>
static int errors=0,basecalls=0,rumbles=0,enemies=0,melees=0,ranged=0,events=0,eventlive=0,attacked=0,removed=0,spirits=0,spiritlive=0,actions=0,guihits=0;static bool enemy=false,melee=false,range=false,failalloc=false;
#define CHECK(x) do{if(!(x)){++errors;std::printf("FAIL %d %s\n",__LINE__,#x);}}while(0)
struct FixtureThing;struct FixtureCreature;struct FixtureEvents;struct FixturePlayer;struct FixtureHit;struct FixtureCarry;struct FixtureModes;struct FixtureEvent;
static FixtureThing* expected_hitter=0;static const FixtureThing*expected_weapon=0;static bool flourish=false;static float damage=0;
struct FixtureEvents{void AddEvent(FixtureEvent*);};
struct FixtureThing{bool alive;unsigned long flags;FixtureEvents*events;FixtureThing():alive(true),flags(8),events(0){}unsigned long GetTypeFlags()const{return flags;}bool GetTC(FixtureEvents**p){*p=events;return events!=0;}};
struct FixtureCreature:FixtureThing{void OnHit(FixtureHit&){CHECK(basecalls==0&&rumbles==0);++basecalls;}};
struct FixtureHit{FixtureThing*hitter;const FixtureThing*weapon;bool fl;float dmg;C3DVector direction;FixtureThing*GetPThingHitting(){CHECK(rumbles==1);return hitter;}const FixtureThing*GetPWeapon(){return weapon;}bool IsFlourish(){return fl;}const C3DVector&GetHitDirection(){return direction;}float GetDamageToThing(const FixturePlayer&){CHECK(spiritlive==0);return dmg;}};
struct FixtureEnemy{static bool AreThingsEnemies(const FixtureThing&,const FixtureThing&h){CHECK(&h==expected_hitter);++enemies;return enemy;}};
struct FixtureCarry{bool free;static bool IsThingCarryingMeleeWeapon(const FixtureThing&h){CHECK(&h==expected_hitter);++melees;return melee;}static bool IsThingCarryingRangedWeapon(const FixtureThing&h){CHECK(&h==expected_hitter);++ranged;return range;}bool IsPrimarySlotFree()const{return free;}FixtureThing*RemoveThingFromPrimarySlot(bool b){CHECK(!b&&!free);++removed;return 0;}};
struct FixtureMode{NCreatureMode::EMode type;NCreatureMode::EMode GetType()const{return type;}};struct FixtureModes{FixtureMode mode;const FixtureMode&GetCurrentMode()const{return mode;}};
struct FixtureSpirit{FixtureSpirit(FixtureCreature&,const C3DVector&v){CHECK(v.X==2&&v.Y==-3&&v.Z==4);++spirits;++spiritlive;}~FixtureSpirit(){--spiritlive;}};
struct FixturePlayer:FixtureCreature{FixtureModes*modes;FixtureCarry*carry;bool coop;FixtureCreature*last;int GetJoystickDeviceNumber()const{CHECK(basecalls==1);return 3;}bool PeekTC(const FixtureModes**p)const{*p=modes;return modes!=0;}bool GetTC(FixtureCarry**p){*p=carry;return carry!=0;}bool HasTC(ETCInterfaceType t)const{CHECK(t==TCI_COOP_SPIRIT);return coop;}void SetAttackedByCreature(FixtureCreature*p){CHECK(p==expected_hitter);last=p;++attacked;}bool SetCurrentAction(const FixtureSpirit&){CHECK(spiritlive==1);++actions;return true;}void OnHit(FixtureHit&);};
struct FixtureJoystick{void AddRumble(long id,const CJoystick::CInitRumble&r,bool replace){CHECK(basecalls==1&&id==2&&!replace&&r.MaxLevel==1&&r.Smoothness==.5f&&r.AttackTime==0&&r.DecayTime==.4f&&!r.Cumulative);++rumbles;}};struct FixtureInput{FixtureJoystick joy;FixtureJoystick&GetJoystick(long n){CHECK(n==3&&basecalls==1);return joy;}};static FixtureInput input;static FixtureInput*FixtureInputManager=&input;
struct FixtureGui{static FixtureGui&Get(){static FixtureGui g;return g;}void OnHeroHit(float d){CHECK(d==damage&&spiritlive==0&&eventlive==0);++guihits;}};
struct FixtureEvent{bool hit,usedmelee,usedranged,fl,decap;FixtureEvent(const FixtureThing&h,FixtureThing&,bool a,bool b,bool c,const FixtureThing*w,bool f,bool dc):hit(a),usedmelee(b),usedranged(c),fl(f),decap(dc){CHECK(&h==expected_hitter&&w==expected_weapon&&a==enemy&&b==melee&&c==range&&f==flourish&&!dc);++eventlive;}~FixtureEvent(){--eventlive;}static void*operator new(unsigned n){return failalloc?0: ::operator new(n);}static void operator delete(void*p){::operator delete(p);}};
void FixtureEvents::AddEvent(FixtureEvent*e){CHECK((e==0)==failalloc);++events;delete e;}
namespace NEntityEvents{typedef ::FixtureEvent FixtureEvent;}
static bool FixtureAlive(FixtureThing*p){return p&&p->alive;}
#define CThingPlayerCreature FixturePlayer
#define CThingCreatureBase FixtureCreature
#define CThing FixtureThing
#define CHitParameters FixtureHit
#define CTCEnemy FixtureEnemy
#define CTCCarrying FixtureCarry
#define CTCEntityEvents FixtureEvents
#define CTCCreatureModeManager FixtureModes
#define CCreatureAction_SpiritHitResponse FixtureSpirit
#define CPlayerGui FixtureGui
#define CEventHitThing FixtureEvent
#define PInputManager FixtureInputManager
#define IS_THING_ALIVE FixtureAlive
void CThingPlayerCreature::OnHit(CHitParameters& hit_params)
{
 CThingCreatureBase::OnHit(hit_params);
 PInputManager->GetJoystick(GetJoystickDeviceNumber()).AddRumble(2,CJoystick::CInitRumble(1.0f,0.5f,0.0f,0.4f,false),false);
 CThing* phitting_thing=hit_params.GetPThingHitting();
 if(IS_THING_ALIVE(phitting_thing)&&(phitting_thing->GetTypeFlags()&8)) {
  CTCEntityEvents* ptc_entity_events;
  if(phitting_thing->GetTC(&ptc_entity_events)) {
   bool decapitate=false;
   bool hit_enemy=CTCEnemy::AreThingsEnemies(*this,*phitting_thing);
   bool used_melee_weapon=CTCCarrying::IsThingCarryingMeleeWeapon(*phitting_thing);
   bool used_ranged_weapon=CTCCarrying::IsThingCarryingRangedWeapon(*phitting_thing);
   ptc_entity_events->AddEvent(new NEntityEvents::CEventHitThing(*phitting_thing,*this,hit_enemy,used_melee_weapon,used_ranged_weapon,hit_params.GetPWeapon(),hit_params.IsFlourish(),decapitate));
  }
  SetAttackedByCreature(static_cast<CThingCreatureBase*>(phitting_thing));
 }
 const CTCCreatureModeManager* ptc_mode_manager;
 if(PeekTC(&ptc_mode_manager)&&ptc_mode_manager->GetCurrentMode().GetType()==NCreatureMode::MODE_CARRYING_GENERIC_BOX) {
  CTCCarrying* ptc_carrying;
  GetTC(&ptc_carrying);
  if(!ptc_carrying->IsPrimarySlotFree())ptc_carrying->RemoveThingFromPrimarySlot(false);
 }
 if(HasTC(TCI_COOP_SPIRIT))SetCurrentAction(CCreatureAction_SpiritHitResponse(*this,hit_params.GetHitDirection()));
 CPlayerGui::Get().OnHeroHit(hit_params.GetDamageToThing(*this));
}

#undef CThingPlayerCreature
#undef CThingCreatureBase
#undef CThing
#undef CHitParameters
#undef CTCEnemy
#undef CTCCarrying
#undef CTCEntityEvents
#undef CTCCreatureModeManager
#undef CCreatureAction_SpiritHitResponse
#undef CPlayerGui
#undef CEventHitThing
#undef PInputManager
#undef IS_THING_ALIVE
int main(){for(int t=0;t<96;++t){FixturePlayer player;FixtureCreature hitter;FixtureThing weapon;FixtureEvents ev;FixtureCarry carry;FixtureModes modes;FixtureHit hit;bool valid=(t%4)==3;hitter.alive=t%4!=1;hitter.flags=t%4==2?0:8;hitter.events=(t&4)?&ev:0;hit.hitter=t%4==0?0:&hitter;hit.weapon=(t&8)?&weapon:0;hit.fl=(t&16)!=0;hit.dmg=(t%3-1)*4.f;hit.direction=C3DVector(2,-3,4);modes.mode.type=(t&8)?NCreatureMode::MODE_CARRYING_GENERIC_BOX:NCreatureMode::MODE_STAND;player.modes=(t&16)?&modes:0;carry.free=(t&32)!=0;player.carry=&carry;player.coop=(t&64)!=0;player.last=0;expected_hitter=hit.hitter;expected_weapon=hit.weapon;flourish=hit.fl;damage=hit.dmg;enemy=(t&8)!=0;melee=(t&16)!=0;range=!melee;failalloc=(t%5)==0;basecalls=rumbles=enemies=melees=ranged=events=eventlive=attacked=removed=spirits=spiritlive=actions=guihits=0;player.OnHit(hit);bool event=valid&&hitter.events;CHECK(basecalls==1&&rumbles==1&&guihits==1&&attacked==(valid?1:0)&&player.last==(valid?&hitter:0));CHECK(events==(event?1:0)&&enemies==events&&melees==events&&ranged==events&&eventlive==0);CHECK(removed==((player.modes&&modes.mode.type==NCreatureMode::MODE_CARRYING_GENERIC_BOX&&!carry.free)?1:0));CHECK(spirits==(player.coop?1:0)&&actions==spirits&&spiritlive==0);}std::puts(errors?"006ab0e0_TEST FAIL":"006ab0e0_TEST PASS");return errors?1:0;}
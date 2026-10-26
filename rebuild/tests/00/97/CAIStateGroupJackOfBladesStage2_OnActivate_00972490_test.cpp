// CAIStateGroup_JackOfBladesStage2::OnActivate() -- register the target and switch the three hit-location enabled states.
// Retail00972490 length254. Shared headers retain opaque group fields24(target) and30(state).
#ifdef _MSC_VER
#include "engine/CAIStateGroup_JackOfBladesStage2.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "fable_string.h"
#else
#define __forceinline inline
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_JackOfBladesStage2:CAIStateGroupBase{char before[16];void* _pad_0x24[2];char unused[4];int _pad_0x30[1];};
class CCharString{public:CCharString(const char*,long);~CCharString();const char* storage_;};
class CCombatManager;
class CWorld{ public:char prefix[0x4c];CCombatManager* PThingCombatManager_ptr;};
#endif
struct JackThing;
class CCombatManager{ public:void RegisterCombatant(JackThing*,JackThing*,bool);}; //006edc50
struct JackPointer{JackThing* GetPItem()const;}; //00a01b50
struct HitLocation{void SetEnabled(bool);}; //00816390
struct HitLocations{HitLocation* GetHitLocation(const CCharString&);}; //00815820
struct JackEntry{long type;HitLocations* value;};
struct JackTable{JackEntry* begin;JackEntry* end;JackEntry* FindEntry(const long&); __forceinline HitLocations* Lookup(){long key=127;JackEntry* entry=FindEntry(key);if(entry==end)entry=end;else if(entry->type>127)entry=end;return entry->value;}}; //0040f020
struct JackThing{char prefix[0x2c];unsigned long mask;char gap[0x14];JackTable components;
__forceinline void GetInterface(HitLocations** output){if(mask&0x80000000)*output=components.Lookup();}
};
struct JackMethods:CAIStateGroup_JackOfBladesStage2{
CWorld* GetPWorld(); //008fcfc0
__forceinline JackThing* Creature(){return (JackThing*)((CAIStateGroupBase*)this)->PBrain->Creature;}
void OnActivate();
};
void JackMethods::OnActivate(){
JackThing* actor=Creature();CCombatManager* combat=GetPWorld()->PThingCombatManager_ptr;
combat->RegisterCombatant(actor,((JackPointer*)_pad_0x24)->GetPItem(),false);
HitLocations* hits;
Creature()->GetInterface(&hits);
HitLocation* location;
{CCharString name("HIT_LOCATION_JACK_OF_BLADES",-1);location=hits->GetHitLocation(name);}location->SetEnabled(false);
{CCharString name("HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD",-1);location=hits->GetHitLocation(name);}location->SetEnabled(false);
{CCharString name("HIT_LOCATION_JACK_OF_BLADES_BIG",-1);location=hits->GetHitLocation(name);}location->SetEnabled(true);
*(int*)_pad_0x30=0;
}

#include <cstdio>
#include <cstring>
static int errors,step,lookups,live,scenario;
static JackMethods* group;
static JackThing actor,target,changedActor;
static CWorld world;
static CCombatManager combat;
static HitLocations hits, sentinelHits;
static HitLocation locations[3];
static const char* names[]={"HIT_LOCATION_JACK_OF_BLADES","HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD","HIT_LOCATION_JACK_OF_BLADES_BIG"};
static const char* activeName;
CCharString::CCharString(const char* name,long length){if(live||length!=-1)++errors;activeName=name;++live;}
CCharString::~CCharString(){if(live!=1)++errors;--live;}
JackThing* JackPointer::GetPItem()const{if(this!=(JackPointer*)group->_pad_0x24||step!=1)++errors;step=2;return &target;}
CWorld* JackMethods::GetPWorld(){if(step)++errors;step=1;return &world;}
void CCombatManager::RegisterCombatant(JackThing* a,JackThing* b,bool flag){if(this!=&combat||a!=&actor||b!=&target||flag||step!=2)++errors;step=3;
#ifdef _MSC_VER
((CAIStateGroupBase*)group)->PBrain->Creature=(CThingAICreature*)&changedActor;
#else
 group->PBrain->Creature=&changedActor;
#endif
}
JackEntry* JackTable::FindEntry(const long& key){if(this!=&changedActor.components||key!=127||step!=3)++errors;step=4;return scenario==0?begin:scenario==1?end:begin;}
HitLocation* HitLocations::GetHitLocation(const CCharString& name){if(this!=(scenario==1||scenario==2?&sentinelHits:&hits)||live!=1||lookups>=3||std::strcmp(activeName,names[lookups])||step!=4+lookups*2)++errors;++step;return &locations[lookups++];}
void HitLocation::SetEnabled(bool enabled){if(live||this!=&locations[lookups-1]||enabled!=(lookups==3)||step!=3+lookups*2||*(int*)group->_pad_0x30!=99)++errors;++step;}
int main(){JackMethods g;group=&g;CAIBrain brain;
#ifdef _MSC_VER
((CAIStateGroupBase*)&g)->PBrain=&brain;
#else
g.PBrain=&brain;
#endif
world.PThingCombatManager_ptr=&combat;JackEntry entries[2];entries[0].value=&hits;entries[1].value=&sentinelHits;changedActor.mask=0x80000000;changedActor.components.begin=entries;changedActor.components.end=entries+1;
for(scenario=0;scenario<4;++scenario){step=lookups=live=0;entries[0].type=scenario==2?128:scenario==3?126:127;*(int*)g._pad_0x30=99;
#ifdef _MSC_VER
brain.Creature=(CThingAICreature*)&actor;
#else
brain.Creature=&actor;
#endif
g.OnActivate();if(step!=10||lookups!=3||live||*(int*)g._pad_0x30)++errors;}
if(errors){std::printf("00972490_TEST FAIL %d\n",errors);return 1;}std::printf("00972490_TEST PASS\n");return 0;}
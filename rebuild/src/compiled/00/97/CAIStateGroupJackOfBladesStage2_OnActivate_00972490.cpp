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
// CAIStateGroup_KidHurtSelfRunToMother::Cleanup() -- remove added sad mode and the mother's matching child event.
// Retail00915190 length245. Shared group/brain headers; native thing mask24/28, table44, dead91,parent180.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#else
#define __forceinline inline
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_KidHurtSelfRunToMother:CAIStateGroupBase{bool AddedSadMode;};
#endif
class CBaseObject;
class CBaseIntelligentPointer{ public:public:
    CBaseObject* GetPItem() const;   // 0x00A01B50
}; //00a01b50
struct KidEvent{char base[8];CBaseIntelligentPointer child;};
struct KidEventType{unsigned long type;KidEventType(unsigned long t):type(t){}};
struct KidEvents{bool GetEvent(KidEvent**,const KidEventType&);void RemoveEvent(KidEvent*); __forceinline KidEvent* FindChild(){KidEvent* event=0;GetEvent(&event,KidEventType(24));return event;}}; //00915760,008112d0
class CTCCreatureModeManager{ public:public:
    void RemoveMode(unsigned long);   // 0x0073ABA0
}; //0073aba0
struct KidEntry{long key;void* value;};
struct KidTable{KidEntry* begin;KidEntry* end;KidEntry* FindEntry(const long&); //0040f020
 template<int Key> __forceinline void* Lookup(){long key=Key;KidEntry* p=FindEntry(key);if(p==end)p=end;else if(p->key>Key)p=end;return p->value;}
};
class CBaseObject{ public:char beforeMask[0x24];unsigned long modesMask,eventsMask;char beforeTable[0x18];KidTable table;char beforeDead[0x45];unsigned char dead;char beforeParent[0xee];CBaseIntelligentPointer parent;
__forceinline void GetModes(CTCCreatureModeManager** p){if(modesMask&0x20000)*p=(CTCCreatureModeManager*)table.Lookup<49>();}
__forceinline void GetEvents(KidEvents** p){if(eventsMask&4)*p=(KidEvents*)table.Lookup<66>();}
};
struct KidCleanup:CAIStateGroup_KidHurtSelfRunToMother{
__forceinline CBaseObject* Creature(){return (CBaseObject*)((CAIStateGroupBase*)this)->PBrain->Creature;}
void Cleanup();};
void KidCleanup::Cleanup(){
if(AddedSadMode){CTCCreatureModeManager* modes;Creature()->GetModes(&modes);modes->RemoveMode(20);}
CBaseObject* mother=Creature()->parent.GetPItem();
if(mother&&!(mother->dead&1)){KidEvents* events;mother->GetEvents(&events);KidEvent* found=events->FindChild();if(found&&found->child.GetPItem()==Creature())events->RemoveEvent(found);}
}
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#define FABLE_ENGINE_CAISTATEGROUP_RECOVERSTEALABLEITEM_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTCENTITYEVENTS_H
struct CAIStateGroup_RecoverStealableItem { unsigned char _pad_0x04[sizeof(void*)]; bool NeedsCleanup; };
class CThingAICreature;
class CAIBrain { public: CThingAICreature* Creature; };
class CTCEntityEvents { public: unsigned char Events[sizeof(void*)]; };
#endif
// BEGIN EXACT CANDIDATE
// CAIStateGroup_RecoverStealableItem::IsStillValid() -- invalidate recovery when disabled or interrupted by hostile events
// Retail0x00965520,279 bytes including embedded switch tables. LowerBound0x4365b0.
#include "engine/CAIStateGroup_RecoverStealableItem.h"
#include "engine/CAIBrain.h"
// No shared CTCAIScratchpad header; PDB flags at104/105.
class CTCAIScratchpad { public: unsigned char prefix[104]; bool StealStealableItems, RecoverStealableItems; };
#include "engine/CTCEntityEvents.h"
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*); };
class CThing { public:
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead;
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        if(bits[type>>5]&(1UL<<(type&31))) {
            long key=type; CInterfaceMap* map=&interfaces; CInterfaceMap::Entry* it=map->LowerBound(&key);
            if(it==map->last || it->type>type) it=map->last;
            *out=(T*)it->component; return true;
        } return false;
    }
};

struct CEventBase { virtual ~CEventBase(); virtual long GetType() const; long WorldFrame; };
struct CEventHitBySpecialAbility : CEventBase { unsigned char Source[8]; long Ability; bool Aggressive; };
struct EventNode { EventNode *next,*previous; CEventBase *data; void *info; };
struct CAIStateGroup_RecoverStealableItem_Adapter : CAIStateGroup_RecoverStealableItem {
 CAIBrain* Brain() const { return *(CAIBrain* const*)&_pad_0x04[0]; }
 bool IsStillValid();
};
bool CAIStateGroup_RecoverStealableItem_Adapter::IsStillValid() {
 CThing* creature=(CThing*)Brain()->Creature;
 CTCAIScratchpad* scratch;
 creature->GetInterface(&scratch,0xaa);
 if(!scratch->RecoverStealableItems) { NeedsCleanup=true; return false; }
 bool interrupted=false;
 CTCEntityEvents* events;
 creature->GetInterface(&events,0x42);
 for(EventNode* node=(*(EventNode**)events->Events)->next; node!=*(EventNode**)events->Events; node=node->next) {
  CEventBase* event=node->data;
  switch(event->GetType()) {
   case 1: case 6: case 13: interrupted=true; break;
   case 27: if(((CEventHitBySpecialAbility*)event)->Aggressive) interrupted=true; break;
  }
 }
 if(interrupted) { NeedsCleanup=true; return false; }
 return true;
}
// END EXACT CANDIDATE
static int lookups, calls, failures, fallback;
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* key) {
 ++lookups;
 if(fallback) return last;
 return *key==0xaa ? first+1 : first;
}
CEventBase::~CEventBase() {}
long CEventBase::GetType() const { return 0; }
struct TestEvent : CEventHitBySpecialAbility { long type; virtual long GetType() const { ++calls; return type; } };
static void Check(bool b) { if(!b) ++failures; }
int main() {
 CAIStateGroup_RecoverStealableItem_Adapter state;
 CAIBrain brain; CThing creature={0}; CTCAIScratchpad scratch={0}; CTCEntityEvents events;
 *(CAIBrain**)state._pad_0x04=&brain; brain.Creature=(CThingAICreature*)&creature;
 CInterfaceMap::Entry entries[3]; entries[0].type=0x42; entries[0].component=&events; entries[1].type=0xaa; entries[1].component=&scratch;
 creature.interfaces.first=entries; creature.interfaces.last=entries+2;
 creature.bits[0xaa>>5]=1UL<<(0xaa&31); creature.bits[0x42>>5]=1UL<<(0x42&31);
 EventNode head, nodes[3]; TestEvent event[3]; *(EventNode**)events.Events=&head; head.next=&head;
 state.NeedsCleanup=false; lookups=calls=0;
 Check(!state.IsStillValid()); Check(state.NeedsCleanup && lookups==1 && calls==0);
 scratch.RecoverStealableItems=true; state.NeedsCleanup=false; lookups=0;
 Check(state.IsStillValid()); Check(!state.NeedsCleanup && lookups==2);
 for(int type=0;type<31;++type) for(int aggressive=0;aggressive<2;++aggressive) {
  event[0].type=type; event[0].Aggressive=aggressive!=0; nodes[0].data=&event[0]; nodes[0].next=&head; head.next=nodes;
  state.NeedsCleanup=false; lookups=calls=0;
  bool bad=type==1 || type==6 || type==13 || (type==27 && aggressive);
  Check(state.IsStillValid()==!bad); Check(state.NeedsCleanup==bad); Check(lookups==2 && calls==1);
 }
 for(int i=0;i<3;++i) { nodes[i].data=&event[i]; nodes[i].next=i==2?&head:&nodes[i+1]; event[i].type=i==0?1:0; }
 calls=0; state.NeedsCleanup=false; Check(!state.IsStillValid()); Check(calls==3 && state.NeedsCleanup);
 head.next=&head; state.NeedsCleanup=true; Check(state.IsStillValid()); Check(state.NeedsCleanup);
 // End-entry fallback for each lookup is unchecked by the retail function.
 fallback=1; entries[2].component=&scratch; scratch.RecoverStealableItems=false; state.NeedsCleanup=false;
 Check(!state.IsStillValid()); Check(state.NeedsCleanup);
 if(failures) { std::printf("00965520_TEST FAIL %d\n",failures); return 1; }
 std::printf("00965520_TEST PASS\n"); return 0;
}
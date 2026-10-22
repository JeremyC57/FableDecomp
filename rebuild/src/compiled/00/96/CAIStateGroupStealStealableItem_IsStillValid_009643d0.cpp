// CAIStateGroup_StealStealableItem::IsStillValid() -- invalidate stealy when disabled or interrupted by hostile events
// Retail0x009643d0,279 bytes including embedded switch tables. LowerBound0x4365b0.
#include "engine/CAIStateGroup_StealStealableItem.h"
#include "engine/CAIBrain.h"
// No shared CTCAIScratchpad header; PDB flags at104/105.
struct CTCAIScratchpad { unsigned char prefix[104]; bool StealStealableItems, RecoverStealableItems; };
#include "engine/CTCEntityEvents.h"
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*); };
struct CThing {
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
struct CAIStateGroup_StealStealableItem_Adapter : CAIStateGroup_StealStealableItem {
 CAIBrain* Brain() const { return *(CAIBrain* const*)&_pad_0x04[0]; }
 bool IsStillValid();
};
bool CAIStateGroup_StealStealableItem_Adapter::IsStillValid() {
 CThing* creature=(CThing*)Brain()->Creature;
 CTCAIScratchpad* scratch;
 creature->GetInterface(&scratch,0xaa);
 if(!scratch->StealStealableItems) { NeedsCleanup=true; return false; }
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
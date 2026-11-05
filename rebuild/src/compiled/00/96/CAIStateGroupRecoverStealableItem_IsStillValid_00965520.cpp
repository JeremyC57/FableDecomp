// CAIStateGroup_RecoverStealableItem::IsStillValid() -- invalidate recovery when disabled or interrupted by hostile events
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members first/last are the header CArray base's begin() / end() (STLport vector start +0 / finish +4); map pointer made const (the row grades the const LowerBound).
// Retail0x00965520,279 bytes including embedded switch tables. LowerBound0x4365b0.
#include "engine/CAIStateGroup_RecoverStealableItem.h"
#include "engine/CAIBrain.h"
// No shared CTCAIScratchpad header; PDB flags at104/105.
class CTCAIScratchpad { public: unsigned char prefix[104]; bool StealStealableItems, RecoverStealableItems; };
#include "engine/CTCEntityEvents.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead;
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        if(bits[type>>5]&(1UL<<(type&31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type); const CInterfaceMap* map=&interfaces; const CInterfaceMap::value_type* it=map->LowerBound(key);
            if(it==map->end() || it->first>type) it=map->end();
            *out=(T*)((void*)it->second); return true;
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
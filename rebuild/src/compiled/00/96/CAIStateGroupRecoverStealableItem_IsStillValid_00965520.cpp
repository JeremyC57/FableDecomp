// CAIStateGroup_RecoverStealableItem::IsStillValid() -- invalidate recovery when disabled or interrupted by hostile events
// Retail0x00965520,279 bytes including embedded switch tables. LowerBound0x4365b0.
#include "engine/CAIStateGroup_RecoverStealableItem.h"
#include "engine/CAIBrain.h"
// No shared CTCAIScratchpad header; PDB flags at104/105.
class CTCAIScratchpad { public: unsigned char prefix[104]; bool StealStealableItems, RecoverStealableItems; };
#include "engine/CTCEntityEvents.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public: typedef std::pair<ETCInterfaceType, CTCBase*> Entry; Entry *first,*last; const Entry* LowerBound(const ETCInterfaceType& key) const; };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead;
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        if(bits[type>>5]&(1UL<<(type&31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type); CInterfaceMap* map=&interfaces; const CInterfaceMap::Entry* it=map->LowerBound(key);
            if(it==map->last || it->first>type) it=map->last;
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
// CAIStateGroup_RecoverStealableItem::IsStillValid() -- invalidate recovery when disabled or interrupted by hostile events
// Leaf: ?IsStillValid@CAIStateGroup_RecoverStealableItem_Adapter@@QAE_NXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members first/last are the header CArray base's begin() / end() (STLport vector start +0 / finish +4); map pointer made const (the row grades the const LowerBound).
// Retail0x00965520,279 bytes including embedded switch tables. LowerBound0x4365b0.
#include "engine/CAIStateGroup_RecoverStealableItem.h"
#include "engine/CAIBrain.h"
// No shared CTCAIScratchpad header; PDB flags at104/105.
class CTCAIScratchpad { public: unsigned char prefix[104]; bool StealStealableItems, RecoverStealableItems; };
#include "engine/CTCEntityEvents.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CEventBase { virtual ~CEventBase(); virtual long GetType() const; long WorldFrame; };
struct CEventHitBySpecialAbility : CEventBase { unsigned char Source[8]; long Ability; bool Aggressive; };
struct EventNode { EventNode *next,*previous; CEventBase *data; void *info; };
struct CAIStateGroup_RecoverStealableItem_Adapter : CAIStateGroup_RecoverStealableItem {
 CAIBrain* Brain() const { return *(CAIBrain* const*)&_pad_0x04[0]; }
 bool IsStillValid();
};
bool CAIStateGroup_RecoverStealableItem_Adapter::IsStillValid() {
 CThing* creature=(CThing*)Brain()->Creature;
 const CTCAIScratchpad* scratch;
 creature->PeekTC(TCI_AI_SCRATCHPAD, &scratch);
 if(!scratch->RecoverStealableItems) { NeedsCleanup=true; return false; }
 bool interrupted=false;
 const CTCEntityEvents* events;
 creature->PeekTC(TCI_ENTITY_EVENTS, &events);
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
#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTRECEIVEDITEM_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTRECEIVEDITEM_H
#ifdef FABLE_RETAIL_GIVE_ITEM_ACTION_PASS26
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CIntelligentPointer.h"
namespace NEntityEvents {
class CEventReceivedItem : public CEventBase {
public:
    CEventReceivedItem(CThing& thing_giving_item, CThing& item_received, bool kill_received_item);
    CIntelligentPointer<CThing> ThingGivingItem;
    CIntelligentPointer<CThing> ItemReceived;
    bool KillReceivedItem;
};
}
FABLE_STATIC_ASSERT_AT(940,sizeof(NEntityEvents::CEventReceivedItem)==28);
#endif
#endif

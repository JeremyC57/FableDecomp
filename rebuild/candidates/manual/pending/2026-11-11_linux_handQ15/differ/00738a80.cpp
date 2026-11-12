// CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy, CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee, std::unary_negate<NEntityEvents::CIsHitEventBlocked>, NEntityEvents::CIsHitEventHitBy> >(const NEntityEvents::CEventHitBy** out, const CFilter_Bind3<..>& filter) const -- the newest hit-by event that was a melee strike, not blocked, by the given thing: stored to *out, true; false when none
// Grade: DIFFER(ours v retail) -- 177v177, ported onto engine/CTCEntityEvents.h (retail list), CCountedPointer.h, NEntityEvents_* and CFilter_Bind3.h: register swap only -- retail keeps the copied pointer's Data in esi and Info in edi, ours the reverse (c2prio: Info prio 80 esi, Data 76 edi; Data needs +4, it wins a tie by key 16 v 15). Tried: cast twice / one typed local, ReleaseData dtor
// Leaf: ??$GetEvent@$$CBVCEventHitBy@NEntityEvents@@V?$CFilter_Bind3@VCIsHitEventHitByMelee@NEntityEvents@@V?$unary_negate@VCIsHitEventBlocked@NEntityEvents@@@std@@VCIsHitEventHitBy@2@@@@CTCEntityEvents@@QBE_NPAPBVCEventHitBy@NEntityEvents@@ABV?$CFilter_Bind3@VCIsHitEventHitByMelee@NEntityEvents@@V?$unary_negate@VCIsHitEventBlocked@NEntityEvents@@@std@@VCIsHitEventHitBy@2@@@@Z
// Retail: Fable.exe 0x00738A80, 177 bytes, thiscall, `ret 8` (Ego_r.pdb / FableWin.pdb name above, the CTCEntityEvents member template).
// Callees: CEventBase::GetType (slot 1), CIsHitEventHitByMelee::operator() 0x00810F80, CIsHitEventBlocked::operator() 0x00810F50
//   (through std::unary_negate), CEventHitBase::GetPHitBy 0x00810FC0 (inside the inline CIsHitEventHitBy), operator delete 0x00BFE9BC.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBY_TYPE
#define FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY
#include "engine/CThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/NEntityEvents_EventFilters.h"
#include "engine/CFilter_Bind3.h"

// The newest event of TEvent's type that passes the filter: stored to *out, true; false when none.
template <class TEvent, class TFilter>
bool CTCEntityEvents::GetEvent(TEvent** out, const TFilter& filter) const
{
    for (std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_reverse_iterator it = Events.rbegin(); it != Events.rend(); ++it)
    {
        CCountedPointer<NEntityEvents::CEventBase> event = *it;
        if (event->GetType() == TEvent::StatGetType())
        {
            TEvent* typed_event = GFDebugPDynamicCast(event.Get(), (TEvent*)0);
            if (filter(typed_event))
            {
                *out = typed_event;
                return true;
            }
        }
    }
    return false;
}

template bool CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy, CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee, std::unary_negate<NEntityEvents::CIsHitEventBlocked>, NEntityEvents::CIsHitEventHitBy> >(const NEntityEvents::CEventHitBy**, const CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee, std::unary_negate<NEntityEvents::CIsHitEventBlocked>, NEntityEvents::CIsHitEventHitBy>&) const;

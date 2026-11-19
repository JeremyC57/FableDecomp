// CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy, CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee, std::unary_negate<NEntityEvents::CIsHitEventBlocked>, NEntityEvents::CIsHitEventHitBy> >(const NEntityEvents::CEventHitBy** out, const CFilter_Bind3<..>& filter) const -- the newest hit-by event that was a melee strike, not blocked, by the given thing: stored to *out, true; false when none
// Leaf: ??$GetEvent@$$CBVCEventHitBy@NEntityEvents@@V?$CFilter_Bind3@VCIsHitEventHitByMelee@NEntityEvents@@V?$unary_negate@VCIsHitEventBlocked@NEntityEvents@@@std@@VCIsHitEventHitBy@2@@@@CTCEntityEvents@@QBE_NPAPBVCEventHitBy@NEntityEvents@@ABV?$CFilter_Bind3@VCIsHitEventHitByMelee@NEntityEvents@@V?$unary_negate@VCIsHitEventBlocked@NEntityEvents@@@std@@VCIsHitEventHitBy@2@@@@Z
// Retail: Fable.exe 0x00738A80, 177 bytes, thiscall, `ret 8` (Ego_r.pdb / FableWin.pdb name above, the CTCEntityEvents member template).
// Callees: CEventBase::GetType (slot 1), CIsHitEventHitByMelee::operator() 0x00810F80, CIsHitEventBlocked::operator() 0x00810F50
//   (through std::unary_negate), CEventHitBase::GetPHitBy 0x00810FC0 (inside the inline CIsHitEventHitBy), operator delete 0x00BFE9BC.
// Source: FableWin.pdb debug body 0x0250BD30 (rbegin / rend per pass, the counted-pointer copy, operator->()->GetType() against
//   StatGetType, GFDebugPDynamicCast(get()), the filter on the typed pointer by const reference, *out, return). No Ego_d record survives.
// What closed it: every byte matched except event.Data / event.Info in esi / edi swapped (c2: Info 80 v Data 76). The inline
//   CIsHitEventHitBy::operator() is written with its original named local (Ego_d.pdb locals pevent, phit_by; FableWin 0x0250E970 stores
//   GetPHitBy() to a local before the compare): new overlay opt-in FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY_LOCAL
//   (hdr/engine/NEntityEvents_EventFilters.h). The local is one more candidate in the HitBy block (K 3 -> 4): Data 80, Info 78 -> retail's
//   esi = Data. The landed sibling 0x0096BB70 (third filter CIsEventCreatedAfter) keeps Info in esi, as its retail does. No flags.
#define FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY_LOCAL
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
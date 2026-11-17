// CAIStateGroup_CarryCrateAndFollow::IsExpressionPerfomedTo(const CCharString& expression, CIntelligentPointer<CThing>& performer) const -- look for an expression-performed-to event naming this expression; on a hit hand back its performer and consume the event
// Leaf: ?IsExpressionPerfomedTo@CAIStateGroup_CarryCrateAndFollow@@ABE_NABVCCharString@@AAV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00966290, 256 bytes (`ret 8`).
// Callees: CVectorMap::LowerBound 0x0040F020 (inline GetTC<CTCEntityEvents>, 0x42), the event's GetType (slot 1), the expression def's
// GetInstantiationName (CDefClassBase slot 10, CDefString by hidden pointer), CDefString::operator==(const CCharString&) 0x004B7980 (its
// argument pushed before the virtual call), the def's release (slot 1), the CIntelligentPointer<CThing> temporary (vtable 0x01238C6C:
// GetPItem 0x00A01B50, ctor 0x00A01B10, SetPItem 0x00A01B90 x2, dtor 0x00A01C10), CTCEntityEvents::RemoveEvent 0x008112D0.
// Shape: the forward walk of CTCEntityEvents::GetEvents() re-reading the sentinel each pass (the 0x0097FEA0 FindAGuardAndYellForHelp loop);
// a counted copy of the expression def for the compare; on a match `performer = CIntelligentPointer<CThing>(performed_by)`, then the event
// is removed.
// NF67 (round 7): ported onto the shared headers (the 0x0097FEA0 event-walk block). The CTCMessageReceiver / CMessageList / CMessage /
//   CExpressionPayload / CIVCountedPointer / CIntelligentPointerThingC / CThing / CAIBrain / CAIStateGroupBase stand-ins are gone; overlay
//   opt-ins FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW_EXPRESSION (the private const member), FABLE_RETAIL_CDEFSTRING_EQ_CHARSTRING,
//   FABLE_RETAIL_CEVENTEXPRESSIONPERFORMEDTO_GETTERS. 0x008112D0 now binds ?RemoveEvent@CTCEntityEvents@@QAEXAAVCEventBase@NEntityEvents@@@Z (nf41:e80).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_EQ_CHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_EVENTS
#define FABLE_RETAIL_CEVENTEXPRESSIONPERFORMEDTO_GETTERS
#define FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW
#define FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW_EXPRESSION
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CarryCrateAndFollow.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_EEventType.h"
#include "engine/NEntityEvents_CEventExpressionPerformedTo.h"
#include "engine/CExpressionDef.h"
#include "engine/GFDebugPDynamicCast.h"

bool CAIStateGroup_CarryCrateAndFollow::IsExpressionPerfomedTo(const CCharString& expression, CIntelligentPointer<CThing>& performer) const
{
    CTCEntityEvents* ptc_events;
    if (GetCreature().GetTC(&ptc_events)) {
        for (std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator it = ptc_events->GetEvents().begin();
             it != ptc_events->GetEvents().end(); ++it) {
            NEntityEvents::CEventBase* event = (*it).Get();
            if (event->GetType() == NEntityEvents::CEventExpressionPerformedTo::StatGetType()) {
                NEntityEvents::CEventExpressionPerformedTo* expression_event = GFDebugPDynamicCast(event, (NEntityEvents::CEventExpressionPerformedTo*)0);
                if (expression_event->GetPExpressionDef()->GetInstantiationName() == expression) {
                    performer = CIntelligentPointer<CThing>(expression_event->GetPPerformedBy());
                    ptc_events->RemoveEvent(*event);
                    return true;
                }
            }
        }
    }
    return false;
}

// CAIStateGroup_MinionFollowThing::IsExpressionPerfomedTo(const CCharString& expression) -- true (and the event consumed) when an expression-performed-to event (type 3) naming this expression is pending and the scratchpad allows following; EXPRESSION_FOLLOW retargets the followee, the event's extra data overrides the follow distance and the obstacle flag
// Leaf: ?IsExpressionPerfomedTo@CAIStateGroup_MinionFollowThing@@IAE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00921440, length 405 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inline GetTC<CTCEntityEvents>, 0x42), its const twin @ 0x004365B0 (inline
// PeekTC<const CTCAIScratchpad>, 0xAA), CTCEntityEvents::GetEvent<CEventExpressionPerformedTo, CIsEventOfType> @ 0x00922440, the expression
// def's GetInstantiationName (CDefClassBase slot 10, by value), CDefString::operator==(const CCharString&) @ 0x004B7980,
// CTCEntityEvents::RemoveEvent @ 0x008112D0, CBasicString<char>::operator==(const char*) @ 0x004115A0 (inline CCharString == "EXPRESSION_FOLLOW",
// strcmp intrinsic on the null string), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CIntelligentPointer<CThingCreatureBase>(T*) @
// 0x00668B60 (out of line), SetPItem @ 0x00A01B90, ~CBaseIntelligentPointer @ 0x00A01C10.
// NF67 (round 7): ported onto the shared headers. The local CThing / CBasicString / CCharStringData / CCharString / CDefString /
//   CExpressionDef / CDefPointerExpression / CIntelligentPointer stand-ins / CEventExpressionPerformedTo / CTCEntityEvents / CTCAIScratchpad /
//   CAIBrain / CAIStateGroupBase models, the local ETCInterfaceType and the same-TU stand-in GetEvent body (TmplSink) are gone: the real
//   GetEvent template body comes from overlay opt-in FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY; overlay opt-ins
//   FABLE_RETAIL_CAISTATEGROUP_MINIONFOLLOWTHING, FABLE_RETAIL_CDEFSTRING_EQ_CHARSTRING, FABLE_RETAIL_CEVENTEXPRESSIONPERFORMEDTO_GETTERS.
//   NEntityEvents::CIsEventOfType has no shared header (local, FableWin.pdb ctor (EEventType) / operator()(const CEventBase*) const).
//   0x008112D0 now binds ?RemoveEvent@CTCEntityEvents@@QAEXAAVCEventBase@NEntityEvents@@@Z (nf41:e80).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_EQ_CHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_EVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY
#define FABLE_RETAIL_CEVENTEXPRESSIONPERFORMEDTO_GETTERS
#define FABLE_RETAIL_CAISTATEGROUP_MINIONFOLLOWTHING
#include <string.h>
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MinionFollowThing.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_EEventType.h"
#include "engine/NEntityEvents_CEventExpressionPerformedTo.h"
#include "engine/CExpressionDef.h"

namespace NEntityEvents {
// Local model (no shared header): FableWin.pdb ??0CIsEventOfType@NEntityEvents@@QAE@W4EEventType@1@@Z / ??R..QBE_NPBVCEventBase@1@@Z.
class CIsEventOfType {
public:
    CIsEventOfType(EEventType type) : Type(type) {}
    bool operator()(const CEventBase* event) const { return event->GetType() == Type; }
private:
    EEventType Type;
};
}

bool CAIStateGroup_MinionFollowThing::IsExpressionPerfomedTo(const CCharString& expression)
{
    CTCEntityEvents* ptc_events;
    if (GetCreature().GetTC(&ptc_events)) {
        NEntityEvents::CEventExpressionPerformedTo* event = 0;
        if (ptc_events->GetEvent(&event, NEntityEvents::CIsEventOfType(NEntityEvents::CEventExpressionPerformedTo::StatGetType()))) {
            if (event->GetPExpressionDef()->GetInstantiationName() == expression) {
                const CTCAIScratchpad* ptc_scratchpad;
                if (!GetCreature().PeekTC(&ptc_scratchpad) || ptc_scratchpad->AllowedToFollow) {
                    if (expression == "EXPRESSION_FOLLOW")
                        PFollowee = event->GetPPerformedBy();
                    if (event->GetExtraData1() > -1)
                        FollowDistance = (float)event->GetExtraData1();
                    if (event->GetExtraData2() > -1)
                        AvoidDynamicObstacles = event->GetExtraData2() != 0;
                    ptc_events->RemoveEvent(*event);
                    return true;
                }
                ptc_events->RemoveEvent(*event);
            }
        }
    }
    return false;
}

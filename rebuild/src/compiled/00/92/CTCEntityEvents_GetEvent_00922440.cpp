// CTCEntityEvents::GetEvent<NEntityEvents::CEventExpressionPerformedTo, NEntityEvents::CIsEventOfType>(CEventExpressionPerformedTo** out,
// const CIsEventOfType& filter) const -- the newest event of type 3 (CEventExpressionPerformedTo) that the filter accepts
// Leaf: ??$GetEvent@VCEventExpressionPerformedTo@NEntityEvents@@VCIsEventOfType@2@@CTCEntityEvents@@QBE_NPAPAVCEventExpressionPerformedTo@NEntityEvents@@ABVCIsEventOfType@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00922440 (`ret 8`, bool in al).
// NF72 (round 7, q10866): moved off the local stand-in CTCEntityEventsMethods (and its local CEventBase / list / filter models) onto
//   engine/CTCEntityEvents.h's member-template body FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY (NF67) and the shared
//   engine/NEntityEvents_CEventExpressionPerformedTo.h, with the macro block and the local CIsEventOfType model (no shared header) of its
//   ported caller CAIStateGroup_MinionFollowThing::IsExpressionPerfomedTo 0x00921440 (NF67). Evidence: FableWin.pdb names this
//   instantiation (_fablewin_api.tsv); 0x00921440 binds it.
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

template bool CTCEntityEvents::GetEvent<NEntityEvents::CEventExpressionPerformedTo, NEntityEvents::CIsEventOfType>(NEntityEvents::CEventExpressionPerformedTo** out, const NEntityEvents::CIsEventOfType& filter) const;

// CTCEntityEvents::GetEvent<NEntityEvents::CEventChildHurtGoToMother, NEntityEvents::CIsEventOfType>(CEventChildHurtGoToMother** out,
// const CIsEventOfType& filter) const -- the newest event of type 0x18 (CEventChildHurtGoToMother) that the filter accepts
// Leaf: ??$GetEvent@VCEventChildHurtGoToMother@NEntityEvents@@VCIsEventOfType@2@@CTCEntityEvents@@QBE_NPAPAVCEventChildHurtGoToMother@NEntityEvents@@ABVCIsEventOfType@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00915760, 141 bytes (+ int3 padding to 144) (`ret 8`, bool in al).
// Callees: GetType (vtable slot 1, twice: the type test and CIsEventOfType), the count block DeleteFunc, operator delete 0x00BFE9BC.
// NF72 (round 7, q10865): moved off the local stand-in CTCEntityEventsMethods (and its local CEventBase / list / filter models) onto
//   engine/CTCEntityEvents.h's member-template body FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY (NF67), with the macro block and the two
//   local event / filter models of its only retail caller CAIStateGroup_KidHurtSelfRunToMother::Cleanup 0x00915190 (NF67; neither class
//   has a shared header). Evidence: FableWin.pdb names this instantiation (_fablewin_api.tsv); 0x00915190 binds it.
#define FABLE_RETAIL_KID_HURT_SELF_PASS23
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_GETMOTHERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_EVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_EEventType.h"
#include "engine/NEntityEvents_CEventExpressionPerformedTo.h"
#include "engine/ThingAlive.h"
namespace NEntityEvents {
// Local model (no shared header): Ego_r.pdb CEventChildHurtGoToMother (16 bytes), FableWin.pdb members.
class CEventChildHurtGoToMother : public CEventBase {
public:
    CEventChildHurtGoToMother(const CThing* child);
    virtual ~CEventChildHurtGoToMother();
    virtual EEventType GetType() const;
    virtual bool IsReadyToDie() const;
    static EEventType FABLE_FASTCALL StatGetType() { return EVENT_CHILD_HURT_GO_TO_MOTHER; }
    CIntelligentPointer<const CThing> PChild;                            // +0x08
};
// Local model (no shared header): FableWin.pdb ??0CIsEventOfType@NEntityEvents@@QAE@W4EEventType@1@@Z / ??R..QBE_NPBVCEventBase@1@@Z.
class CIsEventOfType {
public:
    CIsEventOfType(EEventType type) : Type(type) {}
    bool operator()(const CEventBase* event) const { return event->GetType() == Type; }
private:
    EEventType Type;
};
}

template bool CTCEntityEvents::GetEvent<NEntityEvents::CEventChildHurtGoToMother, NEntityEvents::CIsEventOfType>(NEntityEvents::CEventChildHurtGoToMother** out, const NEntityEvents::CIsEventOfType& filter) const;

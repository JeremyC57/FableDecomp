// CAIStateGroup_KidHurtSelfRunToMother::Cleanup() -- remove the added sad mode and the mother's matching child-hurt event.
// Leaf: ?Cleanup@CAIStateGroup_KidHurtSelfRunToMother@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00915190, 245 bytes. Callees: CVectorMap::LowerBound 0x0040F020 (inline GetTC:
//   CTCCreatureModeManager 49, CTCEntityEvents 66), CTCCreatureModeManager::RemoveMode 0x0073ABA0, CBaseIntelligentPointer::GetPItem
//   0x00A01B50 (MotherCreature +0x180, the event's PChild +0x08), CTCEntityEvents::GetEvent<CEventChildHurtGoToMother, CIsEventOfType>
//   0x00915760 (out of line), CTCEntityEvents::RemoveEvent(CEventBase&) 0x008112D0.
// NF67 (round 7): ported onto the shared headers (the sibling Update 0x00915350 macro block + CTCEntityEvents EVENTS). The KidCleanup /
//   KidEvent(s) / KidEventType / KidTable / KidEntry / local CBaseObject / CTCCreatureModeManager models are gone. NEntityEvents::
//   CEventChildHurtGoToMother and NEntityEvents::CIsEventOfType have no shared header: local, from Ego_r.pdb / FableWin.pdb (PChild
//   CIntelligentPointer<const CThing> +0x08; ctor (EEventType), operator()(const CEventBase*) const). CEventBase is the shared one
//   (engine/NEntityEvents_CEventExpressionPerformedTo.h). Overlay opt-in FABLE_RETAIL_CTHINGAICREATURE_GETMOTHERCREATURE (FableWin
//   ?GetMotherCreature@CThingAICreature@@QBEPAV1@XZ) and FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_BODY (the GetEvent template body, noinline:
//   with it visible VC7.1 keeps the found event in esi across the PChild GetPItem call, as retail; declaration-only a1 reloads it,
//   DIFFER 249v245). 0x008112D0 now binds ?RemoveEvent@CTCEntityEvents@@QAEXAAVCEventBase@NEntityEvents@@@Z
//   (nf41:e80) and 0x00915760 the PDB GetEvent instantiation.
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

void CAIStateGroup_KidHurtSelfRunToMother::Cleanup()
{
    if (AddedSadMode) {
        CTCCreatureModeManager* ptc_cmm;
        GetCreature().GetTC(&ptc_cmm);
        ptc_cmm->RemoveMode(20);
    }
    CThingAICreature* mother = GetCreature().GetMotherCreature();
    if (IS_THING_ALIVE(mother)) {
        CTCEntityEvents* ptc_events;
        mother->GetTC(&ptc_events);
        NEntityEvents::CEventChildHurtGoToMother* event = 0;
        ptc_events->GetEvent(&event, NEntityEvents::CIsEventOfType(NEntityEvents::CEventChildHurtGoToMother::StatGetType()));
        if (event && event->PChild.Get() == &GetCreature())
            ptc_events->RemoveEvent(*event);
    }
}

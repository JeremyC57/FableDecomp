// CGameScriptInterface::EntityStopFollowing(const CScriptThing&) -- if the scripted thing is alive and is one of the hero's followers, remove it from the hero's CTCFollowed, reset its AI and discard its pending expression-performed-to events
// Leaf: ?EntityStopFollowing@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A7980, 228 bytes, thiscall `ret 4` (vtable slot 230).
// NF67 port (round 7): onto the shared headers (the 0x008A7A70 TeleportAllFollowersToHeroPosition macro block, its TU neighbour):
//   CScriptThing slots 75 IsAlive / 11 GetPThing, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CTCFollowed::IsFollower 0x006D4D70 / RemoveFollower 0x006D5180 (TCI_FOLLOWED 188), CThingAICreature::ForceAIReset 0x006D5AB0 and
//   CTCEntityEvents::RemoveEvents<CIsEventOfType> (inline; overlay opt-in) over the list's remove_if 0x008AC890 (body visible and out of
//   line, overlay opt-in). NEntityEvents::CIsEventOfType / CConvFilterToCP have no shared header: defined here (Ego_r.pdb layouts:
//   EEventType Type +0 / F* Filter +0), as the landed remove_if row 0x008AC890 models them (filter body `e->GetType() == Type`).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCFOLLOWED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_NOINLINE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_REMOVEEVENTS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/NEntityEvents_EEventType.h"
#include "engine/NEntityEvents_CEventExpressionPerformedTo.h"   // the shared CEventBase (GetType, slot 1)
namespace NEntityEvents {
class CIsEventOfType {                                       // Ego_r.pdb: EEventType Type +0
public:
    CIsEventOfType(EEventType type) : Type(type) {}
    bool operator()(const CEventBase* event) const { return event->GetType() == Type; }
private:
    EEventType Type;
};
template<class F> class CConvFilterToCP {                    // Ego_r.pdb: F* Filter +0
public:
    CConvFilterToCP(const F& filter) : Filter(&filter) {}
    bool operator()(const CCountedPointer<CEventBase>& event) const { return (*Filter)(event.Get()); }
private:
    const F* Filter;
};
}
#include "engine/CScriptThing.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntityStopFollowing(const CScriptThing& script_thing) const
{
    if (script_thing.IsAlive()) {
        CThing* thing = script_thing.GetPThing();
        CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (hero && !hero->IsDead()) {
            CTCFollowed* followed;
            if (hero->GetTC(&followed) && followed->IsFollower(thing)) {
                followed->RemoveFollower(*thing);
                static_cast<CThingAICreature*>(thing)->ForceAIReset();
                CTCEntityEvents* events;
                if (thing->GetTC(&events))
                    events->RemoveEvents(NEntityEvents::CIsEventOfType(NEntityEvents::EVENT_EXPRESSION_PERFORMED_TO));
            }
        }
    }
}

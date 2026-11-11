// CGameScriptInterface::SendEntityEvent(EEventType type, const CScriptThing& recipient, const CScriptThing& spotter, CThing* target) const
// -- when both script things are alive, post the matching spotted-crime event (pickpocketing 19, picking a lock 20, stealing 21) to
// the recipient's entity-events component
// Leaf: ?SendEntityEvent@CGameScriptInterface@@UBEXW4EEventType@NEntityEvents@@ABVCScriptThing@@1PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008957C0, 293 bytes, `ret 10h`; CGameScriptInterface vtable 0x01260F0C slot 241 (Ego_r.pdb
// ?SendEntityEvent@CGameScriptInterface@@UBEXW4EEventType@NEntityEvents@@ABVCScriptThing@@1PAVCThing@@@Z; engine/CGameScriptInterface.h
// FABLE_RETAIL_NSCRIPT declares it at slot 241). Respelt by agentNF14 2026-11-11: was CGameScriptInterfaceMethods::SendEntityEvent over a
// local CThingNative / ScriptMethods projection with invented EventPickpocketing / EventPickingLock / EventStealing ctors.
// Callees: CScriptThing::IsAlive (slot 75, +0x12C) and GetPThing (slot 11, +0x2C); the CThing interface map's LowerBound @ 0x0040F020
// (GetTC<CTCEntityEvents>, TCI_ENTITY_EVENTS 66); operator new @ 0x00BFEA1A (0x18 bytes); the Ego_r.pdb event ctors
// NEntityEvents::CEventHeroHasBeenSpottedPickpocketing @ 0x008AB7B0 / ..PickingLock @ 0x008AB840 / ..Stealing @ 0x008AB8D0
// (CThing* target, CThing* spotter; vtables 0x012781B0 / 0x012781C8 / 0x012781E0); CTCEntityEvents::AddEvent @ 0x008113C0.
// Layout: the events are Ego_r.pdb CEventBase (vptr, WorldFrame) + PTarget +0x08 / PSpotter +0x10 (CIntelligentPointer<CThing>).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

namespace NEntityEvents {
class CEventHeroHasBeenSpottedPickpocketing : public CEventBase {      // Ego_r.pdb, 0x18 bytes, vtable 0x012781B0
public:
    CEventHeroHasBeenSpottedPickpocketing(CThing* target, CThing* spotter);   // 0x008AB7B0
    CIntelligentPointer<CThing> PTarget;                                       // +0x08
    CIntelligentPointer<CThing> PSpotter;                                      // +0x10
};
class CEventHeroHasBeenSpottedPickingLock : public CEventBase {        // Ego_r.pdb, 0x18 bytes, vtable 0x012781C8
public:
    CEventHeroHasBeenSpottedPickingLock(CThing* target, CThing* spotter);     // 0x008AB840
    CIntelligentPointer<CThing> PTarget;                                       // +0x08
    CIntelligentPointer<CThing> PSpotter;                                      // +0x10
};
class CEventHeroHasBeenSpottedStealing : public CEventBase {           // Ego_r.pdb, 0x18 bytes, vtable 0x012781E0
public:
    CEventHeroHasBeenSpottedStealing(CThing* target, CThing* spotter);        // 0x008AB8D0
    CIntelligentPointer<CThing> PTarget;                                       // +0x08
    CIntelligentPointer<CThing> PSpotter;                                      // +0x10
};
}

void CGameScriptInterface::SendEntityEvent(NEntityEvents::EEventType type, const CScriptThing& recipient, const CScriptThing& spotter,
                                           CThing* target) const
{
    if (spotter.IsAlive() && recipient.IsAlive()) {
        CTCEntityEvents* events;
        if (recipient.GetPThing()->GetTC(TCI_ENTITY_EVENTS, &events)) {
            switch (type) {
            case 19:
                events->AddEvent(new NEntityEvents::CEventHeroHasBeenSpottedPickpocketing(target, spotter.GetPThing()));
                break;
            case 20:
                events->AddEvent(new NEntityEvents::CEventHeroHasBeenSpottedPickingLock(target, spotter.GetPThing()));
                break;
            case 21:
                events->AddEvent(new NEntityEvents::CEventHeroHasBeenSpottedStealing(target, spotter.GetPThing()));
                break;
            }
        }
    }
}

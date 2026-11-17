// CGameScriptThing::MsgGetThingsKilled(std::vector<unsigned long>& groups) const -- append the creature group of every "killed creature" entity event (type 0x22) this live thing received in the current script frame interval; whether any was found
// Leaf: ?MsgGetThingsKilled@CGameScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D3DD0, length 289 bytes, `ret 4`. Ego_r.pdb
//   ?MsgGetThingsKilled@CGameScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z (the CScriptThing slot-55 override).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), the const TC map lower bound 0x004365B0 (PeekTC<const CTCEntityEvents>, 0x42),
//   the event's GetType (slot 1), vector<unsigned long>::push_back(const unsigned long&) 0x005178A0. Data: the quest manager singleton
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass 0x013B89FC (PQuestCurrentlyRunning +0x88 -> PScript +8 -> PActiveThread +0x2C ->
//   LastScriptFrame +0x18 / CurrentScriptFrame +0x1C).
// Shape: the found flag is a byte local cleared first (its zero shared with the early exits); each hit pushes the by-value
//   GetCreatureGroupOfKilledThing() result, an rvalue VC homes in the dead `groups` slot; push_back stays a call.
// NF67 (round 7): ported onto the shared headers. The CGameScriptThingMethods stand-in self, the local CQuestManager / ActiveQuest /
//   ScriptBaseNative / CSpawnedFuncNative / CTBaseSingletonNoDefaultAlloc / CThingNative / CVectorMap / CBaseIntelligentPointer /
//   CEventBase / CTCEntityEventsNative models and the local ETCInterfaceType are gone. Overlay opt-ins: FABLE_RETAIL_CGAMESCRIPTTHING_MSGGETTHINGSKILLED,
//   FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_SCRIPT (HasScript / GetScript), FABLE_RETAIL_CSCRIPTBASE_GETPACTIVETHREAD_INLINE,
//   FABLE_RETAIL_CSPAWNEDFUNCBASE_SCRIPTFRAMES_INLINE. NEntityEvents::CEventKilledCreature has no shared header: local (Ego_r.pdb 12 bytes,
//   CreatureGroupOfKilledThing +8; FableWin.pdb GetCreatureGroupOfKilledThing / StatGetType). Self now the PDB override (q10463).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CGAMESCRIPTTHING
#define FABLE_RETAIL_CGAMESCRIPTTHING_MSGGETTHINGSKILLED
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_SCRIPT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CSCRIPTBASE_GETPACTIVETHREAD_INLINE
#define FABLE_RETAIL_CSPAWNEDFUNCBASE_SCRIPTFRAMES_INLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_EVENTS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptThing.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CScriptBase.h"
#include "engine/CQuestManager.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_EEventType.h"
#include "engine/NEntityEvents_CEventExpressionPerformedTo.h"
#include "engine/GFDebugPDynamicCast.h"

namespace NEntityEvents {
// Local model (no shared header): Ego_r.pdb CEventKilledCreature (12 bytes), FableWin.pdb members.
class CEventKilledCreature : public CEventBase {
public:
    CEventKilledCreature(const CThing& killed, const CThing& killer);
    virtual ~CEventKilledCreature();
    virtual EEventType GetType() const;
    static EEventType FABLE_FASTCALL StatGetType() { return EVENT_KILLED_CREATURE; }
    unsigned long GetCreatureGroupOfKilledThing() const { return CreatureGroupOfKilledThing; }
private:
    unsigned long CreatureGroupOfKilledThing;                            // +0x08
};
}

bool CGameScriptThing::MsgGetThingsKilled(std::vector<unsigned long>& groups) const
{
    bool found = false;
    if (IS_THING_ALIVE(PThing)) {
        CQuestManager::CActiveQuest* quest = CQuestManager::Get().GetPActiveQuest();
        long last_frame = (quest && quest->HasScript()) ? quest->GetScript().GetPActiveThread()->GetLastScriptFrame() : 0;
        long current_frame = (quest && quest->HasScript()) ? quest->GetScript().GetPActiveThread()->GetCurrentScriptFrame() : 0;
        const CTCEntityEvents* events;
        if (PThing->PeekTC(&events)) {
            for (std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator it = events->GetEvents().begin();
                 it != events->GetEvents().end(); ++it) {
                NEntityEvents::CEventBase* event = (*it).Get();
                if (event->GetType() == NEntityEvents::CEventKilledCreature::StatGetType()
                    && event->WorldFrame > last_frame && event->WorldFrame <= current_frame) {
                    groups.push_back(GFDebugPDynamicCast(event, (NEntityEvents::CEventKilledCreature*)0)->GetCreatureGroupOfKilledThing());
                    found = true;
                }
            }
        }
    }
    return found;
}

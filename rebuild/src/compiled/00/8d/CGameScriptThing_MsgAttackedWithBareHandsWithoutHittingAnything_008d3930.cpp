// CGameScriptThing::MsgAttackedWithBareHandsWithoutHittingAnything() const -- true when the live thing's entity-event list holds an event of type 0x1F stamped inside the running quest thread's (last, current] script-frame window
// Leaf: ?MsgAttackedWithBareHandsWithoutHittingAnything@CGameScriptThing@@UBE_NXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base.
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D3930, 219 bytes, thiscall, plain `ret`.
// Identity (proven): CGameScriptThing vtable 0x012852CC slot 53; the manifest label is its FableWin.pdb mangled name.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound(const
//   ETCInterfaceType&) const 0x004365B0, the event's virtual GetType (slot 1).
// Shape (family K042, representative 0x008D2700): IS_THING_ALIVE(PThing); the running quest's last / current script frames
//   (null quest or script -> 0); the const GetTC<CTCEntityEvents> (interface 0x42: the bitset word & mask tested in the `if`
//   itself -- through operator[]'s bool VC7.1 emits shr/test -- and the
//   const lower bound); the event list walk with the type and frame-window test -- the landed sibling
//   MsgPerformedAnyAggressiveSpecialAbility 0x008D27E0 without its Aggressive flag, on Ego_r.pdb types. Header debt:
//   engine/CGameScriptThing.h / CTCEntityEvents.h / CQuestManager.h flatten the vptr or hold PDB-width containers.
#include <stddef.h>
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
namespace NEntityEvents {
enum EEventType { EEventType_Force32 = 0x7fffffff };
class CEventBase {                                            // Ego_r.pdb: vptr + WorldFrame (8 bytes)
public:
    virtual ~CEventBase();                                    // slot 0
    virtual EEventType GetType() const;                       // slot 1
    long WorldFrame;                                          // +0x04
};
}
class CTCEntityEvents {                                       // interface 0x42; Ego_r.pdb Events after the CTCBase
public:
    unsigned char _base[0x0c];                                // CTCBase (vptr, ParentThing, flags)
    std::list<CCountedPointer<NEntityEvents::CEventBase> > Events;   // +0x0C
};
class CSpawnedFuncBase {                                      // Ego_r.pdb (no STL member before +0x20)
public:
    unsigned char _pad00[0x18];
    long LastScriptFrame;                                     // +0x18
    long CurrentScriptFrame;                                  // +0x1C
};
class CScriptBase {                                           // Ego_r.pdb 84 bytes; retail PActiveThread +0x2C (PDB +60)
public:
    unsigned char _pad00[0x2c];
    CSpawnedFuncBase* PActiveThread;                          // +0x2C
};
class CQuestManager {                                         // Ego_r.pdb 276 bytes; retail PQuestCurrentlyRunning +0x88 (PDB +228)
public:
    class CActiveQuest {                                      // Ego_r.pdb
    public:
        long ID;                                              // +0x00
        void* PScriptInfo;                                    // +0x04 CScriptInfo*
        CCountedPointer<CScriptBase> PScript;                 // +0x08
    };
    unsigned char _pad00[0x88];
    CActiveQuest* PQuestCurrentlyRunning;                     // +0x88
};
extern CQuestManager* g_pQuestManager;                        // 0x013B89FC (reloc_bindings' ?g_pQuestManager@@3PAVCQuestManager@@A)
class CGameScriptThing {                                      // Ego_r.pdb 72 bytes
public:
    virtual ~CGameScriptThing();
    virtual bool MsgAttackedWithBareHandsWithoutHittingAnything() const;
    unsigned char _pad04[8];
    CIntelligentPointer<CThing> PThing;                       // +0x0C
};

bool CGameScriptThing::MsgAttackedWithBareHandsWithoutHittingAnything() const
{
    if (IS_THING_ALIVE(PThing)) {
        CQuestManager::CActiveQuest* quest = g_pQuestManager->PQuestCurrentlyRunning;
        long lastFrame = (quest && quest->PScript.Data) ? quest->PScript->PActiveThread->LastScriptFrame : 0;
        long currentFrame = (quest && quest->PScript.Data) ? quest->PScript->PActiveThread->CurrentScriptFrame : 0;
        const CTCEntityEvents* events;
        if (PThing.Get()->PeekTC(TCI_ENTITY_EVENTS, &events)) {
            for (std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator it = events->Events.begin();
                 it != events->Events.end(); ++it) {
                const NEntityEvents::CEventBase* event = it->Data;
                if (event->GetType() == 0x1F && event->WorldFrame > lastFrame && event->WorldFrame <= currentFrame)
                    return true;
            }
        }
    }
    return false;
}
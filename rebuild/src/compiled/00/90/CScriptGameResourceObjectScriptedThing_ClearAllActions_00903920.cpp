// CScriptGameResourceObjectScriptedThing::ClearAllActions() -- for a live (not dead) scripted creature (flag bit 3 at +0x6c), drop its queued actions and finish the current one
// Leaf: ClearAllActions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903920, length 70 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 four times (on PThing at +0x14), the creature's
// queued-action clear @ 0x00663600 (`add ecx,108h ; jmp` into the action list; labelled "_Cons_val<...>",
// propagated), CThingCreatureBase::FinishCurrentAction @ 0x00662650 (tail jump).
// Shape: nested ifs (null, dead bit +0x91, creature bit +0x6c), each test re-fetching the item; the
// creature is fetched once more into a local held in esi for the two calls, the last a tail jump.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include "engine/CScriptGameResourceObjectScriptedThing.h"

struct CThing_Retail {
    unsigned char  _pad00[0x6c];
    unsigned char  m_flags;                        // +0x6c
    unsigned char  _pad6d[0x24];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    void ClearQueuedActions();                     // 0x00663600
    void FinishCurrentAction();                    // 0x00662650
};
struct CIntelligentPointer_Retail {
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void ClearAllActions();
};

void CScriptGameResourceObjectScriptedThing_Methods::ClearAllActions()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (thing->GetPItem()->m_flags & 8) {
                CThing_Retail* creature = thing->GetPItem();
                creature->ClearQueuedActions();
                creature->FinishCurrentAction();
            }
        }
    }
}
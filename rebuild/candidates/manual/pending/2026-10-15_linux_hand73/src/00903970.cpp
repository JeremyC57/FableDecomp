// CScriptGameResourceObjectScriptedThing::ClearAllActionsIncludingLoopingAnimations() -- as ClearAllActions, then finish the current action a second time and release the creature's looping animation
// Leaf: ClearAllActionsIncludingLoopingAnimations
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903970, length 84 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 four times (on PThing at +0x14), the queued-action
// clear @ 0x00663600, CThingCreatureBase::FinishCurrentAction @ 0x00662650 twice, the looping-animation
// release @ 0x006633A0 (drops the counted pointer at +0x100/+0x104; labelled "_Dest_val<...>", propagated;
// tail jump).
// Shape: same guard chain as ClearAllActions (0x00903920); the creature local in esi for four calls.
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
    void ReleaseLoopingAnimation();                // 0x006633A0
};
struct CIntelligentPointer_Retail {
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void ClearAllActionsIncludingLoopingAnimations();
};

void CScriptGameResourceObjectScriptedThing_Methods::ClearAllActionsIncludingLoopingAnimations()
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (thing->GetPItem()->m_flags & 8) {
                CThing_Retail* creature = thing->GetPItem();
                creature->ClearQueuedActions();
                creature->FinishCurrentAction();
                creature->FinishCurrentAction();
                creature->ReleaseLoopingAnimation();
            }
        }
    }
}

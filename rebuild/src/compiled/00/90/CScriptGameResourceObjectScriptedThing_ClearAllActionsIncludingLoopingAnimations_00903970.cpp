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

class CThing_Retail { public:
    unsigned char  _pad00[0x6c];
    unsigned char  m_flags;                        // +0x6c
    unsigned char  _pad6d[0x24];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    void ClearQueuedActions();                     // 0x00663600
    void FinishCurrentAction();                    // 0x00662650
    void ReleaseLoopingAnimation();                // 0x006633A0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void ClearAllActionsIncludingLoopingAnimations();
};

void CScriptGameResourceObjectScriptedThing_Methods::ClearAllActionsIncludingLoopingAnimations()
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
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
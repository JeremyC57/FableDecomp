// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "engine/CCreatureAction_TrollAngryGroundPound.h"

class CBaseObject { public:
    unsigned char padding[0x1c8];
    unsigned char flags;
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CCreatureAction_TrollAngryGroundPound_Methods : CCreatureAction_TrollAngryGroundPound {
    void DoBase();
    void PerformAction();
};

void CCreatureAction_TrollAngryGroundPound_Methods::PerformAction()
{
    this->DoBase();
    CBaseIntelligentPointer* actionState = (CBaseIntelligentPointer*)&this->sub;
    CBaseObject* state = actionState->GetPItem();
    state->flags |= 0x40;
}

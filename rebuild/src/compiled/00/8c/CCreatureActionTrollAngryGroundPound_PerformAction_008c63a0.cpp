#include "engine/CCreatureAction_TrollAngryGroundPound.h"

class CBaseObject { public:
    unsigned char padding[0x1c8];
    unsigned char flags;
};

class CBaseIntelligentPointer { public:
public:
    CBaseObject* GetPItem() const;   // 0x00A01B50
};

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

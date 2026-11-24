#ifndef FABLE_ENGINE_CAISTATEGROUP_JACKOFBLADESSTAGE3_H
#define FABLE_ENGINE_CAISTATEGROUP_JACKOFBLADESSTAGE3_H

#include "engine/CAIStateGroup_JackOfBladesBase.h"

// Original Ego_r and FableWin typed base and virtual declarations.
class CAIStateGroup_JackOfBladesStage3 : public CAIStateGroup_JackOfBladesBase {
public:
    virtual void OnActivate();
    virtual void Update();
protected:
    virtual bool IsRunnable();
};

FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_JackOfBladesStage3) == 0x4c);
#endif

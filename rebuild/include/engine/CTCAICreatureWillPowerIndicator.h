#ifndef FABLE_ENGINE_CTCAICREATUREWILLPOWERINDICATOR_H
#define FABLE_ENGINE_CTCAICREATUREWILLPOWERINDICATOR_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;

// Ego_r record and retail vtable 0x0126FF2C.
class CTCAICreatureWillPowerIndicator : public CTCBase {
public:
    virtual void FrameUpdate();
private:
    bool PoweredUp;                                                      // +0x0c
    long Timer;                                                          // +0x10
    long ResetCount;                                                     // +0x14
    bool ShowEffect;                                                     // +0x18
    CIntelligentPointer<CThing> PEmitter;                                // +0x19
};

FABLE_STATIC_ASSERT(sizeof(CTCAICreatureWillPowerIndicator) == 0x24);
#endif

#ifndef FABLE_ENGINE_CTCAICREATUREWILLPOWERINDICATOR_H
#define FABLE_ENGINE_CTCAICREATUREWILLPOWERINDICATOR_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;

// Ego_r record and retail vtable 0x0126FF2C.
class CTCAICreatureWillPowerIndicator : public CTCBase {
public:
    virtual void FrameUpdate();
#ifdef FABLE_RETAIL_MAZE_TUTORIAL_PASS29
    void Reset(float);
#endif
#ifdef FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_AICREATURE_WILL_POWER_INDICATOR; }
#ifdef FABLE_RETAIL_MAZE_FIXTURE_PASS27
    CTCAICreatureWillPowerIndicator(CThing&);
#endif
    bool IsPoweredUp() const { return PoweredUp; }
#endif
private:
    bool PoweredUp;                                                      // +0x0c
    long Timer;                                                          // +0x10
    long ResetCount;                                                     // +0x14
    bool ShowEffect;                                                     // +0x18
    CIntelligentPointer<CThing> PEmitter;                                // +0x19
};

FABLE_STATIC_ASSERT_AT(19, sizeof(CTCAICreatureWillPowerIndicator) == 0x24);
#endif

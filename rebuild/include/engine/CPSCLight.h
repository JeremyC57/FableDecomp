#ifdef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#ifndef FABLE_ENGINE_CPSCLIGHT_H
#define FABLE_ENGINE_CPSCLIGHT_H
#include "engine/CPSCBase.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CArray.h"
namespace NParticleEngine {
// Retail opt-in (Codex pass22): original Ego_r TPI0x60784 fields; native size108 (Alloc00AF8020), donor112.
// Final CArray is12 bytes in TLC versus16 in donor; all field offsets and bitfields agree.
class CPSCLight:public CPSCBase {public:
    virtual void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
    long LightLifeTimer;
    long LightRespawnDelayTimer;
    bool LightIsAlive;
    unsigned long LightPositionParam;
    float LightLifeSecs;
    float LightRespawnDelaySecs;
    float LightStartTime;
    float LightTimelineSecs;
    float LightStartRenderWorldRadius;
    float LightEndRenderWorldRadius;
    long LightAttenuationFactor;
    long LightFadeInEndInteger;
    long LightFadeOutBeginInteger;
    float LightWorldRadiusFadeMinimum;
    CRGBColour LightColourFadeMinimum;
    bool LightWorldRadiusFadeEnable:1;
    bool LightUseLifeSecs:1;
    bool LightEnabled:1;
    bool LightRespawns:1;
    bool LightColourFadeEnable:1;
    bool LightUseStartColour:1;
    bool LightUseMidColour:1;
    bool LightUseEndColour:1;
    bool LightUseFadeColour:1;
    bool LightUseTimelineSecs:1;
    bool StartedTimer:1;
    CRGBColour LightStartColour;
    CRGBColour LightMidColour;
    CRGBColour LightEndColour;
    CEnginePrimitiveHandle PrimHandle;
    CArray<C3DVector> LightUserPointsArray;
};
}
#endif
#endif

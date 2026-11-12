// NParticleEngine::CPSCLight::DatabaseSerialise(CPersistContext& context, bool) -- persist the light settings by name: enable/life/respawn flags and times, render radii, attenuation and fade integers, colour-fade flags and colours, timeline, then the user points array size and each point
// Grade: DIFFER(1310v1310) -- ported onto fable_string.h / CPersistContext / CRGBColour / C3DVector / CArray / CEnginePrimitiveHandle / CPSCBase (GETCRC_INLINE); one register choice remains: the LightWorldRadiusFadeEnable (bit 0 of +0x48) write-back loads the bool into al before `xor cl,al`, retail folds `xor cl,[esp+30h]` (and the loop-alignment lea shifts by that byte). Tried: /Oa (worse), `= v != false` (worse)
// Leaf: DatabaseSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1A870, 1310 bytes (`ret 8`).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer(name, unsigned long&, ...) 0x004106F0,
//   Transfer(name, CCharString&, ...) 0x004109A0, Transfer(name, float&, ...) 0x00410620, Transfer(name, long&, ...)
//   0x00410BE0, Transfer(name, CRGBColour&, ...) 0x006420F0, Transfer(name, C3DVector&, ...) 0x004859AD; CCharString()
//   0x0099E4B0, CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0; CRC Calc 0x004014A0;
//   CArray<C3DVector>::resize 0x0075C040.
// Data: names "LightEnabled" .. "LightUserPointsArray" 0x012A08E8..0x012A06A8, empty string 0x0129AAF4.
// Layout: PDB (Ego_r) CPSCLight (0x70 bytes), retail offsets equal the PDB: flag bits +0x48 (WorldRadiusFadeEnable,
//   UseLifeSecs, Enabled, Respawns, ColourFadeEnable, UseStart/Mid/EndColour) and +0x49 (UseFadeColour,
//   UseTimelineSecs, StartedTimer); LightUserPointsArray is a 12-byte STLport vector at +0x60.
// Model: shared fable_string.h / engine CPersistContext, CRGBColour, C3DVector, CArray, CPSCBase, CEnginePrimitiveHandle;
//   CPSCLight has no engine header: a local class over CPSCBase with the Ego_r.pdb members (0x70 bytes).
// Shape: the landed CPSCAttractor::DatabaseSerialise (0x00B18110) idiom: each bit flag through its own bool block,
//   the position param as a CRC when loading (inline by-value empty default + if/else CRC helper), CRGBColour
//   defaults as (0,0,0,0xff) temporaries whose ctor assigns R, G, B, A, and the guarded point loop.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CPSCBase.h"
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
namespace NParticleEngine {
class CPSCLight : public CPSCBase {
public:
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
    long LightLifeTimer;                                // +0x0c
    long LightRespawnDelayTimer;                        // +0x10
    bool LightIsAlive;                                  // +0x14
    unsigned long LightPositionParam;                   // +0x18
    float LightLifeSecs;                                // +0x1c
    float LightRespawnDelaySecs;                        // +0x20
    float LightStartTime;                               // +0x24
    float LightTimelineSecs;                            // +0x28
    float LightStartRenderWorldRadius;                  // +0x2c
    float LightEndRenderWorldRadius;                    // +0x30
    long LightAttenuationFactor;                        // +0x34
    long LightFadeInEndInteger;                         // +0x38
    long LightFadeOutBeginInteger;                      // +0x3c
    float LightWorldRadiusFadeMinimum;                  // +0x40
    CRGBColour LightColourFadeMinimum;                  // +0x44
    bool LightWorldRadiusFadeEnable : 1;                // +0x48 bit 0
    bool LightUseLifeSecs : 1;                          // +0x48 bit 1
    bool LightEnabled : 1;                              // +0x48 bit 2
    bool LightRespawns : 1;                             // +0x48 bit 3
    bool LightColourFadeEnable : 1;                     // +0x48 bit 4
    bool LightUseStartColour : 1;                       // +0x48 bit 5
    bool LightUseMidColour : 1;                         // +0x48 bit 6
    bool LightUseEndColour : 1;                         // +0x48 bit 7
    bool LightUseFadeColour : 1;                        // +0x49 bit 0
    bool LightUseTimelineSecs : 1;                      // +0x49 bit 1
    bool StartedTimer : 1;                              // +0x49 bit 2
    CRGBColour LightStartColour;                        // +0x4c
    CRGBColour LightMidColour;                          // +0x50
    CRGBColour LightEndColour;                          // +0x54
    CEnginePrimitiveHandle PrimHandle;                  // +0x58
    CArray<C3DVector> LightUserPointsArray;             // +0x60
};
FABLE_STATIC_ASSERT(offsetof(CPSCLight, LightUserPointsArray) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CPSCLight, PrimHandle) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CPSCLight, LightStartColour) == 0x4c);
void CPSCLight::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    {
        bool v = LightEnabled;
        context.Transfer("LightEnabled", v, false);
        LightEnabled = v;
    }
    if (context.IsSaving()) {
        context.Transfer("LightPositionParam", LightPositionParam, 0UL);
    } else {
        CCharString name;
        context.Transfer("LightPositionParam", name, EmptyCCharString());
        LightPositionParam = name.GetCRC();
    }
    {
        bool v = LightUseLifeSecs;
        context.Transfer("LightUseLifeSecs", v, false);
        LightUseLifeSecs = v;
    }
    context.Transfer("LightLifeSecs", LightLifeSecs, 0.0f);
    {
        bool v = LightRespawns;
        context.Transfer("LightRespawns", v, false);
        LightRespawns = v;
    }
    context.Transfer("LightRespawnDelaySecs", LightRespawnDelaySecs, 0.0f);
    context.Transfer("LightStartRenderWorldRadius", LightStartRenderWorldRadius, 0.0f);
    context.Transfer("LightEndRenderWorldRadius", LightEndRenderWorldRadius, 0.0f);
    context.Transfer("LightAttenuationFactor", LightAttenuationFactor, 0L);
    context.Transfer("LightFadeInEndInteger", LightFadeInEndInteger, 0L);
    context.Transfer("LightFadeOutBeginInteger", LightFadeOutBeginInteger, 0L);
    {
        bool v = LightWorldRadiusFadeEnable;
        context.Transfer("LightWorldRadiusFadeEnable", v, false);
        LightWorldRadiusFadeEnable = v;
    }
    context.Transfer("LightWorldRadiusFadeMinimum", LightWorldRadiusFadeMinimum, 0.0f);
    {
        bool v = LightUseFadeColour;
        context.Transfer("LightUseFadeColour", v, false);
        LightUseFadeColour = v;
    }
    {
        bool v = LightColourFadeEnable;
        context.Transfer("LightColourFadeEnable", v, false);
        LightColourFadeEnable = v;
    }
    context.Transfer("LightColourFadeMinimum", LightColourFadeMinimum, CRGBColour(0, 0, 0, 0xff));
    {
        bool v = LightUseStartColour;
        context.Transfer("LightUseStartColour", v, false);
        LightUseStartColour = v;
    }
    {
        bool v = LightUseMidColour;
        context.Transfer("LightUseMidColour", v, false);
        LightUseMidColour = v;
    }
    {
        bool v = LightUseEndColour;
        context.Transfer("LightUseEndColour", v, false);
        LightUseEndColour = v;
    }
    context.Transfer("LightStartColour", LightStartColour, CRGBColour(0, 0, 0, 0xff));
    context.Transfer("LightMidColour", LightMidColour, CRGBColour(0, 0, 0, 0xff));
    context.Transfer("LightEndColour", LightEndColour, CRGBColour(0, 0, 0, 0xff));
    {
        bool v = LightUseTimelineSecs;
        context.Transfer("LightUseTimelineSecs", v, false);
        LightUseTimelineSecs = v;
    }
    context.Transfer("LightStartTime", LightStartTime, 0.0f);
    context.Transfer("LightTimelineSecs", LightTimelineSecs, 0.0f);
    long count = (long)LightUserPointsArray.size();
    context.Transfer("LightUserPointsArraySize", count, 0L);
    LightUserPointsArray.resize(count);
    long i = 0;
    if (i < count) {
        C3DVector zero(0.0f, 0.0f, 0.0f);
        do {
            context.Transfer("LightUserPointsArray" + i, LightUserPointsArray[i], zero);
        } while (++i < count);
    }
}
}

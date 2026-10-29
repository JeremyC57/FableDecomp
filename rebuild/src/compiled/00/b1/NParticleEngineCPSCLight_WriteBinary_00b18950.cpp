// NParticleEngine::CPSCLight::WriteBinary(CDataOutputStream*) -- serialize the light component: position param, timing/radius floats, attenuation and fade integers, the fade-minimum colour, the eleven flag bits and the start/mid/end colours
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18950, length 416 bytes (402 + int3 padding).
// Callees (lane names, as in the landed sibling CPSCAttractor::WriteBinary 0x00B17F70): WriteULong 0x00993EA0
//   (propagated label "WriteCBYTE"), WriteFloat 0x00993EE0, WriteLong 0x00993EB0, WriteColour(const CRGBColour&)
//   0x00993F30, WriteEBOOL 0x00993E30.
// Layout: PDB (Ego_r) CPSCLight names/types; retail offsets equal the PDB (+0x18 LightPositionParam .. +0x54
//   LightEndColour), the flag bits live in bytes +0x48 (8 bits) and +0x49 (3 bits).
// Shape: one stream call per member in declaration order; each bool bit-field is passed by value, which VC7.1
//   extracts as `xor ; mov r8 ; shr ; and r,0xffffff01` (the top bit needs no mask).
#include "engine/CDataOutputStream.h"
class CRGBColour { public: union { struct { unsigned char B, G, R, A; }; unsigned long Colour; }; };   // 4-byte aligned packed colour
namespace NParticleEngine {
class CPSCLight { public:
    unsigned char _base[0xc];                           // CPSCBase
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
    bool LightUseLifeSecs : 1;
    bool LightEnabled : 1;
    bool LightRespawns : 1;
    bool LightColourFadeEnable : 1;
    bool LightUseStartColour : 1;
    bool LightUseMidColour : 1;
    bool LightUseEndColour : 1;
    bool LightUseFadeColour : 1;                        // +0x49 bit 0
    bool LightUseTimelineSecs : 1;
    bool StartedTimer : 1;
    CRGBColour LightStartColour;                        // +0x4c
    CRGBColour LightMidColour;                          // +0x50
    CRGBColour LightEndColour;                          // +0x54
    void WriteBinary(CDataOutputStream* out);
};
void CPSCLight::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream* out = (CDataOutputStream*)destination;
    out->WriteULONG(LightPositionParam);
    out->WriteFloat(LightLifeSecs);
    out->WriteFloat(LightRespawnDelaySecs);
    out->WriteFloat(LightStartTime);
    out->WriteFloat(LightTimelineSecs);
    out->WriteFloat(LightStartRenderWorldRadius);
    out->WriteFloat(LightEndRenderWorldRadius);
    out->WriteSLONG(LightAttenuationFactor);
    out->WriteSLONG(LightFadeInEndInteger);
    out->WriteSLONG(LightFadeOutBeginInteger);
    out->WriteFloat(LightWorldRadiusFadeMinimum);
    out->WriteColour(LightColourFadeMinimum);
    out->WriteEBOOL(LightWorldRadiusFadeEnable);
    out->WriteEBOOL(LightUseLifeSecs);
    out->WriteEBOOL(LightEnabled);
    out->WriteEBOOL(LightRespawns);
    out->WriteEBOOL(LightColourFadeEnable);
    out->WriteEBOOL(LightUseStartColour);
    out->WriteEBOOL(LightUseMidColour);
    out->WriteEBOOL(LightUseEndColour);
    out->WriteEBOOL(LightUseFadeColour);
    out->WriteEBOOL(LightUseTimelineSecs);
    out->WriteEBOOL(StartedTimer);
    out->WriteColour(LightStartColour);
    out->WriteColour(LightMidColour);
    out->WriteColour(LightEndColour);
}
}
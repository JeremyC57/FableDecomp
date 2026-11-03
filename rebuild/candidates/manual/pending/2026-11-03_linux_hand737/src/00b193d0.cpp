// NParticleEngine::CPSCLight::ReadBinary(CDataInputStream& in) -- vtable slot 15: read the light component back in WriteBinary order (position param, timing / radius floats, attenuation and fade integers, the fade-minimum colour, the eleven flag bits, the start / mid / end colours)
// Leaf: ?ReadBinary@CPSCLight@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B193D0, 2119 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb ?ReadBinary@CPSCLight@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of the CPSCLight
//   vtable 0x012A0624 (rtti-vtables.tsv), as CPSCAttractor::ReadBinary 0x00B18390 / CPSCOrbit::ReadBinary 0x00B16F60 are slot 15 of
//   theirs; the mirror of the landed CPSCLight::WriteBinary 0x00B18950 (same member order). Catalog label "CPSCLight::ReadBinary" kept.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined Read's slow path, x23),
//   CDataInputStream::ReadColour 0x009945E0 (FableWin ?ReadColour@CDataInputStream@@QAE?AVCRGBColour@@XZ, hidden result, x4).
// Inlined: CDataInputStream::ReadULONG / ReadSLONG / ReadFloat / ReadEBOOL (FableWin.pdb names; the bounds-checked chunk copy of
//   engine/CDataInputStream.h FABLE_RETAIL_CDATAINPUTSTREAM, overlay hdr/ adds ReadFloat, ReadEBOOL and ReadColour).
// Layout: PDB (Ego_r) CPSCLight, retail offsets equal the PDB (the landed WriteBinary / DatabaseCopy 0x00B1A700 model):
//   LightPositionParam +0x18 .. LightColourFadeMinimum +0x44, flag bytes +0x48 (8 bits) / +0x49 (3 bits), colours +0x4C..+0x54.
//   CPSCBase (vptr, ParticleSystem, two flag bits) as the landed CPSCDecalRenderer::InterpolateTo 0x00B1BDA0 model; no engine
//   header exists for the CPSC classes (header debt, as for every landed CPSC row).
// Shape: one read per member; every ReadEBOOL result goes into its bool bit-field (`test ; setne ; xor ; and ; xor`, the top bit
//   `shl ; and 7Fh ; or`); the colour reads are stored through CRGBColour's implicit operator= (one dword move).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/CDataInputStream.h"
#include "engine/CBaseClassNonCopyable.h"
namespace NParticleEngine {
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void ReadBinary(CDataInputStream&) = 0;
};
class CPSCLight : public CPSCBase {
public:
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
    virtual void ReadBinary(CDataInputStream& in);
};
void CPSCLight::ReadBinary(CDataInputStream& in)
{
    LightPositionParam = in.ReadULONG();
    LightLifeSecs = in.ReadFloat();
    LightRespawnDelaySecs = in.ReadFloat();
    LightStartTime = in.ReadFloat();
    LightTimelineSecs = in.ReadFloat();
    LightStartRenderWorldRadius = in.ReadFloat();
    LightEndRenderWorldRadius = in.ReadFloat();
    LightAttenuationFactor = in.ReadSLONG();
    LightFadeInEndInteger = in.ReadSLONG();
    LightFadeOutBeginInteger = in.ReadSLONG();
    LightWorldRadiusFadeMinimum = in.ReadFloat();
    LightColourFadeMinimum = in.ReadColour();
    LightWorldRadiusFadeEnable = in.ReadEBOOL();
    LightUseLifeSecs = in.ReadEBOOL();
    LightEnabled = in.ReadEBOOL();
    LightRespawns = in.ReadEBOOL();
    LightColourFadeEnable = in.ReadEBOOL();
    LightUseStartColour = in.ReadEBOOL();
    LightUseMidColour = in.ReadEBOOL();
    LightUseEndColour = in.ReadEBOOL();
    LightUseFadeColour = in.ReadEBOOL();
    LightUseTimelineSecs = in.ReadEBOOL();
    StartedTimer = in.ReadEBOOL();
    LightStartColour = in.ReadColour();
    LightMidColour = in.ReadColour();
    LightEndColour = in.ReadColour();
}
}

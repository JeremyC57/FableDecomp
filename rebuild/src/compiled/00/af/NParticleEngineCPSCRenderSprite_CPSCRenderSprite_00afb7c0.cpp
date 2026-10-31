// NParticleEngine::CPSCRenderSprite::CPSCRenderSprite() -- default sprite renderer component: no sprite/trail bank, white colours, the database defaults packed into the bit-field parameters (render size 1, flicker speed 15, trail width 0.5, ...), two empty primitive handles, and the emitter manager's default alignment / blend mode / blend op
// Leaf: ??0CPSCRenderSprite@NParticleEngine@@QAE@XZ
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00AFB841 `fld dword ptr [esp+0Ch]` + 0x00AFB845 `fistp dword ptr [esp+10h]` -- the engine's inline-assembly float-to-long helper (landed FloatToLong, CPSCEmitterGeneric::GetRandomRadius 0x00B102C0), 16 sites (round to two decimals, then pack); VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFB7C0, 1038 bytes, plain `ret` (no catalog length: extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: Ego_r.pdb + FableWin.pdb ??0CPSCRenderSprite@NParticleEngine@@QAE@XZ + the manifest row; vtable 0x0129F67C.
// Callees: CPSCBase::CPSCBase() 0x00AF2620 (FableWin.pdb ??0CPSCBase@NParticleEngine@@QAE@XZ), CBaseClass() 0x0099A2D0 (the two
//   CEnginePrimitiveHandle members, vtable 0x0122F598), NParticleEngine::GetParticleEmitterManager() 0x00AEAAF0 (Ego_r.pdb
//   ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ; retail `mov eax,[0x013D2E1C]`, between
//   GetParticleEmitterDatabase 0x00AEAAE0 and GetPSCManager 0x00AEAB00 as in the Ego_r TU order; manifest "GFGetPreMainMemoryUsed"
//   is propagated) -- called five times for its inline const GetDefaultAlignmentMode / GetDefaultBlendMode / GetDefaultBlendOp
//   (FableWin.pdb, return type `const long`), whose results fold to constants.
// Layout: Ego_r.pdb CPSCRenderSprite (0x54 bytes; bit fields as GetComponentCost 0x00AF90C0; +0x38 AlphaFadeEnable / SizeFadeEnable /
//   FlickerEnable / ForceAnimationTime; +0x3c PrimHandle, +0x44 ChildPrimIndex, +0x48 TrailPrimHandle, +0x50 ChildTrailPrimIndex).
// Shape: every member, the bit fields included, is in the mem-initialiser list (the handles are built after the packed fields);
//   the blend defaults are assigned in the body. The pack is round-to-2-decimals, `(r - min) * (1 / (max - min))` as one statement,
//   then `* steps` into FloatToLong (one expression lets VC7.1 fold the two constants into one multiply). PackParam is a local
//   helper for that database macro shape (unproven spelling; fully inlined, no symbol).
// Constants: 10.0f 0x01237F0C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, 0.1f 0x0122EE58, 0.5f 0x0122F59C, 255.0f 0x01230014,
//   15.0f 0x0124E640, 0.05f 0x0125A15C, 127.0f 0x0125A160, 1/30 0x012856DC, 2047.0f 0x01288BE4, 1023.0f 0x0129EF90, 4095.0f 0x0129F668,
//   16383.0f 0x0129F66C, 1/99.9 0x0129F670 (read from the binary).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CEngineInternalPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass {   // vtable 0x0122F598
public:
    CEngineInternalPrimitiveBase* Data;              // +0x04
    CEnginePrimitiveHandle() : Data(0) { }
};

static __forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}
// the database parameter packing: round to two decimals, map [min, max] onto [0, steps]
static __forceinline unsigned long PackParam(float value, float min, float max, float steps)
{
    float normalised = (RoundToDecimals(value, 2) - min) * (1.0f / (max - min));
    return FloatToLong(normalised * steps);
}

namespace NParticleEngine {

class CParticleEmitterManager { public:
    const long GetDefaultBlendMode() const { return 2; }        // FableWin.pdb ?GetDefaultBlendMode@CParticleEmitterManager@NParticleEngine@@QBE?BJXZ (inline)
    const long GetDefaultBlendOp() const { return 0; }          // FableWin.pdb ?GetDefaultBlendOp@...@@QBE?BJXZ (inline)
    const long GetDefaultAlignmentMode() const { return 0; }    // FableWin.pdb ?GetDefaultAlignmentMode@...@@QBE?BJXZ (inline)
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();    // 0x00AEAAF0

class CPSCBase { public:
    CPSCBase();                                      // 0x00AF2620
    virtual ~CPSCBase();
    void*         ParticleSystem;               // +0x04
    bool          UpdateStarted : 1;            // +0x08
    bool          Initialised : 1;
};

class CPSCRenderSprite : public CPSCBase { public:
    long          SpriteBankIndex;              // +0x0c
    long          TrailBankIndex;               // +0x10
    CRGBColour    StartColour;                  // +0x14
    CRGBColour    MidColour;                    // +0x18
    CRGBColour    EndColour;                    // +0x1c
    unsigned long BlendMode : 3;                // +0x20
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;     // +0x24
    unsigned long TrailLengthInteger : 12;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;   // +0x28
    unsigned long NoCrossedSprites : 5;
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;         // +0x2c
    unsigned long EndRenderSize : 11;
    unsigned long FlickerBias : 8;
    unsigned long AnimationTimeSecs : 14;       // +0x30
    unsigned long SizeFadeMinimum : 7;
    unsigned long TrailWidth : 10;
    unsigned long FlickerSpeed : 12;            // +0x34
    unsigned long UseStartColour : 1;
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    unsigned char AlphaFadeEnable : 1;          // +0x38
    unsigned char SizeFadeEnable : 1;
    unsigned char FlickerEnable : 1;
    unsigned char ForceAnimationTime : 1;
    CEnginePrimitiveHandle PrimHandle;          // +0x3c
    long          ChildPrimIndex;               // +0x44
    CEnginePrimitiveHandle TrailPrimHandle;     // +0x48
    long          ChildTrailPrimIndex;          // +0x50
    CPSCRenderSprite();
};

CPSCRenderSprite::CPSCRenderSprite()
    : SpriteBankIndex(0), TrailBankIndex(0),
      StartColour(255, 255, 255, 255), MidColour(255, 255, 255, 255), EndColour(255, 255, 255, 255),
      FadeInEndInteger(0),
      FadeOutBeginInteger(1000), TrailLengthInteger(1000), FlickerMinAlphaInteger(0),
      FlickerMinSizeInteger(0), NoCrossedSprites(2), StartRenderSize(PackParam(1.0f, 0.0f, 20.0f, 2047.0f)),
      AlphaFadeMinimum(PackParam(0.0f, 0.0f, 1.0f, 127.0f)), EndRenderSize(PackParam(1.0f, 0.0f, 20.0f, 2047.0f)),
      FlickerBias(PackParam(0.0f, -1.0f, 1.0f, 255.0f)),
      AnimationTimeSecs(PackParam(1.0f, 0.1f, 100.0f, 16383.0f)), SizeFadeMinimum(PackParam(0.0f, 0.0f, 1.0f, 127.0f)),
      TrailWidth(PackParam(0.5f, 0.0f, 10.0f, 1023.0f)),
      FlickerSpeed(PackParam(15.0f, 0.0f, 30.0f, 4095.0f)), UseStartColour(1), UseMidColour(0), UseEndColour(0),
      AlphaFadeEnable(1), SizeFadeEnable(1), FlickerEnable(0), ForceAnimationTime(0),
      ChildPrimIndex(-1), ChildTrailPrimIndex(-1)
{
    Flags = GetParticleEmitterManager().GetDefaultAlignmentMode();
    BlendMode = GetParticleEmitterManager().GetDefaultBlendMode();
    BlendOp = GetParticleEmitterManager().GetDefaultBlendOp();
    TrailBlendMode = GetParticleEmitterManager().GetDefaultBlendMode();
    TrailBlendOp = GetParticleEmitterManager().GetDefaultBlendOp();
}

} // namespace NParticleEngine
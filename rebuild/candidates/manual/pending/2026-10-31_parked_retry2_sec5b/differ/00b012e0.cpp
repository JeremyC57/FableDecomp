// NParticleEngine::CPSCRenderMesh::CPSCRenderMesh() -- default mesh renderer component: no mesh/trail bank, six white colours, the database defaults packed into the bit-field parameters (render size 1 x 1 x 1, flicker speed 15, trail width 0.5, ...), two empty primitive handles, and the emitter manager's default blend mode / blend op for the mesh and the trail
// Grade: DIFFER(1296v1296) 55/334 -- same bytes except one 3-instruction register swap: StartRenderSizeZ merged with CentredOnPos/AlphaFadeEnable = 0 at +0x38 is `(v & 7FFh) | (old & FFFFE000h)` in retail (value register first, the shape VC7.1 gives when the two bools are cleared BEFORE the field is stored) but `(old & mask) | (v & 7FFh)` here (declaration order: the field, then the bools); the other 52 lines are the vptr/constant relocations
// Leaf: ??0CPSCRenderMesh@NParticleEngine@@QAE@XZ
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B01378 `fld dword ptr [esp+0Ch]` + 0x00B0137C `fistp dword ptr [esp+10h]` -- the engine's inline-assembly float-to-long helper (landed FloatToLong, CPSCEmitterGeneric::GetRandomRadius 0x00B102C0), 16 sites (round to two decimals, then pack); VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B012E0, 1296 bytes, plain `ret` (no catalog length: extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: Ego_r.pdb + FableWin.pdb ??0CPSCRenderMesh@NParticleEngine@@QAE@XZ + the manifest row; vtable 0x0129F96C.
// Callees: CPSCBase::CPSCBase() 0x00AF2620 (FableWin.pdb ??0CPSCBase@NParticleEngine@@QAE@XZ), CBaseClass() 0x0099A2D0 (the two
//   CEnginePrimitiveHandle members, vtable 0x0122F598), NParticleEngine::GetParticleEmitterManager() 0x00AEAAF0 (Ego_r.pdb
//   ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ; retail `mov eax,[0x013D2E1C]`, between
//   GetParticleEmitterDatabase 0x00AEAAE0 and GetPSCManager 0x00AEAB00 as in the Ego_r TU order; manifest "GFGetPreMainMemoryUsed"
//   is propagated) -- called four times for its inline const GetDefaultBlendMode / GetDefaultBlendOp
//   (FableWin.pdb, return type `const long`), whose results fold to constants.
// Layout: Ego_r.pdb CPSCRenderMesh (100 bytes, as CPSCRenderMesh::GetComponentCost 0x00B00DB0); RenderSizeParam (+0x48) is not initialised.
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

class CPSCRenderMesh : public CPSCBase { public:
    long          BankIndex;                    // +0x0c
    long          TrailBankIndex;               // +0x10
    CRGBColour    TrailStartColour;             // +0x14
    CRGBColour    TrailMidColour;               // +0x18
    CRGBColour    TrailEndColour;               // +0x1c
    CRGBColour    StartColour;                  // +0x20
    CRGBColour    MidColour;                    // +0x24
    CRGBColour    EndColour;                    // +0x28
    unsigned long BlendMode : 3;                // +0x2c
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;
    unsigned long TrailLengthInteger : 12;      // +0x30
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long StartRenderSizeX : 11;        // +0x34
    unsigned long StartRenderSizeY : 11;
    unsigned long StartRenderSizeZ : 11;        // +0x38
    unsigned long CentredOnPos : 1;
    unsigned long AlphaFadeEnable : 1;
    unsigned long EndRenderSizeX : 11;          // bit 13
    unsigned long EndRenderSizeY : 11;          // +0x3c
    unsigned long EndRenderSizeZ : 11;
    unsigned long SizeFadeEnable : 1;
    unsigned long FlickerEnable : 1;
    unsigned long TrailUseStartColour : 1;
    unsigned long TrailUseMidColour : 1;
    unsigned long TrailUseEndColour : 1;
    unsigned long UseRenderSizeParam : 1;
    unsigned long FlickerBias : 8;              // +0x40
    unsigned long FlickerSpeed : 12;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long UseStartColour : 1;
    unsigned long SizeFadeMinimum : 7;          // +0x44
    unsigned long UseMidColour : 1;
    unsigned long TrailWidth : 10;              // bit 8
    unsigned long UseEndColour : 1;
    unsigned long RenderSizeParam;              // +0x48
    CEnginePrimitiveHandle PrimHandle;          // +0x4c
    long          ChildPrimIndex;               // +0x54
    CEnginePrimitiveHandle TrailPrimHandle;     // +0x58
    long          ChildTrailPrimIndex;          // +0x60
    CPSCRenderMesh();
};

CPSCRenderMesh::CPSCRenderMesh()
    : BankIndex(0), TrailBankIndex(0),
      TrailStartColour(255, 255, 255, 255), TrailMidColour(255, 255, 255, 255), TrailEndColour(255, 255, 255, 255),
      StartColour(255, 255, 255, 255), MidColour(255, 255, 255, 255), EndColour(255, 255, 255, 255),
      FadeInEndInteger(0), FadeOutBeginInteger(1000),
      TrailLengthInteger(1000), FlickerMinAlphaInteger(0), FlickerMinSizeInteger(0),
      StartRenderSizeX(PackParam(1.0f, 0.0f, 20.0f, 2047.0f)), StartRenderSizeY(PackParam(1.0f, 0.0f, 20.0f, 2047.0f)),
      StartRenderSizeZ(PackParam(1.0f, 0.0f, 20.0f, 2047.0f)), CentredOnPos(0), AlphaFadeEnable(0),
      EndRenderSizeX(PackParam(1.0f, 0.0f, 20.0f, 2047.0f)),
      EndRenderSizeY(PackParam(1.0f, 0.0f, 20.0f, 2047.0f)), EndRenderSizeZ(PackParam(1.0f, 0.0f, 20.0f, 2047.0f)),
      SizeFadeEnable(0), FlickerEnable(0), TrailUseStartColour(1), TrailUseMidColour(0), TrailUseEndColour(0), UseRenderSizeParam(0),
      FlickerBias(PackParam(0.0f, -1.0f, 1.0f, 255.0f)), FlickerSpeed(PackParam(15.0f, 0.0f, 30.0f, 4095.0f)),
      AlphaFadeMinimum(PackParam(0.0f, 0.0f, 1.0f, 127.0f)), UseStartColour(1),
      SizeFadeMinimum(PackParam(0.0f, 0.0f, 1.0f, 127.0f)), UseMidColour(0), TrailWidth(PackParam(0.5f, 0.0f, 10.0f, 1023.0f)), UseEndColour(0),
      ChildPrimIndex(-1), ChildTrailPrimIndex(-1)
{
    BlendMode = GetParticleEmitterManager().GetDefaultBlendMode();
    BlendOp = GetParticleEmitterManager().GetDefaultBlendOp();
    TrailBlendMode = GetParticleEmitterManager().GetDefaultBlendMode();
    TrailBlendOp = GetParticleEmitterManager().GetDefaultBlendOp();
}

} // namespace NParticleEngine

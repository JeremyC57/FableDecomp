// NParticleEngine::CPSCRenderMesh::CPSCRenderMesh() -- default mesh renderer component: no mesh/trail bank, six white colours, the database defaults packed into the bit-field parameters (render size 1 x 1 x 1, flicker speed 15, trail width 0.5, ...), two empty primitive handles, and the emitter manager's default blend mode / blend op for the mesh and the trail
// Leaf: ??0CPSCRenderMesh@NParticleEngine@@QAE@XZ
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00B01378 `fld dword ptr [esp+0Ch]` + 0x00B0137C `fistp dword ptr [esp+10h]` (and 15 more sites) -- the engine's
//   inline-assembly GFFloatToLongNear inside the shared engine/GFPackFloat.h quantiser (round to two decimals, then pack); this row
//   carries no asm of its own. VC7.1 `(long)x` calls __ftol2.
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
//   the blend defaults are assigned in the body. Each packed float is the engine's GFPackFloat(value, bits, min, max, 2)
//   (FableWin.pdb ?GFPackFloat@@YIKMKMMK@Z: the /Od ctor 0x02F2E920 calls it with ecx = bits, edx = 2 decimals, in member order;
//   shared engine/GFPackFloat.h, inlined here). The header's FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT body (as the landed
//   CPSCRenderMesh rows 0x00B01900 / 0x00B03CA0) gives retail's register choice at the SizeFadeMinimum store 0x00B016FD; the
//   default body swaps ecx / edx there (same instructions).
// Constants: 10.0f 0x01237F0C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, 0.1f 0x0122EE58, 0.5f 0x0122F59C, 255.0f 0x01230014,
//   15.0f 0x0124E640, 0.05f 0x0125A15C, 127.0f 0x0125A160, 1/30 0x012856DC, 2047.0f 0x01288BE4, 1023.0f 0x0129EF90, 4095.0f 0x0129F668,
//   16383.0f 0x0129F66C, 1/99.9 0x0129F670 (read from the binary).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPSCBASE_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTBLEND
#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CPSCBase.h"
#include "engine/GFPackFloat.h"

namespace NParticleEngine {

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
      StartRenderSizeX(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), StartRenderSizeY(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)),
      StartRenderSizeZ(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), CentredOnPos(0), AlphaFadeEnable(0),
      EndRenderSizeX(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)),
      EndRenderSizeY(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)), EndRenderSizeZ(GFPackFloat(1.0f, 11, 0.0f, 20.0f, 2)),
      SizeFadeEnable(0), FlickerEnable(0), TrailUseStartColour(1), TrailUseMidColour(0), TrailUseEndColour(0), UseRenderSizeParam(0),
      FlickerBias(GFPackFloat(0.0f, 8, -1.0f, 1.0f, 2)), FlickerSpeed(GFPackFloat(15.0f, 12, 0.0f, 30.0f, 2)),
      AlphaFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)), UseStartColour(1),
      SizeFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)), UseMidColour(0), TrailWidth(GFPackFloat(0.5f, 10, 0.0f, 10.0f, 2)), UseEndColour(0),
      ChildPrimIndex(-1), ChildTrailPrimIndex(-1)
{
    BlendMode = GetParticleEmitterManager().GetDefaultBlendMode();
    BlendOp = GetParticleEmitterManager().GetDefaultBlendOp();
    TrailBlendMode = GetParticleEmitterManager().GetDefaultBlendMode();
    TrailBlendOp = GetParticleEmitterManager().GetDefaultBlendOp();
}

} // namespace NParticleEngine

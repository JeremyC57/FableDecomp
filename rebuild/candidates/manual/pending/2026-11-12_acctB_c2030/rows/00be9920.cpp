// CEngineWaterSettings::CEngineWaterSettings() -- default water tuning: the shore lookup mask matrices (twelve 30-degree phases and the same phases one step back), zeroed shader-variable matrices, the feature switches and scalar defaults, cleared histories / bump shifts, the environment def's bump banks and the per-phase bound widths
// Leaf: ??0CEngineWaterSettings@@QAE@XZ
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077 RTM, no flags, FABLE_HDR_OVERLAY=hdr/) -- 1886 bytes, 397 instructions.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE9920, 1886 bytes (`ret` at 0x00BEA07D, then two int3; the extent scan's 1886 is right, the
//   reservation's 2018 ran into the uncatalogued console commands WaterReflectBlendFactors 0x00BEA080 / WaterBumpmaps 0x00BEA0B0).
// Identity: Ego_r.pdb / FableWin.pdb ??0CEngineWaterSettings@@QAE@XZ -- it is the constructor (not an Initialise): it calls
//   ??0CBaseClassNonCopyable@@IAE@XZ 0x0099A2F0 first, stores the CEngineWaterSettings vtable 0x012A8114 (RTTI .?AVCEngineWaterSettings@@,
//   slot 0 the landed ??_G 0x00BEA760), returns `this` in eax, and its sole caller is the CEngineWaterRenderer constructor 0x00B71880
//   (0x00B73A49 `lea ecx,[esi+280h] ; call 0x00BE9920` on the embedded Settings).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (out of line, the base); this TU's own
//   ?CalcReflectToRefractBlendFactors@CEngineWaterSettings@@QAEXMM@Z 0x00BE94B0 (defined here; VC7.1 expands the call with the constants
//   0 / 15 and folds it to the stores -1/15 and 1.0 at 0x00BE9CF5); cos as the x87 intrinsic (fcos on folded double angles).
// Globals: g_pEngineEnvironmentDef 0x01436E24 (the landed CEngine::Initialise 0x00B26360 spelling; CEnvironmentDef members
//   WaterLakeMinimumFlowSpeed +0x12C / WaterLakeMaximumFlowSpeed +0x130 / WaterBumpMap +0xD8 / WaterBumpMap2 +0xDC, Ego_r.pdb names).
// Layout: engine/CEngineWaterSettings.h with the TYPED (C2DVector / CMatrix3x4 members) and CTOR (declarations) opt-ins.
// Shape (each item measured against retail):
//   - The mask phases: angle = k * (2pi_f / 12.0) in double (the twelve folded constants are bit-identical to retail's pool entries,
//     e.g. k = 7 -> 0x400D524FF0000000, which (7 / 12.0) * 2pi_f does not give); below half a turn 1 - cos, else cos + 3. The helper
//     returns the double expression with an implicit narrowing: a `(float)(...)` cast leaves one no-code PAREN tuple per value and moves
//     c2's 81-tuple scheduling cut ahead of the ShoreLookupMaskMatrix2 copies.
//   - ShoreLookupMaskMatrix is assigned element by element in member order (computed and stored in that order, 0x00BE992E..);
//     ShoreLookupMaskMatrix2 through CMatrix3x4::Set(12 floats): the stores through Set's `this` stay behind the last
//     ShoreLookupMaskMatrix store (0x00BE9A4C..0x00BE9AA4); element-wise stores are hoisted into the fcos chain instead.
//   - The four shader-variable matrices are CMatrix3x4::SetScale(0.0f) (FableWin.pdb, inline: the chained off-diagonal clear stored
//     E43 first, then E33 / E22 / E11).
//   - Feature switches: retail sets EnableSwell before EnableBumpMap and never initialises EnableWake (+0x14) / EnableSoftEdge (+0x15).
//   - FresnelFactor before Colour (the 2.0f constant then lives across the Colour copy and takes ebx, 0x00BE9C0C); the plain
//     defaults in declaration order (so 0.1f / 0.05f / 3.0f / 2.0f take ecx / edx / edi / ebx), then the five values UpdateConstants
//     overwrites from the environment theme (SwellOscilationSpeed, SwellFactor, ShoreSwellFactor, FlowSpeedFactor, RefOffset) and
//     the blend factors.
//   - Histories: one two-pass loop (VC7.1 unrolls it: all [0] then all [1]); the bump-map PC frame offsets as chained `[0] = [1] = 0`
//     ([1] stored first); the bump shifts as C2DVector(0, 0) temporaries (each temporary's two dead frame stores survive, 18 pairs),
//     water type 0's histories assigned [1] before [0] (retail's store order 0x00BE9DC7..0x00BE9DF9).
//   - The bounds: a flat float[12] copy of ShoreLookupMaskMatrix in pre-transposed (column) order straight from the members
//     (a CPreTransposedBoneMatrix(const CMatrix3x4&) reference parameter makes the BumpMap2BankIndex store a barrier, 0x00BE9EAD),
//     each entry minus all earlier ones (VC7.1 unrolls the inner loop by four), and the copy back through CMatrix3x4::Set (explicit
//     stores chain on the possibly-aliased array and keep `pop edi` behind the 0x218 store, 0x00BEA062).
#define FABLE_RETAIL_CENGINEWATERSETTINGS_TYPED
#define FABLE_RETAIL_CENGINEWATERSETTINGS_CTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CMATRIX3X4_SETSCALE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#include "fable_string.h"
#include "engine/CEngineWaterSettings.h"
#include "engine/CEnvironmentDef.h"
#include <math.h>
#pragma intrinsic(cos)

extern const CEnvironmentDef* g_pEngineEnvironmentDef;        // 0x01436E24

// Descriptive name (no original found in the Ego_r / FableWin name sets, strings or headers): the shore lookup mask at phase step k of
// twelve (30 degrees apart) -- rises 0..2 over the first half turn (1 - cos), 2..4 over the second (cos + 3). File-local, always expanded.
static inline float ShoreLookupMask(long step)
{
    double angle = step * (6.2831855f / 12.0);
    if (step < 6)
        return 1.0 - cos(angle);
    return cos(angle) + 3.0;
}

void CEngineWaterSettings::CalcReflectToRefractBlendFactors(float start, float end)
{
    float scale = 1.0f / (end - start);
    ReflectToRefractBlendMult = -1.0f * scale;
    ReflectToRefractBlendAdd = 1.0f - start * scale;
}

CEngineWaterSettings::CEngineWaterSettings()
{
    ShoreLookupMaskMatrix.E11 = ShoreLookupMask(0);
    ShoreLookupMaskMatrix.E12 = ShoreLookupMask(4);
    ShoreLookupMaskMatrix.E13 = ShoreLookupMask(8);
    ShoreLookupMaskMatrix.E21 = ShoreLookupMask(1);
    ShoreLookupMaskMatrix.E22 = ShoreLookupMask(5);
    ShoreLookupMaskMatrix.E23 = ShoreLookupMask(9);
    ShoreLookupMaskMatrix.E31 = ShoreLookupMask(2);
    ShoreLookupMaskMatrix.E32 = ShoreLookupMask(6);
    ShoreLookupMaskMatrix.E33 = ShoreLookupMask(10);
    ShoreLookupMaskMatrix.E41 = ShoreLookupMask(3);
    ShoreLookupMaskMatrix.E42 = ShoreLookupMask(7);
    ShoreLookupMaskMatrix.E43 = ShoreLookupMask(11);
    ShoreLookupMaskMatrix2.Set(ShoreLookupMask(0), ShoreLookupMask(3), ShoreLookupMask(7),
                               ShoreLookupMask(0), ShoreLookupMask(4), ShoreLookupMask(8),
                               ShoreLookupMask(1), ShoreLookupMask(5), ShoreLookupMask(9),
                               ShoreLookupMask(2), ShoreLookupMask(6), ShoreLookupMask(10));
    OtherShaderVars.SetScale(0.0f);
    OtherSeaShaderVars.SetScale(0.0f);
    OtherBackgroundShaderVars.SetScale(0.0f);
    OtherSeaBackgroundShaderVars.SetScale(0.0f);

    EnableSea = true;
    EnableSeaForeground = true;
    EnableSeaMidground = true;
    EnableSeaBackground = true;
    EnableForeground = true;
    EnableBackground = true;
    EnableIceForeground = true;
    EnableIceBackground = true;
    EnableSwell = true;                 // retail: +0x0E before +0x0D (as the console command EnableWaterWaves 0x00BE9490)
    EnableBumpMap = true;
    EnableReflectionGeneration = true;
    EnableDrawReflection = false;
    EnableDrawBumpmap = false;
    EnableSpecular = false;
    EnableFresnel = false;
    EnableWaterOcclusion = false;
    ForceWaterSettings = false;
    FresnelFactor = 2.0f;
    Colour = CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f);
    BumpmapSpan = 0.15f;
    SeaBumpmapSpan = 0.1f;
    BumpMapSpeed0 = 0.1f;
    BumpMapSpeed1 = 0.05f;
    WaterBodySpanFactor = 0.05f;
    WaterBodySpanOffset = 0.0f;
    BumpMatFactor = 0.1f;
    SoftEdgeMultFactor = 3.0f;
    SoftEdgeAddFactor = 0.0f;
    SoftEdgeSpanFactor = 0.5f;
    EdgeWakeMultFactor = 3.0f;
    EdgeWakeAddFactor = 0.0f;
    EdgeWakeSpanFactorU = 0.05f;
    EdgeWakeSpanFactorV = -0.05f;
    EdgeWakeDepthClamp = 2.0f;
    EdgeWakeSpeed = 0.005f;
    ColourToReflectionFactor = 0.7f;
    IceBumpMapPCFrameOffsetSpeed = 0.1f;
    WaterBumpMapPCFrameOffsetSpeed = 0.1f;
    SeaBumpMapPCFrameOffsetSpeed = 0.1f;
    LakeMinimumFlowSpeed = g_pEngineEnvironmentDef->WaterLakeMinimumFlowSpeed;
    LakeMaximumFlowSpeed = g_pEngineEnvironmentDef->WaterLakeMaximumFlowSpeed;
    SwellOscilationSpeed = 0.05f;
    SwellFactor = 0.0625f;
    ShoreSwellFactor = 1.0f / 24.0f;
    FlowSpeedFactor = 0.25f;
    RefOffset = 0.01f;
    CalcReflectToRefractBlendFactors(0.0f, 15.0f);

    SinShift = 0.0f;
    CosShift = 0.0f;
    ShoreSinShift = 0.0f;
    EdgeWakeOffset = 0.0f;
    IceBumpMapPCFrameIndex = 0;
    WaterBumpMapPCFrameIndex = 0;
    SeaBumpMapPCFrameIndex = 0;
    for (long i = 0; i < 2; i++) {
        SinShiftHistory[i] = 0.0f;
        CosShiftHistory[i] = 0.0f;
        ShoreSinShiftHistory[i] = 0.0f;
        SwellTrigIncrimentHistory[i] = 0.0f;
        EdgeWakeOffsetHistory[i] = 0.0f;
    }
    IceBumpMapPCFrameOffset[0] = IceBumpMapPCFrameOffset[1] = 0.0f;
    WaterBumpMapPCFrameOffset[0] = WaterBumpMapPCFrameOffset[1] = 0.0f;
    SeaBumpMapPCFrameOffset[0] = SeaBumpMapPCFrameOffset[1] = 0.0f;
    BumpmapGenerationShift[0][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShift[0][1] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[0][0][1] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[0][0][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[0][1][1] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[0][1][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShift[1][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShift[1][1] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[1][0][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[1][0][1] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[1][1][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[1][1][1] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShift[2][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShift[2][1] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[2][0][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[2][0][1] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[2][1][0] = C2DVector(0.0f, 0.0f);
    BumpmapGenerationShiftHistory[2][1][1] = C2DVector(0.0f, 0.0f);
    BumpMap1BankIndex = g_pEngineEnvironmentDef->WaterBumpMap;
    BumpMap2BankIndex = g_pEngineEnvironmentDef->WaterBumpMap2;

    float bounds[12];
    bounds[0] = ShoreLookupMaskMatrix.E11;
    bounds[1] = ShoreLookupMaskMatrix.E21;
    bounds[2] = ShoreLookupMaskMatrix.E31;
    bounds[3] = ShoreLookupMaskMatrix.E41;
    bounds[4] = ShoreLookupMaskMatrix.E12;
    bounds[5] = ShoreLookupMaskMatrix.E22;
    bounds[6] = ShoreLookupMaskMatrix.E32;
    bounds[7] = ShoreLookupMaskMatrix.E42;
    bounds[8] = ShoreLookupMaskMatrix.E13;
    bounds[9] = ShoreLookupMaskMatrix.E23;
    bounds[10] = ShoreLookupMaskMatrix.E33;
    bounds[11] = ShoreLookupMaskMatrix.E43;
    for (long i = 0; i < 12; i++)
        for (long j = i - 1; j >= 0; j--)
            bounds[i] -= bounds[j];
    ShoreLookupMaskMatrixForBounds.Set(bounds[0], bounds[4], bounds[8],
                                       bounds[1], bounds[5], bounds[9],
                                       bounds[2], bounds[6], bounds[10],
                                       bounds[3], bounds[7], bounds[11]);
}

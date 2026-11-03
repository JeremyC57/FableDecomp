#ifndef FABLE_ENGINE_CENGINEWATERSETTINGS_H
#define FABLE_ENGINE_CENGINEWATERSETTINGS_H
// Hand-written (chunk 723): CEngineWaterSettings, the water renderer's tuning block (CEngineWaterRenderer::Settings, retail +0x280).
// RTTI .?AVCEngineWaterSettings@@ : CBaseClassNonCopyable : CBaseClass (vtable 0x012A8114, slot 0 0x00BEA760). Members: Ego_r.pdb
// (ghidra_out/struct_layouts_egor.tsv, 0x2e4 bytes; no containers, so retail = Ego_r). Retail evidence: the landed water rows read
// EnableSwell +0x0e, EnableFresnel +0x13, FresnelFactor +0x18, Colour +0x28, BumpMatFactor +0x5c, ShoreLookupMaskMatrix +0x194 ..
// OtherSeaShaderVars +0x254 at CEngineWaterRenderer +0x280 + offset (e.g. 0x00B77E20), and WorldFrameUpdate 0x00BE95F0 updates the
// histories at +0xc0.. / +0xd0... Composite members (CRGBFloatColour, C2DVector arrays, CMatrix3x4: no shared headers yet) are opaque
// bytes with their type in the comment. Functions: Ego_r.pdb / FableWin.pdb publics.
#ifndef FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#endif
#ifndef FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CMatrix3x4.h"
class CEngineComponentRenderParams;
class CGenericVar;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class vector;
}
class C2DVector;
class CEngineWaterSettings : public CBaseClassNonCopyable {
public:
    void WorldFrameUpdate(bool force);                    // 0x00BE95F0 ?WorldFrameUpdate@CEngineWaterSettings@@QAEX_N@Z
    void UpdateConstants(const CEngineComponentRenderParams& params);   // ?UpdateConstants@CEngineWaterSettings@@QAEXABVCEngineComponentRenderParams@@@Z
    void CalcReflectToRefractBlendFactors(float a, float b);           // ?CalcReflectToRefractBlendFactors@CEngineWaterSettings@@QAEXMM@Z
    void KeepHistoryInRange(long* history, long value) const;          // ?KeepHistoryInRange@CEngineWaterSettings@@QBEXPAJJ@Z
    void KeepHistoryInRange(float* history, float value) const;        // ?KeepHistoryInRange@CEngineWaterSettings@@QBEXPAMM@Z
    void KeepHistoryInRange(C2DVector* history, float value) const;    // ?KeepHistoryInRange@CEngineWaterSettings@@QBEXPAVC2DVector@@M@Z
    void AddToConsole();                                  // ?AddToConsole@CEngineWaterSettings@@QAEXXZ
    static void __fastcall DisableWaterWaves(CEngineWaterSettings& settings, const std::vector<CGenericVar, std::allocator<CGenericVar> >& args);
    static void __fastcall EnableWaterWaves(CEngineWaterSettings& settings, const std::vector<CGenericVar, std::allocator<CGenericVar> >& args);
    static void __fastcall WaterBumpmaps(CEngineWaterSettings& settings, const std::vector<CGenericVar, std::allocator<CGenericVar> >& args);
    static void __fastcall WaterReflectBlendFactors(CEngineWaterSettings& settings, const std::vector<CGenericVar, std::allocator<CGenericVar> >& args);
    bool ForceWaterSettings;                                  // +0x004
    bool EnableSea;                                           // +0x005
    bool EnableSeaForeground;                                 // +0x006
    bool EnableSeaMidground;                                  // +0x007
    bool EnableSeaBackground;                                 // +0x008
    bool EnableForeground;                                    // +0x009
    bool EnableBackground;                                    // +0x00a
    bool EnableIceForeground;                                 // +0x00b
    bool EnableIceBackground;                                 // +0x00c
    bool EnableBumpMap;                                       // +0x00d
    bool EnableSwell;                                         // +0x00e
    bool EnableReflectionGeneration;                          // +0x00f
    bool EnableDrawReflection;                                // +0x010
    bool EnableDrawBumpmap;                                   // +0x011
    bool EnableSpecular;                                      // +0x012
    bool EnableFresnel;                                       // +0x013
    bool EnableWake;                                          // +0x014
    bool EnableSoftEdge;                                      // +0x015
    bool EnableWaterOcclusion;                                // +0x016
    float FresnelFactor;                                      // +0x018
    float SwellFactor;                                        // +0x01c
    float ShoreSwellFactor;                                   // +0x020
    float SwellOscilationSpeed;                               // +0x024
    CRGBFloatColour Colour;                                   // +0x028
    float BumpmapSpan;                                        // +0x038
    float SeaBumpmapSpan;                                     // +0x03c
    float BumpMapSpeed0;                                      // +0x040
    float BumpMapSpeed1;                                      // +0x044
    float LakeMinimumFlowSpeed;                               // +0x048
    float LakeMaximumFlowSpeed;                               // +0x04c
    float FlowSpeedFactor;                                    // +0x050
    float WaterBodySpanFactor;                                // +0x054
    float WaterBodySpanOffset;                                // +0x058
    float BumpMatFactor;                                      // +0x05c
    float SoftEdgeMultFactor;                                 // +0x060
    float SoftEdgeAddFactor;                                  // +0x064
    float SoftEdgeSpanFactor;                                 // +0x068
    float EdgeWakeMultFactor;                                 // +0x06c
    float EdgeWakeAddFactor;                                  // +0x070
    float EdgeWakeSpanFactorU;                                // +0x074
    float EdgeWakeSpanFactorV;                                // +0x078
    float EdgeWakeDepthClamp;                                 // +0x07c
    float EdgeWakeSpeed;                                      // +0x080
    float ColourToReflectionFactor;                           // +0x084
    float RefOffset;                                          // +0x088
    float IceBumpMapPCFrameOffsetSpeed;                       // +0x08c
    float WaterBumpMapPCFrameOffsetSpeed;                     // +0x090
    float SeaBumpMapPCFrameOffsetSpeed;                       // +0x094
    float SinShift;                                           // +0x098
    float CosShift;                                           // +0x09c
    float ShoreSinShift;                                      // +0x0a0
    float EdgeWakeOffset;                                     // +0x0a4
    float SinShiftHistory[2];                                 // +0x0a8
    float CosShiftHistory[2];                                 // +0x0b0
    float ShoreSinShiftHistory[2];                            // +0x0b8
    float SwellTrigIncrimentHistory[2];                       // +0x0c0
    float EdgeWakeOffsetHistory[2];                           // +0x0c8
    float IceBumpMapPCFrameOffset[2];                         // +0x0d0
    float WaterBumpMapPCFrameOffset[2];                       // +0x0d8
    float SeaBumpMapPCFrameOffset[2];                         // +0x0e0
    unsigned char BumpmapGenerationShift[0x30];               // +0x0e8 C2DVector[3][2]
    unsigned char BumpmapGenerationShiftHistory[0x60];        // +0x118 C2DVector[3][2][2]
    float ReflectToRefractBlendMult;                          // +0x178
    float ReflectToRefractBlendAdd;                           // +0x17c
    long BumpMap1BankIndex;                                   // +0x180
    long BumpMap2BankIndex;                                   // +0x184
    long IceBumpMapPCFrameIndex;                              // +0x188
    long WaterBumpMapPCFrameIndex;                            // +0x18c
    long SeaBumpMapPCFrameIndex;                              // +0x190
    CMatrix3x4 ShoreLookupMaskMatrix;                         // +0x194
    CMatrix3x4 ShoreLookupMaskMatrix2;                        // +0x1c4
    CMatrix3x4 ShoreLookupMaskMatrixForBounds;                // +0x1f4
    CMatrix3x4 OtherShaderVars;                               // +0x224
    CMatrix3x4 OtherSeaShaderVars;                            // +0x254
    CMatrix3x4 OtherBackgroundShaderVars;                     // +0x284
    CMatrix3x4 OtherSeaBackgroundShaderVars;                  // +0x2b4
};
FABLE_STATIC_ASSERT(sizeof(CEngineWaterSettings) == 0x2e4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ForceWaterSettings) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableSea) == 0x5);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableSeaForeground) == 0x6);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableSeaMidground) == 0x7);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableSeaBackground) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableForeground) == 0x9);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableBackground) == 0xa);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableIceForeground) == 0xb);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableIceBackground) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableBumpMap) == 0xd);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableSwell) == 0xe);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableReflectionGeneration) == 0xf);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableDrawReflection) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableDrawBumpmap) == 0x11);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableSpecular) == 0x12);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableFresnel) == 0x13);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableWake) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableSoftEdge) == 0x15);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EnableWaterOcclusion) == 0x16);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, FresnelFactor) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SwellFactor) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ShoreSwellFactor) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SwellOscilationSpeed) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, Colour) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, BumpmapSpan) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SeaBumpmapSpan) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, BumpMapSpeed0) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, BumpMapSpeed1) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, LakeMinimumFlowSpeed) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, LakeMaximumFlowSpeed) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, FlowSpeedFactor) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, WaterBodySpanFactor) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, WaterBodySpanOffset) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, BumpMatFactor) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SoftEdgeMultFactor) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SoftEdgeAddFactor) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SoftEdgeSpanFactor) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EdgeWakeMultFactor) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EdgeWakeAddFactor) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EdgeWakeSpanFactorU) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EdgeWakeSpanFactorV) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EdgeWakeDepthClamp) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EdgeWakeSpeed) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ColourToReflectionFactor) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, RefOffset) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, IceBumpMapPCFrameOffsetSpeed) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, WaterBumpMapPCFrameOffsetSpeed) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SeaBumpMapPCFrameOffsetSpeed) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SinShift) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, CosShift) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ShoreSinShift) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EdgeWakeOffset) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SinShiftHistory) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, CosShiftHistory) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ShoreSinShiftHistory) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SwellTrigIncrimentHistory) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, EdgeWakeOffsetHistory) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, IceBumpMapPCFrameOffset) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, WaterBumpMapPCFrameOffset) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SeaBumpMapPCFrameOffset) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, BumpmapGenerationShift) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, BumpmapGenerationShiftHistory) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ReflectToRefractBlendMult) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ReflectToRefractBlendAdd) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, BumpMap1BankIndex) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, BumpMap2BankIndex) == 0x184);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, IceBumpMapPCFrameIndex) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, WaterBumpMapPCFrameIndex) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, SeaBumpMapPCFrameIndex) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ShoreLookupMaskMatrix) == 0x194);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ShoreLookupMaskMatrix2) == 0x1c4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, ShoreLookupMaskMatrixForBounds) == 0x1f4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, OtherShaderVars) == 0x224);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, OtherSeaShaderVars) == 0x254);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, OtherBackgroundShaderVars) == 0x284);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterSettings, OtherSeaBackgroundShaderVars) == 0x2b4);

#endif // FABLE_ENGINE_CENGINEWATERSETTINGS_H

#ifndef FABLE_ENGINE_CENVIRONMENTTHEMEDEF_H
#define FABLE_ENGINE_CENVIRONMENTTHEMEDEF_H
// Retail model (chunk 1018, row 0x00476E3E): RTTI .?AVCEnvironmentThemeDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x01236244 (rebuild/manifest/rtti-vtables.tsv: slot 0 scalar deleting dtor 0x00473496, 18 Transfer 0x004732e0, 19 Copy 0x004734b2, 20 GetSizeofClass 0x004732da).
// Members: Ego_r.pdb CEnvironmentThemeDef (200 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase;
//   sizeof 0xC4 = the element stride of TransferVectorOfSubComponents<CEnvironmentThemeDef> 0x00476E3E (`idiv` by 0xC4, `add reg,esi` with esi=0xC4)
//   and RECONCILE.tsv GetSizeofClass 196.
// Implicit constructor and destructor: retail builds the default element inline (CParentDefClassBase() 0x00430370, then the
//   0x01236244 vtable store; 0x00476EFB..0x00476F0E) and destroys it with ~CParentDefClassBase 0x00430300 (0x00476F1D).
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CEnvironmentThemeDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CEnvironmentThemeDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CEnvironmentThemeDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CEnvironmentThemeDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CEnvironmentThemeDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    float TimeOfDay;                                         // +0x3C
    bool  MoonLit;                                           // +0x40
    float FogStartZ;                                         // +0x44
    float FogEndZ;                                           // +0x48
    long  SkyTexture0;                                       // +0x4C
    long  SkyTexture1;                                       // +0x50
    float SkyTexture1Blend;                                  // +0x54
    long  CloudLowerLayerTexture0;                           // +0x58
    long  CloudLowerLayerTexture1;                           // +0x5C
    float CloudLowerLayerTexture1Blend;                      // +0x60
    float CloudLowerLayerSpeedMultiplier;                    // +0x64
    long  CloudUpperLayerTexture0;                           // +0x68
    long  CloudUpperLayerTexture1;                           // +0x6C
    float CloudUpperLayerTexture1Blend;                      // +0x70
    float CloudUpperLayerSpeedMultiplier;                    // +0x74
    float WaterColourToReflectionBlend;                      // +0x78
    float WaterAlphaFactor;                                  // +0x7C
    float WaterSpecularHightlightFactor;                     // +0x80
    float RainStrength;                                      // +0x84
    float SnowStrength;                                      // +0x88
    float MistMaxAlpha;                                      // +0x8C
    float LightningFrequency;                                // +0x90
    float GlowThresholdScale;                                // +0x94
    float GlowBloomScale;                                    // +0x98
    float GlowMotionBlur;                                    // +0x9C
    float ShadowFactor;                                      // +0xA0
    float FadedShadowFactor;                                 // +0xA4
    float WaterRefractionBlendStart;                         // +0xA8
    float WaterRefractionBlendEnd;                           // +0xAC
    float WaterFlowSpeedFactor;                              // +0xB0
    float WaterOscilationSpeed;                              // +0xB4
    float WaterSwellFactor;                                  // +0xB8
    float WaterShoreSwellFactor;                             // +0xBC
    float WaterReflectionOffset;                             // +0xC0
};
FABLE_STATIC_ASSERT(sizeof(CEnvironmentThemeDef) == 0xC4);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CENVIRONMENTTHEMEDEF_H

#ifndef FABLE_ENGINE_CENVIRONMENTDEF_H
#define FABLE_ENGINE_CENVIRONMENTDEF_H
// New header (chunk 731, 2026-11-03; one model, no class macro): the def class CEnvironmentDef : CDefClassBase (Ego_r.pdb / FableWin.pdb
// ??0/??_E/Alloc/Copy/GetSizeofClass spellings). sizeof 0x134: retail ?GetSizeofClass@CEnvironmentDef@@UAEKXZ 0x004558D9 returns it
// (`mov eax,0x134 ; ret`); scalar deleting destructor 0x00455C1D, Copy 0x0045BE26, constructor 0x004558BF. CEngine::Initialise 0x00B26360 holds it through
// CDefPointer<const CEnvironmentDef> and publishes the raw pointer. Own members are not modelled yet (one opaque block after the packed
// 0x25-byte CDefClassBase; the quarantined generated model engine/_quarantine/CEnvironmentDef.h lists the Ego_r names).
#ifdef FABLE_RETAIL_CENVIRONMENTDEF_PARENT
// Retail opt-in (agentNF4, 2026-11-10, row 0x004558BF the constructor): RTTI .?AVCEnvironmentDef@@ : CParentDefClassBase (ghidra_out/
// rtti_bases.tsv; the default text below derives CDefClassBase, which is header debt), vtable 0x012335A4. Members: Ego_r.pdb
// CEnvironmentDef in order and spelling, at Ego_r - 4 (the retail CParentDefClassBase is 0x39 bytes packed; the first member is at
// +0x3C), sizeof 0x134 as GetSizeofClass 0x004558D9 returns. The constructor 0x004558BF runs CParentDefClassBase() 0x00430370,
// stores the vtable and calls CCharString() 0x0099E4B0 on ColourLookupTexture (+0x3C); the scalars stay uninitialised.
// Spellings: Ego_r.pdb / FableWin.pdb ??0CEnvironmentDef@@QAE@XZ, ?Alloc@, ?Transfer@, ?Copy@, ?GetSizeofClass@ (declarations only).
// The destructor is the implicit one: retail's deleting destructors expand it (0x00455C64 for CEnvironmentThemeDaySetDef:
// the member destructor, then ~CParentDefClassBase 0x00430300, no vtable store).
// Needs FABLE_RETAIL_DEF_CLASS_BASE and fable_string.h (CCharString) first.
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CEnvironmentDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CEnvironmentDef();                                                  // 0x004558BF
    virtual void Transfer(CPersistContext&);                            // slot 18
    virtual void Copy(const CDefClassBase*);                            // slot 19
    virtual unsigned long GetSizeofClass();                             // slot 20 0x004558D9
    CCharString ColourLookupTexture;                                    // +0x3C
    long DiffuseLookupRow;                                              // +0x40
    long AmbientLookupRow;                                              // +0x44
    long BacklightLookupRow;                                            // +0x48
    long ReflectionLookupRow;                                           // +0x4C
    long MistEffectColourLookupRow;                                     // +0x50
    long FogColourLookupRow;                                            // +0x54
    long FogAlphaLookupRow;                                             // +0x58
    long SunColourLookupRow;                                            // +0x5C
    long CloudColourLookupRow;                                          // +0x60
    long MoonColourLookupRow;                                           // +0x64
    long StarsColourLookupRow;                                          // +0x68
    long SunFlareColourLookupRow;                                       // +0x6C
    long LensFlareColourLookupRow;                                      // +0x70
    long SkyGradientTopLookupRow;                                       // +0x74
    long SkyGradientTopAlphaLookupRow;                                  // +0x78
    long SkyGradientBottomLookupRow;                                    // +0x7C
    long SkyGradientBottomAlphaLookupRow;                               // +0x80
    long WaterColourLookupRow;                                          // +0x84
    long SeaColourLookupRow;                                            // +0x88
    long GlowThresholdColourLookupRow;                                  // +0x8C
    long GlowBloomColourLookupRow;                                      // +0x90
    long SunlightAttenuatorColourLookupRow;                             // +0x94
    float DiffuseClampAngle;                                            // +0x98
    float SunlightAttenuatorAngleFadeStart;                             // +0x9C
    float SunlightAttenuatorAngleFadeEnd;                               // +0xA0
    long SeaTexture;                                                    // +0xA4
    long SeaRadius;                                                     // +0xA8
    long SeaFlatSectionStart;                                           // +0xAC
    long SeaFlatSectionEnd;                                             // +0xB0
    float DayStartTime;                                                 // +0xB4
    float DaySpeed;                                                     // +0xB8
    long WaterBumpMapPC;                                                // +0xBC
    long SeaBumpMapPC;                                                  // +0xC0
    long IceBumpMapPC;                                                  // +0xC4
    long IceBumpMap;                                                    // +0xC8
    long IceTexture;                                                    // +0xCC
    long WaterEdgeAlphaMap;                                             // +0xD0
    long WaterSurfMap;                                                  // +0xD4
    long WaterBumpMap;                                                  // +0xD8
    long WaterBumpMap2;                                                 // +0xDC
    long SeaBumpMap;                                                    // +0xE0
    long SeaBumpMap2;                                                   // +0xE4
    long WaterEnvMapOverlayTexture;                                     // +0xE8
    long MistAlphaGraphic;                                              // +0xEC
    long MistAlphaGraphicPC;                                            // +0xF0
    long RainTexture;                                                   // +0xF4
    long RainTexturePC;                                                 // +0xF8
    long SnowTexture;                                                   // +0xFC
    long RainSplashParticle;                                            // +0x100
    float LightningFadeInDuration;                                      // +0x104
    float LightningFadeOutDuration;                                     // +0x108
    float LightningFlashDuration;                                       // +0x10C
    float LightningRainThreshold;                                       // +0x110
    long LightningTheme;                                                // +0x114
    float CloudSpeedMultiplier;                                         // +0x118
    float CloudMaxSpeed;                                                // +0x11C
    float CloudTextureCoordMultiplier;                                  // +0x120
    float CloudTextureCoordOffset;                                      // +0x124
    float CloudHeightOffset;                                            // +0x128
    float WaterLakeMinimumFlowSpeed;                                    // +0x12C
    float WaterLakeMaximumFlowSpeed;                                    // +0x130
};
FABLE_STATIC_ASSERT(sizeof(CEnvironmentDef) == 0x134);
#else
#include "rebuild_abi.h"
#include "engine/CDefClassBase.h"
class CEnvironmentDef : public CDefClassBase {
private:
    unsigned char _pad_0x25[0x10f];                                       // +0x25 members not modelled yet
};
FABLE_STATIC_ASSERT(sizeof(CEnvironmentDef) == 0x134);
#endif // FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#endif // FABLE_ENGINE_CENVIRONMENTDEF_H

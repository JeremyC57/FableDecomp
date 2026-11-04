#ifndef FABLE_ENGINE_CENGINELIGHTINGMANAGER_H
#define FABLE_ENGINE_CENGINELIGHTINGMANAGER_H
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER
// Retail opt-in (chunk 723b): CEngineLightingManager : CEnginePrimitiveManager (+0) , CEngineComponent (+0xC) (RTTI
// .?AVCEngineLightingManager@@); the engine's instance is 0x01436E9C (settled spelling g_pEngineLightingManager, NAME_FIXES_QUEUE c722). Own members from +0x18 with their
// Ego_r.pdb names and offsets up to the primitive fog colour (the first container, ThemeChannels +0x3C, is a 12-byte STLport
// CArray in both: Ego_r's 0x10 vector would put PrimitiveDiffuseColour at +0x4C, retail reads the theme channels' first element
// through +0x3C and the default header's +0x48 follows the retail size); the rest is opaque to sizeof 0x46D0. Nested
// CThemeChannel (Ego_r.pdb struct, 0x70 bytes): the five colours, fog range, shadow factors and fog transform.
#ifndef FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#endif
#ifndef FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4
#endif
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineComponent.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
class C3DBoundingBox;
class CEngineLocalLightArray;
class CEngineLightingManager : public CEnginePrimitiveManager, public CEngineComponent {
public:
    // Chunk 731: the constructor (CEngine::Initialise 0x00B26360 news 0x46D0 bytes) and the overrides of the pure CEnginePrimitiveManager
    // slots and CEngineComponent::PeekSceneFilterFlags (Ego_r.pdb / FableWin.pdb spellings).
    CEngineLightingManager();                                            // 0x00B482A0 ??0CEngineLightingManager@@QAE@XZ
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);   // ?CreateInternalPrimitive@CEngineLightingManager@@UAEPAVCEngineInternalPrimitiveBase@@W4EEnginePrimitiveType@@@Z
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;                      // ?GetSupportedTypes@CEngineLightingManager@@UBEXAAVCEnginePrimitiveTypeList@@@Z
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);   // ?RenderPrimitive@CEngineLightingManager@@UAE?AW4ERendered@@...@Z
    virtual unsigned long PeekSceneFilterFlags() const;                  // ?PeekSceneFilterFlags@CEngineLightingManager@@UBEKXZ
    // Ego_r.pdb / FableWin.pdb publics, out of line in retail (the ice pass 0x00B751F0 calls them):
    CEngineLocalLightArray* GetLightsInArea(const C3DBoundingBox& area, long maxLights,
                                            const CEnginePrimitiveThemeChannelDesc& channel);   // 0x00B46280
    void SetupShaderPointLights(const CEngineLocalLightArray& lights);   // 0x00B44F20
    void DisablePointLights();                                           // 0x00B44D50
    void UpdateGlobalLightingStates();                                   // 0x00B46C80
    void SetupDefaultFogStates();                                        // 0x00B47630 ?SetupDefaultFogStates@CEngineLightingManager@@QAEXXZ (CEngine::SetDefaultRenderStates 0x00B25877)
    struct CThemeChannel {                                               // 0x70 bytes (Ego_r.pdb; U-keyed in the allocator names)
        CRGBFloatColour DiffuseColour;                                   // +0x00
        CRGBFloatColour AmbientColour;                                   // +0x10
        CRGBFloatColour BacklightColour;                                 // +0x20
        CRGBFloatColour ReflectionColour;                                // +0x30
        CRGBFloatColour FogColour;                                       // +0x40
        float           FogStartZ;                                       // +0x50
        float           FogEndZ;                                         // +0x54
        float           ShadowFactor;                                    // +0x58
        float           FadedShadowFactor;                               // +0x5c
        C4DVector       FogTransform;                                    // +0x60
    };
    C3DVector              SunVector;                                    // +0x18
    C3DVector              MoonVector;                                   // +0x24
    C3DVector              DiffuseVector;                                // +0x30
    CArray<CThemeChannel>  ThemeChannels;                                // +0x3c
    CRGBFloatColour        PrimitiveDiffuseColour;                       // +0x48
    CRGBFloatColour        PrimitiveAmbientColour;                       // +0x58
    CRGBFloatColour        PrimitiveBacklightColour;                     // +0x68
    CRGBFloatColour        PrimitiveFogColour;                           // +0x78
private:
    unsigned char          _pad_0x88[0xce - 0x88];                       // +0x88 PrimitiveFogStartZ .. ShadowedSpotLightsEnabled (opaque)
public:
    // Chunk 742 (RenderSingleSpriteTrail 0x00BBE090; the members and setters chunk 723c drafted for RenderSingleSprite 0x00BC04F0):
    // Ego_r.pdb names, retail offsets = Ego_r minus 0xC (the two CArray light lists are 0xC in retail, 0x10 in Ego_r):
    // FadedShadowDiffuseFactorEnabled Ego_r 0xDA, MistEffectColour 0xDC, CurrentThemeChannel 0xEC, StateUpdateFlags 0xF0,
    // SelfIlluminationAmbientPercentage 0xF4.
    bool                   FadedShadowDiffuseFactorEnabled;              // +0xce
    CRGBFloatColour        MistEffectColour;                             // +0xd0
    CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;                // +0xe0
    unsigned long          StateUpdateFlags;                             // +0xe4
    long                   SelfIlluminationAmbientPercentage;            // +0xe8
    // Ego_r.pdb / FableWin.pdb setters with no out-of-line copy in retail (0x00BBE090 expands them; FableWin debug bodies only):
    // ?SetEnableFadedShadowFactor@CEngineLightingManager@@QAEX_N@Z, ?SetCurrentEnvironmentThemeChannel@CEngineLightingManager@@
    // QAEXABVCEnginePrimitiveThemeChannelDesc@@@Z, ?SetSelfIlluminationAmbient@CEngineLightingManager@@QAEXJ@Z and
    // ?RestoreDefaultPrimitiveParameters@CEngineLightingManager@@QAEXXZ (the three resets in this order, 0x00BBE922..0x00BBE9BB).
    __forceinline void SetEnableFadedShadowFactor(bool enable)
    {
        if (FadedShadowDiffuseFactorEnabled != enable) {
            FadedShadowDiffuseFactorEnabled = enable;
            StateUpdateFlags |= 1;
        }
    }
    __forceinline void SetCurrentEnvironmentThemeChannel(const CEnginePrimitiveThemeChannelDesc& channel)
    {
        if (CurrentThemeChannel.ChannelIndex0 != channel.ChannelIndex0 || CurrentThemeChannel.ChannelIndex1 != channel.ChannelIndex1 ||
            CurrentThemeChannel.ChannelBlendFactor != channel.ChannelBlendFactor) {
            CurrentThemeChannel = channel;
            if (CurrentThemeChannel.ChannelBlendFactor == 0xff) {
                CurrentThemeChannel.ChannelIndex0 = CurrentThemeChannel.ChannelIndex1;
                CurrentThemeChannel.ChannelBlendFactor = 0;
            }
            StateUpdateFlags |= 0xf;
        }
    }
    __forceinline void SetSelfIlluminationAmbient(long percentage)
    {
        if (SelfIlluminationAmbientPercentage != percentage) {
            SelfIlluminationAmbientPercentage = percentage;
            StateUpdateFlags |= 2;
        }
    }
    __forceinline void RestoreDefaultPrimitiveParameters()
    {
        SetEnableFadedShadowFactor(false);
        SetCurrentEnvironmentThemeChannel(CEnginePrimitiveThemeChannelDesc());
        SetSelfIlluminationAmbient(0);
    }
private:
    unsigned char          _pad_0xec[0x46d0 - 0xec];                     // +0xec LightLookupGrid .. (opaque)
};
FABLE_STATIC_ASSERT(sizeof(CEngineLightingManager::CThemeChannel) == 0x70);
FABLE_STATIC_ASSERT(sizeof(CEngineLightingManager) == 0x46d0);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, SunVector) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, ThemeChannels) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveFogColour) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, SelfIlluminationAmbientPercentage) == 0xe8);
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineLightingManager = 0x46dc.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineLightingManager = 0x46d0.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CEngineLocalLightArray;
struct CLightingSIMDVarBlock;

#pragma pack(push, 1)
class CEngineLightingManager {
public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char           _pad_0x04[0x4];                  // +0x04
    unsigned char           _pad_0x08[0x4];                  // +0x08
    unsigned char           _pad_0x0c[0x4];                  // +0x0c
    unsigned char           _pad_0x10[0x4];                  // +0x10
    unsigned char           _pad_0x14[0x4];                  // +0x14
    float                   SunVector_X;                     // +0x18 C3DVector::X
    float                   SunVector_Y;                     // +0x1c C3DVector::Y
    float                   SunVector_Z;                     // +0x20 C3DVector::Z
    float                   MoonVector_X;                    // +0x24 C3DVector::X
    float                   MoonVector_Y;                    // +0x28 C3DVector::Y
    float                   MoonVector_Z;                    // +0x2c C3DVector::Z
    float                   DiffuseVector_X;                 // +0x30 C3DVector::X
    float                   DiffuseVector_Y;                 // +0x34 C3DVector::Y
    float                   DiffuseVector_Z;                 // +0x38 C3DVector::Z
    unsigned char           ThemeChannels[0xc];              // +0x3c CArray<CEngineLightingManager::CThemeChannel>
    float                   PrimitiveDiffuseColour_R;        // +0x48 CRGBFloatColour::R
    float                   PrimitiveDiffuseColour_G;        // +0x4c CRGBFloatColour::G
    float                   PrimitiveDiffuseColour_B;        // +0x50 CRGBFloatColour::B
    float                   PrimitiveDiffuseColour_A;        // +0x54 CRGBFloatColour::A
    float                   PrimitiveAmbientColour_R;        // +0x58 CRGBFloatColour::R
    float                   PrimitiveAmbientColour_G;        // +0x5c CRGBFloatColour::G
    float                   PrimitiveAmbientColour_B;        // +0x60 CRGBFloatColour::B
    float                   PrimitiveAmbientColour_A;        // +0x64 CRGBFloatColour::A
    float                   PrimitiveBacklightColour_R;      // +0x68 CRGBFloatColour::R
    float                   PrimitiveBacklightColour_G;      // +0x6c CRGBFloatColour::G
    float                   PrimitiveBacklightColour_B;      // +0x70 CRGBFloatColour::B
    float                   PrimitiveBacklightColour_A;      // +0x74 CRGBFloatColour::A
    float                   PrimitiveFogColour_R;            // +0x78 CRGBFloatColour::R
    float                   PrimitiveFogColour_G;            // +0x7c CRGBFloatColour::G
    float                   PrimitiveFogColour_B;            // +0x80 CRGBFloatColour::B
    float                   PrimitiveFogColour_A;            // +0x84 CRGBFloatColour::A
    float                   PrimitiveFogStartZ;              // +0x88
    float                   PrimitiveFogEndZ;                // +0x8c
    float                   LightAttenuation_Attenuation0;   // +0x90 CShaderLightAttenuation::Attenuation0
    float                   LightAttenuation_Attenuation1;   // +0x94 CShaderLightAttenuation::Attenuation1
    float                   LightAttenuation_Attenuation2;   // +0x98 CShaderLightAttenuation::Attenuation2
    float                   LightAttenuation_AttenuationBase; // +0x9c CShaderLightAttenuation::AttenuationBase
    long                    ShaderPointLightsEnabledCount;   // +0xa0
    bool                    DrawLightRadius;                 // +0xa4
    bool                    DrawSpotLightCone;               // +0xa5
    bool                    EnableLightCache;                // +0xa6
    unsigned char           _pad_0xa7[0x1];                  // +0xa7
    CEngineLocalLightArray* CurrentShaderLocalLights;        // +0xa8
    unsigned char           LocalLightList[0xc];             // +0xac CArray<CEngineLightingManager::CManagerLocalLight_*>
    unsigned char           ShadowedSpotLightList[0xc];      // +0xb8 CArray<CEngineLightingManager::CManagerShadowedSpotLight_*>
    float                   LookupGridWorldX;                // +0xc4
    float                   LookupGridWorldY;                // +0xc8
    bool                    LocalLightsEnabled;              // +0xcc
    bool                    ShadowedSpotLightsEnabled;       // +0xcd
    bool                    FadedShadowDiffuseFactorEnabled; // +0xce
    unsigned char           _pad_0xcf[0x1];                  // +0xcf
    float                   MistEffectColour_R;              // +0xd0 CRGBFloatColour::R
    float                   MistEffectColour_G;              // +0xd4 CRGBFloatColour::G
    float                   MistEffectColour_B;              // +0xd8 CRGBFloatColour::B
    float                   MistEffectColour_A;              // +0xdc CRGBFloatColour::A
    unsigned char           CurrentThemeChannel_ChannelIndex0; // +0xe0 CEnginePrimitiveThemeChannelDesc::ChannelIndex0
    unsigned char           CurrentThemeChannel_ChannelIndex1; // +0xe1 CEnginePrimitiveThemeChannelDesc::ChannelIndex1
    unsigned char           CurrentThemeChannel_ChannelBlendFactor; // +0xe2 CEnginePrimitiveThemeChannelDesc::ChannelBlendFactor
    unsigned char           CurrentThemeChannel_Pad;         // +0xe3 CEnginePrimitiveThemeChannelDesc::Pad
    unsigned long           StateUpdateFlags;                // +0xe4
    long                    SelfIlluminationAmbientPercentage; // +0xe8
    unsigned char           LightLookupGrid[0x400];          // +0xec CLocalLightElementLink *[16][16]
    unsigned char           SmallObjectLookupGrid[0x4000];   // +0x4ec CEngineLocalLightArray *[64][64]
    unsigned char           EmptyLightArray[0x1a4];          // +0x44ec CEngineLocalLightArray
    CLightingSIMDVarBlock*  SIMDVarBlock;                    // +0x4690
    unsigned char           ShadowMode[0x4];                 // +0x4694 ELightingManagerShadowMode
    unsigned char           CurrentShadowedSpotLightParams[0x38]; // +0x4698 CEngineLightingManagerSpotlightParams
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineLightingManager) == 0x46d0);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, SunVector_X) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, SunVector_Y) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, SunVector_Z) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, MoonVector_X) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, MoonVector_Y) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, MoonVector_Z) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, DiffuseVector_X) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, DiffuseVector_Y) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, DiffuseVector_Z) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, ThemeChannels) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveDiffuseColour_R) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveDiffuseColour_G) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveDiffuseColour_B) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveDiffuseColour_A) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveAmbientColour_R) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveAmbientColour_G) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveAmbientColour_B) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveAmbientColour_A) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveBacklightColour_R) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveBacklightColour_G) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveBacklightColour_B) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveBacklightColour_A) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveFogColour_R) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveFogColour_G) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveFogColour_B) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveFogColour_A) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveFogStartZ) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, PrimitiveFogEndZ) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LightAttenuation_Attenuation0) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LightAttenuation_Attenuation1) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LightAttenuation_Attenuation2) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LightAttenuation_AttenuationBase) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, ShaderPointLightsEnabledCount) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, DrawLightRadius) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, DrawSpotLightCone) == 0xa5);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, EnableLightCache) == 0xa6);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, CurrentShaderLocalLights) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LocalLightList) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, ShadowedSpotLightList) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LookupGridWorldX) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LookupGridWorldY) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LocalLightsEnabled) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, ShadowedSpotLightsEnabled) == 0xcd);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, FadedShadowDiffuseFactorEnabled) == 0xce);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, MistEffectColour_R) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, MistEffectColour_G) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, MistEffectColour_B) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, MistEffectColour_A) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, CurrentThemeChannel_ChannelIndex0) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, CurrentThemeChannel_ChannelIndex1) == 0xe1);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, CurrentThemeChannel_ChannelBlendFactor) == 0xe2);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, CurrentThemeChannel_Pad) == 0xe3);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, StateUpdateFlags) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, SelfIlluminationAmbientPercentage) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, LightLookupGrid) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, SmallObjectLookupGrid) == 0x4ec);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, EmptyLightArray) == 0x44ec);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, SIMDVarBlock) == 0x4690);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, ShadowMode) == 0x4694);
FABLE_STATIC_ASSERT(offsetof(CEngineLightingManager, CurrentShadowedSpotLightParams) == 0x4698);

#endif // FABLE_RETAIL_CENGINELIGHTINGMANAGER

#endif // FABLE_ENGINE_CENGINELIGHTINGMANAGER_H

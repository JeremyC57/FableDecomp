#ifndef FABLE_ENGINE_CENGINELIGHTINGMANAGER_H
#define FABLE_ENGINE_CENGINELIGHTINGMANAGER_H
#include "fable_compiler.h"
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
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23
#include "engine/CDynamicObjectHeap.h"
extern CDynamicObjectHeap<72,16> LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight;
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
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_ADDLIGHTPRIMITIVE
class CEngineInternalPrimitiveLight;
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_FRAMESTART
class CEngineCamera;
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE
class C3DBoundingSphere;
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
// Nested opt-in (chunk 892, row 0x00B482A0 the constructor): every member named, with the Ego_r.pdb names and types and the
// retail offsets (the landed destructor 0x00B47B50 model). The member classes the constructor builds in place:
// CShaderLightAttenuation (Ego_r.pdb V, 0x10; four zeroed floats at +0x90..+0x9C in member order), CEngineLocalLightElement
// (Ego_r.pdb V, 0x34, no constructor) and CEngineLocalLightArray (Ego_r.pdb V, 0x1A4; its inline constructor zeroes LightCount:
// 0x00B4832B `mov [esi+44ECh],ebx` in member-init order, no out-of-line copy in either PDB),
// CEngineLightingManagerSpotlightParams (Ego_r.pdb U, 0x38, byte-packed: Pos at +1).
class CShaderLightAttenuation {
public:
    CShaderLightAttenuation() : Attenuation0(0.0f), Attenuation1(0.0f), Attenuation2(0.0f), AttenuationBase(0.0f) { }
    float Attenuation0;                                                  // +0x00
    float Attenuation1;                                                  // +0x04
    float Attenuation2;                                                  // +0x08
    float AttenuationBase;                                               // +0x0c
};
class CEngineLocalLightElement {
public:
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23
 CEngineLocalLightElement(){}
 CEngineLocalLightElement(const C3DVector&,float,float,const CRGBFloatColour&,unsigned long);
#endif
    C3DVector       WorldPos;                                            // +0x00
    float           Radius;                                              // +0x0c
    float           RadiusSquared;                                       // +0x10
    float           SpotRadius;                                          // +0x14
    CRGBFloatColour Colour;                                              // +0x18
    float           AttenuationDistSquared;                              // +0x28
    float           AttenuationConstant;                                 // +0x2c
    unsigned long   ChannelMask;                                         // +0x30
};
class CEngineLocalLightArray {
public:
    CEngineLocalLightArray() : LightCount(0) { }
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
    // Opt-in (bwf1 / agentC2, row 0x00BB3E80): FableWin.pdb inline ?GetCount@CEngineLocalLightArray@@QBEJXZ (9 debug instructions).
    long GetCount() const { return LightCount; }
#endif
#ifdef FABLE_RETAIL_CENGINELOCALLIGHTARRAY_PEEKLIGHT
    // egod opt-in (e08, row 0x00B45CF0): FableWin.pdb ?PeekLight@CEngineLocalLightArray@@QBEABVCEngineLocalLightElement@@K@Z and (when
    // FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT does not already declare it) ?GetCount@CEngineLocalLightArray@@QBEJXZ, both inline
    // (Ego_d lists them in the lighting manager's object next to CalcSWLightingNoClip; no retail out-of-line copy). Bodies only.
    const CEngineLocalLightElement& PeekLight(unsigned long index) const { return Lights[index]; }
#ifndef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
    long GetCount() const { return LightCount; }
#endif
#endif
    unsigned long            LightCount;                                 // +0x00
    CEngineLocalLightElement Lights[8];                                  // +0x04
};
#pragma pack(push, 1)
struct CEngineLightingManagerSpotlightParams {
    bool            Active;                                              // +0x00
    C3DVector       Pos;                                                 // +0x01
    C3DVector       Vector;                                              // +0x0d
    unsigned char   _pad_0x19[0x3];                                      // +0x19
    float           Radius;                                              // +0x1c
    float           SpotRadius;                                          // +0x20
    float           ConeWidth;                                           // +0x24
    CRGBFloatColour Colour;                                              // +0x28
};
#pragma pack(pop)
struct CLightingSIMDVarBlock;
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWMODE_ENUM
#include "engine/ELightingManagerShadowMode.h"
#else
enum ELightingManagerShadowMode { };
#endif                                     // Ego_r.pdb enum (enumerator names not recovered)
#else
class CEngineLocalLightArray;
#endif
class CEngineLightingManager : public CEnginePrimitiveManager, public CEngineComponent {
public:
#ifdef FABLE_RETAIL_LIGHT_CACHE_SPHERE_PASS30
    CEngineLocalLightArray* GetLightsInSphere(const C3DBoundingSphere& sphere, long max_lights,
                                               const CEnginePrimitiveThemeChannelDesc& theme_channel);
private:
    CEngineLocalLightArray* LookupSmallObjectLightCache(float x, float y, long max_lights);
public:
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTINGNOCLIP
    // egod opt-in (e08, row 0x00B45CF0): Ego_r.pdb / FableWin.pdb
    // ?CalcSWLightingNoClip@CEngineLightingManager@@QAEXABVC3DVector@@0ABVCEngineLocalLightArray@@AAVCRGBFloatColour@@@Z (Ego_d parameter
    // names pos, normal, light_array, light_result). Declaration only, no layout change.
    void CalcSWLightingNoClip(const C3DVector& pos,const C3DVector& normal,const CEngineLocalLightArray& light_array,CRGBFloatColour& light_result);
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_VECTOR
    // Opt-in (hand3c2, row 0x00B99300 CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW): the two SSE batch lighters it calls,
    // Ego_r.pdb / Ego_d.pdb ?CalcSWLightingNoClipVector@CEngineLightingManager@@QAEXPAVCRGBFloatColour@@PBVC4DVector@@PBM22JABVCEngineLocalLightArray@@@Z
    // (retail 0x00B44820) and ?CalcSWLightingNoClipSeparateMainLightVector@CEngineLightingManager@@QAEXPAVCRGBFloatColour@@0PBVC4DVector@@PBM22JABVCEngineLocalLightArray@@@Z
    // (retail 0x00B44A60; engine/CEngineLightingSSE.h names both). Parameter names = their Ego_d records. Declarations only.
    void CalcSWLightingNoClipVector(CRGBFloatColour* results, const C4DVector* position, const float* vec_normal_x,
        const float* vec_normal_y, const float* vec_normal_z, long count, const CEngineLocalLightArray& light_array);
    void CalcSWLightingNoClipSeparateMainLightVector(CRGBFloatColour* results, CRGBFloatColour* results_main_light,
        const C4DVector* position, const float* vec_normal_x, const float* vec_normal_y, const float* vec_normal_z, long count,
        const CEngineLocalLightArray& light_array);
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_SEPARATE_MAIN
    // Retail opt-in (Codex pass 24): the original public member, retail 0x00B45A40.
    void CalcSWLightingNoClipSeparateMainLight(const C3DVector& pos, const C3DVector& normal,
        const CEngineLocalLightArray& light_array, CRGBFloatColour& main_light_result,
        CRGBFloatColour& misc_light_result);
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_LOCALLIGHTS
    // e11 opt-in (row 0x00B45720): Ego_r.pdb / FableWin.pdb ?CalcSWLighting@CEngineLightingManager@@QAE?AVCRGBFloatColour@@ABVC3DVector@@0ABVCEngineLocalLightArray@@@Z
    // (the overload with the local lights; Ego_r order: the global-only overload 0x009BA660, this one 0x009BA8A0 = retail 0x00B45510 /
    // 0x00B45720). Parameter names as the global-only overload's Ego_d record and CalcSWLightingNoClip's (pos, normal, light_array).
    // Declaration only, no layout change.
    CRGBFloatColour CalcSWLighting(const C3DVector& pos,const C3DVector& normal,const CEngineLocalLightArray& light_array);
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALDIFFUSE
    // Original public const reference accessor, donor02CA1E00; native reads first ThemeChannel.
    const CRGBFloatColour& PeekGlobalDiffuseColour() const { return ThemeChannels[0].DiffuseColour; }
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_GLOBAL_LIGHTING_PASS23
    // Both original PDB public const-reference accessors and setter.
    const C3DVector& PeekDiffuseVector() const {return DiffuseVector;}
    const CRGBFloatColour& PeekGlobalAmbientColour() const {return ThemeChannels[0].AmbientColour;}
    const CRGBFloatColour& PeekGlobalBacklightColour() const {return ThemeChannels[0].BacklightColour;}
    void SetGlobalLightingParameters(const C3DVector&,const CRGBFloatColour&,const CRGBFloatColour&,const CRGBFloatColour&);
#endif
    // Chunk 731: the constructor (CEngine::Initialise 0x00B26360 news 0x46D0 bytes) and the overrides of the pure CEnginePrimitiveManager
    // slots and CEngineComponent::PeekSceneFilterFlags (Ego_r.pdb / FableWin.pdb spellings).
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_FRAMESTART
    // Retail opt-in (Codex pass22): both original PDBs, module983; native secondary-base slot012A2240.
    virtual void FrameStart(const CEngineComponentParams& comp_params);
    void UpdateShadowScene(const CEngineCamera& camera);
private:
    void UpdateShadowedSpotLights();
    void AddLightsToLookupGrid();
    void UpdateSIMDVarBlock();
public:
#endif
    CEngineLightingManager();                                            // 0x00B482A0 ??0CEngineLightingManager@@QAE@XZ
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKSUNVECTOR
    // Opt-in (chunk 2048): FableWin.pdb ?PeekSunVector@CEngineLightingManager@@QBEABVC3DVector@@XZ (inline; RenderSunFlare reads +0x18..+0x20).
    const C3DVector& PeekSunVector() const { return SunVector; }
#endif
#ifdef FABLE_RETAIL_CENGINESKYRENDERER_MOON_PASS30
    // The FableWin PeekMoonVector returns this member, retail reads +0x24.
    const C3DVector& PeekMoonVector() const { return MoonVector; }
#endif
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);   // ?CreateInternalPrimitive@CEngineLightingManager@@UAEPAVCEngineInternalPrimitiveBase@@W4EEnginePrimitiveType@@@Z
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;                      // ?GetSupportedTypes@CEngineLightingManager@@UBEXAAVCEnginePrimitiveTypeList@@@Z
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);   // ?RenderPrimitive@CEngineLightingManager@@UAE?AW4ERendered@@...@Z
    virtual unsigned long PeekSceneFilterFlags() const;                  // ?PeekSceneFilterFlags@CEngineLightingManager@@UBEKXZ
    // Ego_r.pdb / FableWin.pdb publics, out of line in retail (the ice pass 0x00B751F0 calls them):
    CEngineLocalLightArray* GetLightsInArea(const C3DBoundingBox& area, long maxLights,
                                            const CEnginePrimitiveThemeChannelDesc& channel);   // 0x00B46280
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE
    // Nested opt-in (chunk 2032, row 0x00BA6FF0): Ego_r.pdb ?GetLightsInSphereFast@CEngineLightingManager@@QAEPAVCEngineLocalLightArray@@ABVC3DBoundingSphere@@JABVCEnginePrimitiveThemeChannelDesc@@@Z
    // (donor 0x009BC860; reloc_bindings.tsv binds 0x00B46B90 to it). Declaration only.
    CEngineLocalLightArray* GetLightsInSphereFast(const C3DBoundingSphere& sphere, long maxLights,
                                                  const CEnginePrimitiveThemeChannelDesc& channel);   // 0x00B46B90
#ifdef FABLE_RETAIL_GET_OPINIONS_LIGHT_CACHE_PASS30
    CEngineLocalLightArray* GetLightsInSphere(const C3DBoundingSphere&, long, const CEnginePrimitiveThemeChannelDesc&);
private:
    CEngineLocalLightArray* LookupSmallObjectLightCache(float, float, long);
public:
#endif
#endif
    void SetupShaderPointLights(const CEngineLocalLightArray& lights);   // 0x00B44F20
    void DisablePointLights();                                           // 0x00B44D50
    void UpdateGlobalLightingStates();                                   // 0x00B46C80
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_ADDLIGHTPRIMITIVE
    // NF151 opt-in (row 0x00B47FD0): Ego_r.pdb 0x009BD400 / FableWin.pdb 0x02CEFE10
    // ?AddLightPrimitive@CEngineLightingManager@@QAEXABVCEngineInternalPrimitiveLight@@@Z (the point-light overload). Declaration only.
    void AddLightPrimitive(const CEngineInternalPrimitiveLight& light);   // 0x00B47FD0
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
    // Opt-in (bwf1 / agentC2, row 0x00BB3E80): Ego_r.pdb / FableWin.pdb ?SetupShaderSpotLight@CEngineLightingManager@@QAEXABVCMatrix3x4@@@Z (0x00B47390).
    void SetupShaderSpotLight(const CMatrix3x4& transform);              // 0x00B47390
#endif
#if defined(FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23) && !defined(FABLE_RETAIL_CENGINELIGHTINGMANAGER_FRAMESTART)
private:
 void UpdateShadowedSpotLights();
public:
#endif
    void SetupDefaultFogStates();                                        // 0x00B47630 ?SetupDefaultFogStates@CEngineLightingManager@@QAEXXZ (CEngine::SetDefaultRenderStates 0x00B25877)
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_ENABLEFOG
    // NF68 opt-in (row 0x00B32AD0): Ego_r.pdb 0x009BC540 ?EnableFog@CEngineLightingManager@@QAEXXZ (retail 0x00B46890, out of line). Declaration only.
    void EnableFog();                                                    // 0x00B46890
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_FOGTRANSFORM
    // Chunk 764 (row 0x00B451D0): Ego_r.pdb ?CalculateFogTransformMatrix@CEngineLightingManager@@AAE?AVC4DVector@@MM@Z (private). Declaration only.
private:
    C4DVector CalculateFogTransformMatrix(float fogStart, float fogEnd);
public:
#endif
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
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SHADOWED_UPDATE_PASS23
    struct CManagerLocalLight {
     virtual void Delete();
     CEngineLocalLightElement LightElement;
     CManagerLocalLight* NextLocalLight;
     float ClippingScore,ThemeWeight;
     bool AlreadyFoundFlag;
     CManagerLocalLight(const C3DVector& world_pos,float radius,float spot_radius,const CRGBFloatColour& colour,unsigned long theme_channel_mask):LightElement(world_pos,radius,spot_radius,colour,theme_channel_mask),AlreadyFoundFlag(false){}
#if defined(FABLE_RETAIL_PASS23_HOST_TEST_ABI) && !defined(_MSC_VER)
     static void* operator new(size_t size) throw(){return LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight.Alloc((long)size);}
#else
     static void* FABLE_FASTCALL operator new(unsigned int size){return LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight.Alloc((long)size);}
#endif
     static void FABLE_FASTCALL operator delete(void*);
#if defined(FABLE_RETAIL_PASS23_HOST_TEST_ABI) && !defined(_MSC_VER)
     static void* operator new(size_t,void* p) throw(){return p;}
#else
     static void* FABLE_FASTCALL operator new(unsigned int,void* p){return p;}
#endif
     static void FABLE_FASTCALL operator delete(void*,void*){}
    };
    struct CManagerShadowedSpotLight {
     virtual void Delete();
     C3DVector Position,Direction;
     float Radius,SpotRadius,ConeRadius;
     CRGBFloatColour Colour;
     CManagerShadowedSpotLight(const C3DVector&,const C3DVector&,float,float,float,const CRGBFloatColour&);
    };
#else
    struct CManagerLocalLight;                                           // Ego_r.pdb U (nested)
    struct CManagerShadowedSpotLight;                                    // Ego_r.pdb U (nested)
#endif
    class CLocalLightElementLink;
    float                  PrimitiveFogStartZ;                           // +0x88
    float                  PrimitiveFogEndZ;                             // +0x8c
    CShaderLightAttenuation LightAttenuation;                            // +0x90
    long                   ShaderPointLightsEnabledCount;                // +0xa0
    bool                   DrawLightRadius;                              // +0xa4
    bool                   DrawSpotLightCone;                            // +0xa5
    bool                   EnableLightCache;                             // +0xa6
    CEngineLocalLightArray* CurrentShaderLocalLights;                    // +0xa8
    CArray<CManagerLocalLight*> LocalLightList;                          // +0xac
    CArray<CManagerShadowedSpotLight*> ShadowedSpotLightList;            // +0xb8
    float                  LookupGridWorldX;                             // +0xc4
    float                  LookupGridWorldY;                             // +0xc8
    bool                   LocalLightsEnabled;                           // +0xcc
    bool                   ShadowedSpotLightsEnabled;                    // +0xcd
#else
private:
    unsigned char          _pad_0x88[0xce - 0x88];                       // +0x88 PrimitiveFogStartZ .. ShadowedSpotLightsEnabled (opaque)
public:
#endif
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
    // Correction (0x00B68DA0): SetEnableFadedShadowFactor DOES have an out-of-line copy in retail, 0x00B66EB0
    // (`mov eax,[ecx+4694h] ; cmp eax,1 ; je ; xor al,al ; ... cmp al,[ecx+0CEh]`): its real body starts with
    // `if (PeekGlobalShadowMode() != 1) enable = false;` -- that guard needs ShadowMode (+0x4694), so it is modelled under
    // FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD (requires _MEMBERS); the default body below leaves it out (with a
    // constant `false` argument, as in RestoreDefaultPrimitiveParameters, the guard is dead code anyway).
    // ?SetEnableFadedShadowFactor@CEngineLightingManager@@QAEX_N@Z, ?SetCurrentEnvironmentThemeChannel@CEngineLightingManager@@
    // QAEXABVCEnginePrimitiveThemeChannelDesc@@@Z, ?SetSelfIlluminationAmbient@CEngineLightingManager@@QAEXJ@Z and
    // ?RestoreDefaultPrimitiveParameters@CEngineLightingManager@@QAEXXZ (the three resets in this order, 0x00BBE922..0x00BBE9BB).
#if defined(FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD) && defined(FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD_DIRECT)
    // Retail opt-in (agentC17, rows 0x00BA5260 / 0x00BA2350; needs _SETFADED_SHADOWGUARD + _MEMBERS): the same guarded setter with the
    // shadow mode read straight from the member. FableWin ?SetEnableFadedShadowFactor@CEngineLightingManager@@QAEX_N@Z is 24i/80b; this
    // text gives 24i/80b at plain /Od (fwsize SAME), the PeekGlobalShadowMode() spelling 25i/81b. Retail out-of-line copy 0x00B66EB0.
    FABLE_FORCEINLINE ELightingManagerShadowMode PeekGlobalShadowMode() const { return ShadowMode; }
    FABLE_FORCEINLINE void SetEnableFadedShadowFactor(bool enable)
    {
        if (ShadowMode != (ELightingManagerShadowMode)1)
            enable = false;
        if (enable != FadedShadowDiffuseFactorEnabled) {
            FadedShadowDiffuseFactorEnabled = enable;
            StateUpdateFlags |= 1;
        }
    }
#elif defined(FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD)
    // FableWin ?PeekGlobalShadowMode@CEngineLightingManager@@QBE?AW4ELightingManagerShadowMode@@XZ (inline).
    FABLE_FORCEINLINE ELightingManagerShadowMode PeekGlobalShadowMode() const { return ShadowMode; }
    // FableWin ?SetEnableFadedShadowFactor@CEngineLightingManager@@QAEX_N@Z, the PDB body (out-of-line copy 0x00B66EB0).
    // Inline-budget: retail 0x00B68DA0 RenderSortedPatches expands this setter, guard included, at its call sites.
    FABLE_FORCEINLINE void SetEnableFadedShadowFactor(bool enable)
    {
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWMODE_DIRECT
        // Nested opt-in (wfA5, row 0x00B9AC30): FableWin.pdb ?SetEnableFadedShadowFactor@CEngineLightingManager@@QAEX_N@Z is 24 debug
        // instructions / 80 bytes = the guard reading the member directly (plain VC7.1 /Od of this text: 24 / 80, fwsize SAME); through
        // PeekGlobalShadowMode() it is 25 / 81. Code-identical in release.
        if (ShadowMode != (ELightingManagerShadowMode)1)
            enable = false;
#else
        if (PeekGlobalShadowMode() != (ELightingManagerShadowMode)1)
            enable = false;
#endif
        if (enable != FadedShadowDiffuseFactorEnabled) {
            FadedShadowDiffuseFactorEnabled = enable;
            StateUpdateFlags |= 1;
        }
    }
#else
    FABLE_FORCEINLINE void SetEnableFadedShadowFactor(bool enable)
    {
        if (FadedShadowDiffuseFactorEnabled != enable) {
            FadedShadowDiffuseFactorEnabled = enable;
            StateUpdateFlags |= 1;
        }
    }
#endif
    FABLE_FORCEINLINE void SetCurrentEnvironmentThemeChannel(const CEnginePrimitiveThemeChannelDesc& channel)
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
    FABLE_FORCEINLINE void SetSelfIlluminationAmbient(long percentage)
    {
        if (SelfIlluminationAmbientPercentage != percentage) {
            SelfIlluminationAmbientPercentage = percentage;
            StateUpdateFlags |= 2;
        }
    }
    FABLE_FORCEINLINE void RestoreDefaultPrimitiveParameters()
    {
        SetEnableFadedShadowFactor(false);
        SetCurrentEnvironmentThemeChannel(CEnginePrimitiveThemeChannelDesc());
        SetSelfIlluminationAmbient(0);
    }
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
    CLocalLightElementLink* LightLookupGrid[16][16];                     // +0xec
    CEngineLocalLightArray* SmallObjectLookupGrid[64][64];               // +0x4ec
    CEngineLocalLightArray EmptyLightArray;                              // +0x44ec
    CLightingSIMDVarBlock* SIMDVarBlock;                                 // +0x4690
    ELightingManagerShadowMode ShadowMode;                               // +0x4694
    CEngineLightingManagerSpotlightParams CurrentShadowedSpotLightParams;   // +0x4698
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALSHADOWMODE
    // Nested opt-in (chunk 2024, row 0x00B4FC40 CEngineShadowRenderer::SetupShadowFuncRenderStates): FableWin.pdb
    // ?PeekGlobalShadowMode@CEngineLightingManager@@QBE?AW4ELightingManagerShadowMode@@XZ (0x02D739B0; no Ego_r.pdb or Fable.exe copy),
    // inline in retail: `mov ecx,[0x01436E9C] ; cmp dword ptr [ecx+4694h],1` (0x00B5030F).
    ELightingManagerShadowMode PeekGlobalShadowMode() const { return ShadowMode; }
#endif
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETSHADOWMODE
    // Nested opt-in (NF42, row 0x00B23C00 CEngine::SetSpotlightShadowMode): FableWin.pdb
    // ?SetShadowMode@CEngineLightingManager@@QAEXW4ELightingManagerShadowMode@@@Z (0x02C9E7C0, 10 debug instructions; no Fable.exe copy),
    // inline in retail: `mov ecx,[0x01436E9C] ; ... ; mov [ecx+4694h],eax` (0x00B23C00).
    void SetShadowMode(ELightingManagerShadowMode mode) { ShadowMode = mode; }
#endif
#else
private:
    unsigned char          _pad_0xec[0x46d0 - 0xec];                     // +0xec LightLookupGrid .. (opaque)
#endif
};
FABLE_STATIC_ASSERT_AT(241, sizeof(CEngineLightingManager::CThemeChannel) == 0x70);
FABLE_STATIC_ASSERT_AT(242, sizeof(CEngineLightingManager) == 0x46d0);
FABLE_STATIC_ASSERT_AT(243, offsetof(CEngineLightingManager, SunVector) == 0x18);
FABLE_STATIC_ASSERT_AT(244, offsetof(CEngineLightingManager, ThemeChannels) == 0x3c);
FABLE_STATIC_ASSERT_AT(245, offsetof(CEngineLightingManager, PrimitiveFogColour) == 0x78);
FABLE_STATIC_ASSERT_AT(246, offsetof(CEngineLightingManager, SelfIlluminationAmbientPercentage) == 0xe8);
#ifdef FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
FABLE_STATIC_ASSERT_AT(248, sizeof(CEngineLocalLightArray) == 0x1a4);
FABLE_STATIC_ASSERT_AT(249, sizeof(CEngineLightingManagerSpotlightParams) == 0x38);
FABLE_STATIC_ASSERT_AT(250, offsetof(CEngineLightingManager, LightAttenuation) == 0x90);
FABLE_STATIC_ASSERT_AT(251, offsetof(CEngineLightingManager, LocalLightList) == 0xac);
FABLE_STATIC_ASSERT_AT(252, offsetof(CEngineLightingManager, LocalLightsEnabled) == 0xcc);
FABLE_STATIC_ASSERT_AT(253, offsetof(CEngineLightingManager, LightLookupGrid) == 0xec);
FABLE_STATIC_ASSERT_AT(254, offsetof(CEngineLightingManager, EmptyLightArray) == 0x44ec);
FABLE_STATIC_ASSERT_AT(255, offsetof(CEngineLightingManager, ShadowMode) == 0x4694);
FABLE_STATIC_ASSERT_AT(256, offsetof(CEngineLightingManager, CurrentShadowedSpotLightParams) == 0x4698);
#endif
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

FABLE_STATIC_ASSERT_AT(345, sizeof(CEngineLightingManager) == 0x46d0);
FABLE_STATIC_ASSERT_AT(346, offsetof(CEngineLightingManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(347, offsetof(CEngineLightingManager, SunVector_X) == 0x18);
FABLE_STATIC_ASSERT_AT(348, offsetof(CEngineLightingManager, SunVector_Y) == 0x1c);
FABLE_STATIC_ASSERT_AT(349, offsetof(CEngineLightingManager, SunVector_Z) == 0x20);
FABLE_STATIC_ASSERT_AT(350, offsetof(CEngineLightingManager, MoonVector_X) == 0x24);
FABLE_STATIC_ASSERT_AT(351, offsetof(CEngineLightingManager, MoonVector_Y) == 0x28);
FABLE_STATIC_ASSERT_AT(352, offsetof(CEngineLightingManager, MoonVector_Z) == 0x2c);
FABLE_STATIC_ASSERT_AT(353, offsetof(CEngineLightingManager, DiffuseVector_X) == 0x30);
FABLE_STATIC_ASSERT_AT(354, offsetof(CEngineLightingManager, DiffuseVector_Y) == 0x34);
FABLE_STATIC_ASSERT_AT(355, offsetof(CEngineLightingManager, DiffuseVector_Z) == 0x38);
FABLE_STATIC_ASSERT_AT(356, offsetof(CEngineLightingManager, ThemeChannels) == 0x3c);
FABLE_STATIC_ASSERT_AT(357, offsetof(CEngineLightingManager, PrimitiveDiffuseColour_R) == 0x48);
FABLE_STATIC_ASSERT_AT(358, offsetof(CEngineLightingManager, PrimitiveDiffuseColour_G) == 0x4c);
FABLE_STATIC_ASSERT_AT(359, offsetof(CEngineLightingManager, PrimitiveDiffuseColour_B) == 0x50);
FABLE_STATIC_ASSERT_AT(360, offsetof(CEngineLightingManager, PrimitiveDiffuseColour_A) == 0x54);
FABLE_STATIC_ASSERT_AT(361, offsetof(CEngineLightingManager, PrimitiveAmbientColour_R) == 0x58);
FABLE_STATIC_ASSERT_AT(362, offsetof(CEngineLightingManager, PrimitiveAmbientColour_G) == 0x5c);
FABLE_STATIC_ASSERT_AT(363, offsetof(CEngineLightingManager, PrimitiveAmbientColour_B) == 0x60);
FABLE_STATIC_ASSERT_AT(364, offsetof(CEngineLightingManager, PrimitiveAmbientColour_A) == 0x64);
FABLE_STATIC_ASSERT_AT(365, offsetof(CEngineLightingManager, PrimitiveBacklightColour_R) == 0x68);
FABLE_STATIC_ASSERT_AT(366, offsetof(CEngineLightingManager, PrimitiveBacklightColour_G) == 0x6c);
FABLE_STATIC_ASSERT_AT(367, offsetof(CEngineLightingManager, PrimitiveBacklightColour_B) == 0x70);
FABLE_STATIC_ASSERT_AT(368, offsetof(CEngineLightingManager, PrimitiveBacklightColour_A) == 0x74);
FABLE_STATIC_ASSERT_AT(369, offsetof(CEngineLightingManager, PrimitiveFogColour_R) == 0x78);
FABLE_STATIC_ASSERT_AT(370, offsetof(CEngineLightingManager, PrimitiveFogColour_G) == 0x7c);
FABLE_STATIC_ASSERT_AT(371, offsetof(CEngineLightingManager, PrimitiveFogColour_B) == 0x80);
FABLE_STATIC_ASSERT_AT(372, offsetof(CEngineLightingManager, PrimitiveFogColour_A) == 0x84);
FABLE_STATIC_ASSERT_AT(373, offsetof(CEngineLightingManager, PrimitiveFogStartZ) == 0x88);
FABLE_STATIC_ASSERT_AT(374, offsetof(CEngineLightingManager, PrimitiveFogEndZ) == 0x8c);
FABLE_STATIC_ASSERT_AT(375, offsetof(CEngineLightingManager, LightAttenuation_Attenuation0) == 0x90);
FABLE_STATIC_ASSERT_AT(376, offsetof(CEngineLightingManager, LightAttenuation_Attenuation1) == 0x94);
FABLE_STATIC_ASSERT_AT(377, offsetof(CEngineLightingManager, LightAttenuation_Attenuation2) == 0x98);
FABLE_STATIC_ASSERT_AT(378, offsetof(CEngineLightingManager, LightAttenuation_AttenuationBase) == 0x9c);
FABLE_STATIC_ASSERT_AT(379, offsetof(CEngineLightingManager, ShaderPointLightsEnabledCount) == 0xa0);
FABLE_STATIC_ASSERT_AT(380, offsetof(CEngineLightingManager, DrawLightRadius) == 0xa4);
FABLE_STATIC_ASSERT_AT(381, offsetof(CEngineLightingManager, DrawSpotLightCone) == 0xa5);
FABLE_STATIC_ASSERT_AT(382, offsetof(CEngineLightingManager, EnableLightCache) == 0xa6);
FABLE_STATIC_ASSERT_AT(383, offsetof(CEngineLightingManager, CurrentShaderLocalLights) == 0xa8);
FABLE_STATIC_ASSERT_AT(384, offsetof(CEngineLightingManager, LocalLightList) == 0xac);
FABLE_STATIC_ASSERT_AT(385, offsetof(CEngineLightingManager, ShadowedSpotLightList) == 0xb8);
FABLE_STATIC_ASSERT_AT(386, offsetof(CEngineLightingManager, LookupGridWorldX) == 0xc4);
FABLE_STATIC_ASSERT_AT(387, offsetof(CEngineLightingManager, LookupGridWorldY) == 0xc8);
FABLE_STATIC_ASSERT_AT(388, offsetof(CEngineLightingManager, LocalLightsEnabled) == 0xcc);
FABLE_STATIC_ASSERT_AT(389, offsetof(CEngineLightingManager, ShadowedSpotLightsEnabled) == 0xcd);
FABLE_STATIC_ASSERT_AT(390, offsetof(CEngineLightingManager, FadedShadowDiffuseFactorEnabled) == 0xce);
FABLE_STATIC_ASSERT_AT(391, offsetof(CEngineLightingManager, MistEffectColour_R) == 0xd0);
FABLE_STATIC_ASSERT_AT(392, offsetof(CEngineLightingManager, MistEffectColour_G) == 0xd4);
FABLE_STATIC_ASSERT_AT(393, offsetof(CEngineLightingManager, MistEffectColour_B) == 0xd8);
FABLE_STATIC_ASSERT_AT(394, offsetof(CEngineLightingManager, MistEffectColour_A) == 0xdc);
FABLE_STATIC_ASSERT_AT(395, offsetof(CEngineLightingManager, CurrentThemeChannel_ChannelIndex0) == 0xe0);
FABLE_STATIC_ASSERT_AT(396, offsetof(CEngineLightingManager, CurrentThemeChannel_ChannelIndex1) == 0xe1);
FABLE_STATIC_ASSERT_AT(397, offsetof(CEngineLightingManager, CurrentThemeChannel_ChannelBlendFactor) == 0xe2);
FABLE_STATIC_ASSERT_AT(398, offsetof(CEngineLightingManager, CurrentThemeChannel_Pad) == 0xe3);
FABLE_STATIC_ASSERT_AT(399, offsetof(CEngineLightingManager, StateUpdateFlags) == 0xe4);
FABLE_STATIC_ASSERT_AT(400, offsetof(CEngineLightingManager, SelfIlluminationAmbientPercentage) == 0xe8);
FABLE_STATIC_ASSERT_AT(401, offsetof(CEngineLightingManager, LightLookupGrid) == 0xec);
FABLE_STATIC_ASSERT_AT(402, offsetof(CEngineLightingManager, SmallObjectLookupGrid) == 0x4ec);
FABLE_STATIC_ASSERT_AT(403, offsetof(CEngineLightingManager, EmptyLightArray) == 0x44ec);
FABLE_STATIC_ASSERT_AT(404, offsetof(CEngineLightingManager, SIMDVarBlock) == 0x4690);
FABLE_STATIC_ASSERT_AT(405, offsetof(CEngineLightingManager, ShadowMode) == 0x4694);
FABLE_STATIC_ASSERT_AT(406, offsetof(CEngineLightingManager, CurrentShadowedSpotLightParams) == 0x4698);

#endif // FABLE_RETAIL_CENGINELIGHTINGMANAGER

#endif // FABLE_ENGINE_CENGINELIGHTINGMANAGER_H

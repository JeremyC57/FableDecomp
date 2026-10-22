// Grade: DIFFER(528v528) -- only the EmptyLightArray.LightCount store (+0x44ec) is scheduled before `lea ebp,[esi+0x3c]` (retail after); tried ctor-init/body-init at 4 positions, element ctors, memset loops, Clear()
// CEngineLightingManager::CEngineLightingManager() -- builds the lighting manager: primitive-manager and engine-component bases, every list and grid empty, light cache and local/shadowed-spot lights enabled, the SIMD lighting block allocated (0x110 bytes, 16-aligned), sun/moon/diffuse vectors, one default theme channel (grey diffuse/ambient, black backlight/fog, fog 1000..2000), then registers itself with the primitive type info
// Leaf: ??0CEngineLightingManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B482A0, length 528 bytes, plain `ret`. Manifest label
//   "CEnginePrimitiveManager::Initialise" is wrong: the body is the lighting manager's constructor (vtables 0x012A2274 / 0x012A223C).
// Callees: CEnginePrimitiveManager::CEnginePrimitiveManager(CArray<CEngineScene*>*) @ 0x00BACDB0 (renderer @ 0x01436E80 +0x338),
//   CEngineComponent::CEngineComponent @ 0x00B59710, AllocateAligned(ulong, long) @ 0x009F2BE0 (__fastcall),
//   vector<CThemeChannel>::resize(size_type, const CThemeChannel&) @ 0x00B4A4C0,
//   CEnginePrimitiveTypeInfo::RegisterPrimitiveManager @ 0x00B8FAD0 (global @ 0x01436E84).
// Layout: Ego_r.pdb CEngineLightingManager, shifted -4 after ThemeChannels (retail STLport vectors are 12 bytes): Diffuse colour
//   +0x48 ... LightAttenuation +0x90, ShaderPointLightsEnabledCount +0xa0, flags +0xa4, lists +0xac/+0xb8, LocalLightsEnabled +0xcc,
//   MistEffectColour +0xd0, CurrentThemeChannel +0xe0, StateUpdateFlags +0xe4, SelfIllumination +0xe8, grids +0xec/+0x4ec,
//   EmptyLightArray +0x44ec, SIMDVarBlock +0x4690, ShadowMode +0x4694, spot light params +0x4698.
#include <string.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    void Set(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
};
struct C4DVector { float X, Y, Z, W; C4DVector() { } };
struct CThemeChannel {                             // 0x70 bytes
    CRGBFloatColour DiffuseColour;                 // +0x00
    CRGBFloatColour AmbientColour;                 // +0x10
    CRGBFloatColour BacklightColour;               // +0x20
    CRGBFloatColour ReflectionColour;              // +0x30
    CRGBFloatColour FogColour;                     // +0x40
    float           FogStartZ;                     // +0x50
    float           FogEndZ;                       // +0x54
    float           ShadowFactor;                  // +0x58
    float           FadedShadowFactor;             // +0x5c
    C4DVector       FogTransform;                  // +0x60
};
template <class T> struct vector {                 // retail 12-byte STLport vector
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void resize(unsigned long n, const T& x);      // 0x00B4A4C0 (CThemeChannel)
};
struct CShaderLightAttenuation {
    float Attenuation0, Attenuation1, Attenuation2, AttenuationBase;
    CShaderLightAttenuation() : Attenuation0(0.0f), Attenuation1(0.0f), Attenuation2(0.0f), AttenuationBase(0.0f) { }
};
struct CEnginePrimitiveThemeChannelDesc {
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CEngineLocalLightArray {                    // 0x1a4 bytes
    long          LightCount;
    unsigned char Lights[0x1a0];
    CEngineLocalLightArray() : LightCount(0) { }
};
struct CManagerLocalLight;
struct CManagerShadowedSpotLight;
struct CLocalLightElementLink;
struct CLightingSIMDVarBlock { unsigned char Data[0x110]; };
void* __fastcall AllocateAligned(unsigned long size, long alignment);   // 0x009F2BE0

struct CEngineSceneArray;
struct CEnginePrimitiveManager {
    virtual ~CEnginePrimitiveManager();
    CEngineSceneArray* SceneList;                  // +0x04
    bool               Enable;                     // +0x08
    CEnginePrimitiveManager(CEngineSceneArray* scenes);   // 0x00BACDB0
};
struct CEngineComponent {                          // 0x0C bytes
    virtual ~CEngineComponent();
    void** m_ppOwnerLink;                          // +0x04
    void*  m_pOwner;                               // +0x08
    CEngineComponent();                            // 0x00B59710
};
struct CEnginePrimitiveRenderer {
    unsigned char _pad000[0x338];
    unsigned char Scenes[4];                       // +0x338 CArray<CEngineScene*>
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80
struct CEnginePrimitiveTypeInfo {
    void RegisterPrimitiveManager(CEnginePrimitiveManager* manager);   // 0x00B8FAD0
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

#pragma pack(push, 1)
struct CEngineLightingManagerSpotlightParams {     // 0x38 bytes, byte-packed (PDB)
    bool          Active;                          // +0x00
    unsigned char _rest[0x37];
};
#pragma pack(pop)

struct CEngineLightingManager : CEnginePrimitiveManager, CEngineComponent {
    C3DVector                     SunVector;                 // +0x18
    C3DVector                     MoonVector;                // +0x24
    C3DVector                     DiffuseVector;             // +0x30
    vector<CThemeChannel>         ThemeChannels;             // +0x3c
    CRGBFloatColour               PrimitiveDiffuseColour;    // +0x48
    CRGBFloatColour               PrimitiveAmbientColour;    // +0x58
    CRGBFloatColour               PrimitiveBacklightColour;  // +0x68
    CRGBFloatColour               PrimitiveFogColour;        // +0x78
    float                         PrimitiveFogStartZ;        // +0x88
    float                         PrimitiveFogEndZ;          // +0x8c
    CShaderLightAttenuation       LightAttenuation;          // +0x90
    long                          ShaderPointLightsEnabledCount;   // +0xa0
    bool                          DrawLightRadius;           // +0xa4
    bool                          DrawSpotLightCone;         // +0xa5
    bool                          EnableLightCache;          // +0xa6
    CEngineLocalLightArray*       CurrentShaderLocalLights;  // +0xa8
    vector<CManagerLocalLight*>   LocalLightList;            // +0xac
    vector<CManagerShadowedSpotLight*> ShadowedSpotLightList;   // +0xb8
    float                         LookupGridWorldX;          // +0xc4
    float                         LookupGridWorldY;          // +0xc8
    bool                          LocalLightsEnabled;        // +0xcc
    bool                          ShadowedSpotLightsEnabled; // +0xcd
    bool                          FadedShadowDiffuseFactorEnabled;   // +0xce
    CRGBFloatColour               MistEffectColour;          // +0xd0
    CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;    // +0xe0
    unsigned long                 StateUpdateFlags;          // +0xe4
    long                          SelfIlluminationAmbientPercentage;   // +0xe8
    CLocalLightElementLink*       LightLookupGrid[16][16];   // +0xec
    CEngineLocalLightArray*       SmallObjectLookupGrid[64][64];   // +0x4ec
    CEngineLocalLightArray        EmptyLightArray;           // +0x44ec
    CLightingSIMDVarBlock*        SIMDVarBlock;              // +0x4690
    long                          ShadowMode;                // +0x4694
    CEngineLightingManagerSpotlightParams CurrentShadowedSpotLightParams;   // +0x4698
    CEngineLightingManager();
    virtual ~CEngineLightingManager();
};

CEngineLightingManager::CEngineLightingManager()
    : CEnginePrimitiveManager((CEngineSceneArray*)g_pPrimitiveRenderer_01436e80->Scenes)
{
    memset(LightLookupGrid, 0, sizeof(LightLookupGrid));
    memset(SmallObjectLookupGrid, 0, sizeof(SmallObjectLookupGrid));
    ShadowMode = 1;
    DrawLightRadius = false;
    DrawSpotLightCone = false;
    EnableLightCache = true;
    ShaderPointLightsEnabledCount = 0;
    LocalLightsEnabled = true;
    ShadowedSpotLightsEnabled = true;
    FadedShadowDiffuseFactorEnabled = false;
    SelfIlluminationAmbientPercentage = 0;
    SIMDVarBlock = (CLightingSIMDVarBlock*)AllocateAligned(sizeof(CLightingSIMDVarBlock), 16);
    SunVector = C3DVector(0.0f, 1.0f, 0.0f);
    MoonVector = C3DVector(0.0f, 0.0f, -1.0f);
    DiffuseVector = C3DVector(0.0f, 1.0f, 0.0f);
    ThemeChannels.resize(1, CThemeChannel());
    ThemeChannels[0].DiffuseColour.Set(0.25f, 0.25f, 0.25f, 1.0f);
    ThemeChannels[0].AmbientColour.Set(0.25f, 0.25f, 0.25f, 1.0f);
    ThemeChannels[0].BacklightColour.Set(0.0f, 0.0f, 0.0f, 1.0f);
    ThemeChannels[0].FogColour.Set(0.0f, 0.0f, 0.0f, 1.0f);
    ThemeChannels[0].FogStartZ = 1000.0f;
    ThemeChannels[0].FogEndZ = 2000.0f;
    MistEffectColour.Set(0.0f, 0.0f, 0.0f, 1.0f);
    g_pEnginePrimitiveTypeInfo->RegisterPrimitiveManager(this);
    CurrentShadowedSpotLightParams.Active = false;
    StateUpdateFlags = 0xf;
}

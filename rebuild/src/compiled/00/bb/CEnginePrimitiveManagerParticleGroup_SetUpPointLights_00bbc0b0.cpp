// CEnginePrimitiveManagerParticleGroup::SetUpPointLights() -- once per frame, fetches up to five lights around the current group's bounding sphere, remembers their count and uploads them as shader point lights; returns the light count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBC0B0, length 116 bytes (+ int3 padding).
// Signature from Ego_r.pdb: ?SetUpPointLights@CEnginePrimitiveManagerParticleGroup@@QAEJXZ.
// Callees: 0x00B46B90 CEngineLightingManager::GetLightsInSphereFast(const C3DBoundingSphere&, long,
// const CEnginePrimitiveThemeChannelDesc&) -> CEngineLocalLightArray* (its first byte is the light count),
// 0x00B44F20 CEngineLightingManager::SetupShaderPointLights(CEngineLocalLightArray*), both on the global lighting
// manager @ 0x01436E9C.
// Layout: PDB CEnginePrimitiveManagerParticleGroup (0x28): CEnginePrimitiveManager {vptr, SceneList +4, Enable +8},
// PointLightsSet +0xc, PointLightCount (signed char) +0xd, CurrentWorldPos +0xe (unaligned, as the retail word loads at
// +0xe/+0x12/+0x16 show: modelled with `#pragma pack(1)` and explicit pads), CurrentBSRadius +0x1c.
// Shape: the bounding sphere is a frame temporary built by its (centre, radius) constructor (C3DVector with a
// memberwise copy constructor: direct dword loads, no `lea` block copy); the theme-channel description is a named
// local assigned from a default-constructed temporary (three zero bytes into the shared slot, then the whole-dword
// self copy `mov eax,[esp+4] ; mov [esp+4],eax`); the count is returned sign-extended (`movsx`).

struct C3DVector {                                       // memberwise user copy constructor: direct dword moves
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere(const C3DVector& center, float radius) : Center(center), Radius(radius) { }
};
struct CEnginePrimitiveThemeChannelDesc {                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CEngineLocalLightArray { signed char Count; };
class CEngineLightingManager {
public:
    CEngineLocalLightArray* GetLightsInSphereFast(const C3DBoundingSphere& sphere, long maxLights,
                                                  const CEnginePrimitiveThemeChannelDesc& channels);   // 0x00B46B90
    void SetupShaderPointLights(CEngineLocalLightArray* lights);   // 0x00B44F20
};
extern CEngineLightingManager* g_pEngineLightingManager;   // 0x01436E9C

#pragma pack(push, 1)
class CEnginePrimitiveManagerParticleGroup {
public:
    void*         vptr;                                  // +0x00
    void*         SceneList;                             // +0x04
    bool          Enable;                                // +0x08
    unsigned char _pad09[3];
    bool          PointLightsSet;                        // +0x0c
    signed char   PointLightCount;                       // +0x0d
    C3DVector     CurrentWorldPos;                       // +0x0e (unaligned)
    unsigned char _pad1a[2];
    float         CurrentBSRadius;                       // +0x1c
    bool          ClipEffectToWeatherMask;               // +0x20
    bool          ReadZBuffer;                           // +0x21
    bool          WriteZBuffer;                          // +0x22
    bool          DisplayBoundingBox;                    // +0x23
    bool          DisplayBoundingSphere;                 // +0x24
    unsigned char _pad25[3];                             // sizeof == 0x28
    long SetUpPointLights();
};
#pragma pack(pop)

long CEnginePrimitiveManagerParticleGroup::SetUpPointLights()
{
    if (!PointLightsSet) {
        CEnginePrimitiveThemeChannelDesc channels;
        channels = CEnginePrimitiveThemeChannelDesc();
        CEngineLocalLightArray* lights = g_pEngineLightingManager->GetLightsInSphereFast(
            C3DBoundingSphere(CurrentWorldPos, CurrentBSRadius), 5, channels);
        PointLightCount = lights->Count;
        g_pEngineLightingManager->SetupShaderPointLights(lights);
        PointLightsSet = true;
    }
    return PointLightCount;
}
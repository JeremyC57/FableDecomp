// CEngineWeatherRenderer::RenderRainBox(const C3DVector& boxPos) -- upload the rain box position, flush the dirty shader state and draw the rain particle buffers as indexed triangles scaled by the current rain strength
// Leaf: RenderRainBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B54100, length 262 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ?RenderRainBox@CEngineWeatherRenderer@@AAEXABVC3DVector@@@Z. The catalog row is labelled
// RenderSnowBox (propagated); this body reads CurrentRainStrength and is called only from RenderRain (0x00B54890),
// its twin 0x00B54210 (CurrentSnowStrength, point list) is the real RenderSnowBox.
// Callees: CShaderRenderManager::SetUserVertexShaderConstant(unsigned long, float, float, float, float) @ 0x00989B00 and the
// inlined CShaderRenderManager::PrepareForRender(5) (out-of-line copy @ 0x009E1280): SetVSConstants @ 0x0098B930,
// UpdatePixelShader @ 0x00988A20, UpdatePixelShaderConstants @ 0x00989BF0, UpdateAmbient @ 0x00989760, UpdateFogColour
// @ 0x009897C0, UpdateLightGlobals @ 0x0098A760, UpdateCombinedProjectionTransform @ 0x00988A50, UpdateCameraPos
// @ 0x00989700 (named from the PDB private Update* list by the dirty bit each clears); __ftol2 @ 0x00BFEA70;
// CRenderManagerCore::DrawCurrentBuffersAsIndexedTris(unsigned long x4) @ 0x00A0AC60.
// Globals: the shader render manager @ 0x01436E14, the render core @ 0x01436E18.
// Layout: engine/CEngineWeatherRenderer.h for `this` (Settings +0x114: RainVolumeDepthSelectMultiplier +0x34,
// CurrentRainStrength +0x4b4, PDB CEngineWeatherSettings). CShaderRenderManager is the retail layout (PDB minus 0x20
// past the 12-byte CArrays): CurrentVSConstantLayout +4, WorldMatrixIsIdentity +0x1e8, UpdateFlags +0x3d8;
// CVertexShaderConstantLayout: Reserved.Offset +8, ReservedPresets CArray<C4DVector> +0x140.
// Shape: PrepareForRender(flags) is inlined into this TU (`__forceinline` in the model; plain inline stays a call) and
// folds for the constant 5 (bits 1 and 4): the reserved-preset
// upload, the pixel-shader pair, the ambient/fog/light-globals updates, the combined projection and the camera
// position; the CArray data is the `size ? first : (T*)this` GetData select. The two counts are truncated with
// __ftol2 right to left (triangles = strength * 256 first, vertices = strength * 512 second).
#include "engine/CEngineWeatherRenderer.h"

struct C3DVector { float X, Y, Z; };
struct C4DVector { float X, Y, Z, W; };

template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};

struct CShaderConstantRange {
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};

struct CVertexShaderConstantLayout {
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};

class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00

    __forceinline void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];                       // +0x008
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];                       // +0x1e9
    unsigned long UpdateFlags;                          // +0x3d8

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    void ResetWorldTransform();                         // 0x00988290
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14

class CRenderManagerCore {
public:
    void DrawCurrentBuffersAsIndexedTris(unsigned long minVertexIndex, unsigned long vertexCount,
                                         unsigned long startIndex, unsigned long triangleCount);   // 0x00A0AC60
};
extern CRenderManagerCore* g_pRenderManagerCore;        // 0x01436E18

struct CEngineWeatherSettingsView {                     // CEngineWeatherSettings (PDB, 0x558 bytes)
    unsigned char _pad00[0x34];
    float RainVolumeDepthSelectMultiplier;              // +0x34
    unsigned char _pad38[0x47c];
    float CurrentRainStrength;                          // +0x4b4
    float CurrentSnowStrength;                          // +0x4b8
};

struct CEngineWeatherRendererMethods : CEngineWeatherRenderer {
    CEngineWeatherSettingsView& GetSettings() { return *reinterpret_cast<CEngineWeatherSettingsView*>(Settings); }
    void RenderRainBox(const C3DVector& boxPos);
};

void CEngineWeatherRendererMethods::RenderRainBox(const C3DVector& boxPos)
{
    g_pShaderRenderManager->SetUserVertexShaderConstant(0, boxPos.X, boxPos.Y, boxPos.Z,
                                                        GetSettings().RainVolumeDepthSelectMultiplier);
    g_pShaderRenderManager->PrepareForRender(5);
    g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, (long)(GetSettings().CurrentRainStrength * 512.0f), 0,
                                                          (long)(GetSettings().CurrentRainStrength * 256.0f));
}
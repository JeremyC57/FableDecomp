// CEngineSkyRenderer::RenderScreenSpaceSprite(CTexture* texture, C2DVector topLeft, C2DVector size, CRGBColour colour) -- draw a screen-space sky sprite: convert the rectangle's corners from screen to projection space through the engine camera, build four pre-transformed coloured vertices at depth 0.9999, then the screen-space sprite shaders, PrepareForRender(0) and DrawAsIndexedTris
// Leaf: RenderScreenSpaceSprite
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B63530, `ret 0x18`, /GS cookie (0x0139C8A8, check 0x00BFE9F9).
// Callees: CEngineCamera::ScreenToProjection(C2DVector&) 0x00B2FB70 on the camera @ 0x01436EA0, ApplyVertexShader 0x00988020,
//   ApplyPixelShader 0x00988140, the inlined PrepareForRender(0) (0x00B54100 model), DrawAsIndexedTris 0x00A0AEA0.
// Layout: engine/CEngineSkyRenderer.h (ScreenSpaceSpriteVertexShader +0x140, ScreenSpaceSpritePixelShader +0x148).
#include "engine/CEngineSkyRenderer.h"

class C2DVector { public: float X, Y; };
class C4DVector { public: float X, Y, Z, W; };
class CRGBColour { public:                                      // D3DCOLOR byte order
    unsigned char B, G, R, A;
    unsigned long GetARGB() const { return (((((A << 8) + R) << 8) + G) << 8) + B; }
};
class CTexture;
struct SScreenSpriteVertex {                             // 0x20 bytes, pre-transformed
    float         X, Y, Z, W;
    unsigned long Colour;
    unsigned long _unused;
    float         U, V;
};
struct CEngineCameraView {
    void ScreenToProjection(C2DVector& pos) const;      // 0x00B2FB70
};
extern CEngineCameraView* g_pEngineCamera;              // 0x01436EA0
struct CRenderManagerCoreView {
    void DrawAsIndexedTris(CTexture** textures, unsigned long textureCount, const void* vertices, long stride,
                           long vertexCount, const void* indices, long triangleCount);   // 0x00A0AEA0
};
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18
template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
class CShaderConstantRange { public:
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};
class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};
class CVertexShader;
class CPixelShader;

class CShaderRenderManager {
public:
    void ApplyVertexShader(CVertexShader* shader, bool force);                  // 0x00988020
    void ApplyPixelShader(CPixelShader* shader);                                // 0x00988140
    void SetUserVertexShaderConstant(unsigned long index, const C4DVector* value);   // 0x00989B50

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
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
public:
    void ResetWorldTransform();   // 0x00988290
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14

struct CEngineSkyRenderer_Methods : CEngineSkyRenderer {
    void RenderScreenSpaceSprite(CTexture* texture, C2DVector topLeft, C2DVector size, CRGBColour colour);
};

void CEngineSkyRenderer_Methods::RenderScreenSpaceSprite(CTexture* texture, C2DVector topLeft, C2DVector size, CRGBColour colour)
{
    C2DVector bottomRight;
    bottomRight.X = size.X + topLeft.X;
    bottomRight.Y = size.Y + topLeft.Y;
    unsigned long argb = colour.GetARGB();
    C2DVector corner = topLeft;
    g_pEngineCamera->ScreenToProjection(corner);
    g_pEngineCamera->ScreenToProjection(bottomRight);
    SScreenSpriteVertex verts[4];
    verts[0].X = corner.X;
    verts[0].Y = corner.Y;
    verts[0].Z = 0.9999f;
    verts[0].U = 0.0f;
    verts[0].V = 0.0f;
    verts[0].Colour = argb;
    verts[0].W = 1.0f;
    verts[1].X = bottomRight.X;
    verts[1].Y = corner.Y;
    verts[1].Z = 0.9999f;
    verts[1].U = 1.0f;
    verts[1].V = 0.0f;
    verts[1].Colour = argb;
    verts[1].W = 1.0f;
    verts[2].X = bottomRight.X;
    verts[2].Y = bottomRight.Y;
    verts[2].Z = 0.9999f;
    verts[2].U = 1.0f;
    verts[2].V = 1.0f;
    verts[2].Colour = argb;
    verts[2].W = 1.0f;
    verts[3].X = corner.X;
    verts[3].Y = bottomRight.Y;
    verts[3].Z = 0.9999f;
    verts[3].U = 0.0f;
    verts[3].V = 1.0f;
    verts[3].Colour = argb;
    verts[3].W = 1.0f;
    unsigned short indices[6];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
    g_pShaderRenderManager->ApplyVertexShader((CVertexShader*)ScreenSpaceSpriteVertexShader, false);
    g_pShaderRenderManager->ApplyPixelShader((CPixelShader*)ScreenSpaceSpritePixelShader);
    g_pShaderRenderManager->PrepareForRender(0);
    g_pRenderManagerCore->DrawAsIndexedTris(&texture, 1, verts, 0x20, 4, indices, 2);
}
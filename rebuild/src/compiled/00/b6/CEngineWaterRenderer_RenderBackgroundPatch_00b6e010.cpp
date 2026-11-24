// CEngineWaterRenderer::RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch) -- draw one background water sub-patch: a camera-centred world transform, the inlined PrepareForRender(1), the patch's vertex/index buffers and its triangles, then each of the four bridge strips that has a vertex buffer
// Leaf: ?RenderBackgroundPatch@CEngineWaterRenderer@@AAEXPAVCEngineWaterBackgroundSubPatch@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6E010, length 592 bytes (+ int3 padding), `ret 4` (`this` unused).
// Callees: CShaderRenderManager::SetWorldTransform @ 0x009881F0, the PrepareForRender inline of the landed 0x00B7F600,
//   IDirect3DDevice9::SetStreamSource / SetIndices (slots 100/104) through the render core's inline helpers of the landed
//   RenderSky 0x00B66190, CRenderManagerCore::DrawCurrentBuffersAsIndexedTris @ 0x00A0AC60. Globals: shader render manager
//   @ 0x01436E14, render core @ 0x01436E18, camera @ 0x01436EA0 (WorldPosition +0x54).
// Layout: engine/CEngineWaterBackgroundSubPatch.h (IndexBuffer +8, VertexBuffer +0xc, bridge buffers +0x10/+0x20, counts
//   +0x30/+0x32/+0x34); the vertex buffer through the PDB CVertexBufferBase fields (PVertexBuffer +4, Length +8, Stride +0x10).
// Shape: the world matrix is 0x00B7C510's (members written translation/off-diagonal first, diagonal last); a bridge strip's
//   vertex count is its byte length divided (signed) by the stride.
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore at offset 0): the buffer binds
//   are its PDB inlines SelectVertexBuffer(0, buffer) / SelectIndexBuffer(buffer, 0) (PD3DDevice +0x3CF0, PSelectedIndexBuffer +0x3CF4)
//   on the shared CVertexBufferWin32 / CIndexBuffer (a bridge strip's vertex count is the inline CVertexBufferBase::GetSize() over the
//   stride), and the draw ?DrawCurrentBuffersAsIndexedTris@CRenderManagerCore@@QAEXKKKK@Z 0x00A0AC60. Was g_pRenderManagerCore with
//   local CRenderManagerCore (SetVertexBuffer / SetIndexBuffer) / IDirect3DDevice9 / CVertexBufferWin32 / CIndexBuffer models.
#include "engine/CRenderManager.h"
#include "engine/CEngineWaterBackgroundSubPatch.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

class CRGBFloatColour;
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
    inline C4DVector(const CRGBFloatColour& c);
};
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
inline CRGBFloatColour operator*(const CRGBFloatColour& a, const CRGBFloatColour& b) { return CRGBFloatColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A); }
inline C4DVector::C4DVector(const CRGBFloatColour& c) : X(c.R), Y(c.G), Z(c.B), W(c.A) { }
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }
};
class CMatrix3x4 { public:
    float M[9];                                         // rotation rows
    C3DVector Pos;                                      // translation
    CMatrix3x4() { }
    CMatrix3x4(const C3DVector& xAxis, const C3DVector& yAxis, const C3DVector& zAxis, const C3DVector& pos)
    {
        Pos.Z = pos.Z; Pos.Y = pos.Y; Pos.X = pos.X;
        M[7] = zAxis.Y; M[6] = zAxis.X;
        M[5] = yAxis.Z; M[3] = yAxis.X;
        M[2] = xAxis.Z; M[1] = xAxis.Y;
        M[8] = zAxis.Z; M[4] = yAxis.Y; M[0] = xAxis.X;
    }
};
struct CPreTransposedBoneMatrix {
    float M[12];
    CPreTransposedBoneMatrix() { }
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
    void resize(unsigned int n, const T& value);        // 0x00B7EDF0 for CPreTransposedBoneMatrix
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

class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void SetBoneTransform(unsigned long index, const CMatrix3x4* matrix);                        // 0x0098A7F0
    void SetWorldTransform(const CMatrix3x4& matrix);                                           // 0x009881F0
    void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
        *(PSConstTable.First + index) = value;
        UpdateFlags |= 2;
    }

    FABLE_FORCEINLINE void PrepareForRender(unsigned long flags)
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
    unsigned char _pad1e9[0x1e3];                       // +0x1e9
    CArray<C4DVector> PSConstTable;                     // +0x3cc
    unsigned long UpdateFlags;                          // +0x3d8
    CArray<CPreTransposedBoneMatrix> BoneMatrices;      // +0x3dc

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
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14

class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                        // +0x54
};
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                  // 0x01436EA0

class CEngineWaterRenderer { public:
    private:
    void RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch);
    public:
};

void CEngineWaterRenderer::RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch)
{
    CMatrix3x4 world(C3DVector(1.0f, 0.0f, 0.0f), C3DVector(0.0f, 1.0f, 0.0f), C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 0.0f, 0.0f));
    world.Pos = NEngineInternal::GPCamera->WorldPosition;
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(world);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, patch->VertexBuffer);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(patch->IndexBuffer, 0);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, patch->VertexCount, 0, patch->PolyCount);
    for (long i = 0; i < 4; ++i) {
        if (patch->BridgeVertexBuffers[i]) {
            NEngineInternal::GPRenderManager->SelectVertexBuffer(0, patch->BridgeVertexBuffers[i]);
            NEngineInternal::GPRenderManager->SelectIndexBuffer(patch->BridgeIndexBuffers[i], 0);
            NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, patch->BridgeVertexBuffers[i]->GetSize() / patch->BridgeVertexBuffers[i]->Stride,
                                                                  0, patch->BridgePolyCount[i]);
        }
    }
}
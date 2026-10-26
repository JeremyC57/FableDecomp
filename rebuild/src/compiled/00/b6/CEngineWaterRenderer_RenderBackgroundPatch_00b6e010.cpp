// CEngineWaterRenderer::RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch) -- draw one background water sub-patch: a camera-centred world transform, the inlined PrepareForRender(1), the patch's vertex/index buffers and its triangles, then each of the four bridge strips that has a vertex buffer
// Leaf: RenderBackgroundPatch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6E010, length 592 bytes (+ int3 padding), `ret 4` (`this` unused).
// Callees: CShaderRenderManager::SetWorldTransform @ 0x009881F0, the PrepareForRender inline of the landed 0x00B7F600,
//   IDirect3DDevice9::SetStreamSource / SetIndices (slots 100/104) through the render core's inline helpers of the landed
//   RenderSky 0x00B66190, CRenderManagerCore::DrawCurrentBuffersAsIndexedTris @ 0x00A0AC60. Globals: shader render manager
//   @ 0x01436E14, render core @ 0x01436E18, camera @ 0x01436EA0 (WorldPosition +0x54).
// Layout: engine/CEngineWaterBackgroundSubPatch.h (IndexBuffer +8, VertexBuffer +0xc, bridge buffers +0x10/+0x20, counts
//   +0x30/+0x32/+0x34); the vertex buffer through the PDB CVertexBufferBase fields (PVertexBuffer +4, Length +8, Stride +0x10).
// Shape: the world matrix is 0x00B7C510's (members written translation/off-diagonal first, diagonal last); a bridge strip's
//   vertex count is its byte length divided (signed) by the stride.
#include "engine/CEngineWaterBackgroundSubPatch.h"

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
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14

struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;

struct IDirect3DDevice9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual void __stdcall v30(); virtual void __stdcall v34(); virtual void __stdcall v38(); virtual void __stdcall v3c();
    virtual void __stdcall v40(); virtual void __stdcall v44(); virtual void __stdcall v48(); virtual void __stdcall v4c();
    virtual void __stdcall v50(); virtual void __stdcall v54(); virtual void __stdcall v58(); virtual void __stdcall v5c();
    virtual void __stdcall v60(); virtual void __stdcall v64(); virtual void __stdcall v68(); virtual void __stdcall v6c();
    virtual void __stdcall v70(); virtual void __stdcall v74(); virtual void __stdcall v78(); virtual void __stdcall v7c();
    virtual void __stdcall v80(); virtual void __stdcall v84(); virtual void __stdcall v88(); virtual void __stdcall v8c();
    virtual void __stdcall v90(); virtual void __stdcall v94(); virtual void __stdcall v98(); virtual void __stdcall v9c();
    virtual void __stdcall va0(); virtual void __stdcall va4(); virtual void __stdcall va8(); virtual void __stdcall vac();
    virtual void __stdcall vb0(); virtual void __stdcall vb4(); virtual void __stdcall vb8(); virtual void __stdcall vbc();
    virtual void __stdcall vc0(); virtual void __stdcall vc4(); virtual void __stdcall vc8(); virtual void __stdcall vcc();
    virtual void __stdcall vd0(); virtual void __stdcall vd4(); virtual void __stdcall vd8(); virtual void __stdcall vdc();
    virtual void __stdcall ve0(); virtual void __stdcall ve4(); virtual void __stdcall ve8(); virtual void __stdcall vec();
    virtual void __stdcall vf0(); virtual void __stdcall vf4(); virtual void __stdcall vf8(); virtual void __stdcall vfc();
    virtual void __stdcall v100(); virtual void __stdcall v104(); virtual void __stdcall v108(); virtual void __stdcall v10c();
    virtual void __stdcall v110(); virtual void __stdcall v114(); virtual void __stdcall v118(); virtual void __stdcall v11c();
    virtual void __stdcall v120(); virtual void __stdcall v124(); virtual void __stdcall v128(); virtual void __stdcall v12c();
    virtual void __stdcall v130(); virtual void __stdcall v134(); virtual void __stdcall v138(); virtual void __stdcall v13c();
    virtual void __stdcall v140(); virtual void __stdcall v144(); virtual void __stdcall v148(); virtual void __stdcall v14c();
    virtual void __stdcall v150(); virtual void __stdcall v154(); virtual void __stdcall v158(); virtual void __stdcall v15c();
    virtual void __stdcall v160(); virtual void __stdcall v164(); virtual void __stdcall v168(); virtual void __stdcall v16c();
    virtual void __stdcall v170(); virtual void __stdcall v174(); virtual void __stdcall v178(); virtual void __stdcall v17c();
    virtual void __stdcall v180(); virtual void __stdcall v184(); virtual void __stdcall v188(); virtual void __stdcall v18c();
    virtual long __stdcall SetStreamSource(unsigned long stream, IDirect3DVertexBuffer9* data, unsigned long offset, unsigned long stride);   // slot 100 (+0x190)
    virtual void __stdcall v194(); virtual void __stdcall v198(); virtual void __stdcall v19c();
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indices);   // slot 104 (+0x1A0)
};

class CVertexBufferWin32 { public:                        // PDB CVertexBufferBase fields of CVertexBufferWin32
    void*                   _vptr;                 // +0x00
    IDirect3DVertexBuffer9* PVertexBuffer;         // +0x04
    long                    Length;                // +0x08 (bytes; divided signed by the stride)
    long                    Type;                  // +0x0c
    unsigned char           Stride;                // +0x10
};
class CIndexBuffer { public:
    void*                  _vptr;                  // +0x00
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
};

struct CRenderManagerCore {                         // the render manager core @ 0x01436E18 (Ego_r.pdb offsets)
    unsigned char     _pad0000[0x3cf0];
    IDirect3DDevice9* PD3DDevice;                       // +0x3CF0
    CIndexBuffer*     PSelectedIndexBuffer;             // +0x3CF4

    void DrawCurrentBuffersAsIndexedTris(unsigned long minIndex, unsigned long vertexCount,
                                         unsigned long startIndex, unsigned long triangleCount);   // 0x00A0AC60
    __forceinline void SetVertexBuffer(CVertexBufferWin32* buffer)
    {
        if (buffer)
            PD3DDevice->SetStreamSource(0, buffer->PVertexBuffer, 0, buffer->Stride);
        else
            PD3DDevice->SetStreamSource(0, 0, 0, 0);
    }
    __forceinline void SetIndexBuffer(CIndexBuffer* buffer)
    {
        PSelectedIndexBuffer = buffer;
        if (buffer)
            PD3DDevice->SetIndices(buffer->PD3DIndexBuffer);
        else
            PD3DDevice->SetIndices(0);
    }
};
extern CRenderManagerCore* g_pRenderManagerCore;        // 0x01436E18

class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                        // +0x54
};
extern CEngineCamera* g_pEngineCamera;                  // 0x01436EA0

class CEngineWaterRenderer { public:
    void RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch);
};

void CEngineWaterRenderer::RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch)
{
    CMatrix3x4 world(C3DVector(1.0f, 0.0f, 0.0f), C3DVector(0.0f, 1.0f, 0.0f), C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 0.0f, 0.0f));
    world.Pos = g_pEngineCamera->WorldPosition;
    g_pShaderRenderManager->SetWorldTransform(world);
    g_pShaderRenderManager->PrepareForRender(1);
    g_pRenderManagerCore->SetVertexBuffer(patch->VertexBuffer);
    g_pRenderManagerCore->SetIndexBuffer(patch->IndexBuffer);
    g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, patch->VertexCount, 0, patch->PolyCount);
    for (long i = 0; i < 4; ++i) {
        if (patch->BridgeVertexBuffers[i]) {
            g_pRenderManagerCore->SetVertexBuffer(patch->BridgeVertexBuffers[i]);
            g_pRenderManagerCore->SetIndexBuffer(patch->BridgeIndexBuffers[i]);
            g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, patch->BridgeVertexBuffers[i]->Length / patch->BridgeVertexBuffers[i]->Stride,
                                                                  0, patch->BridgePolyCount[i]);
        }
    }
}
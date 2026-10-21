// Grade: DIFFER(456v464) -- loop register allocation: retail keeps the primitive in ebp (pushed in the prologue), the strip flag in bl then the stride in ebx and homes the index buffer in the primitive arg slot; ours swaps ebx/ebp and spills differently (tried a direct body, an inlined CShaderRenderManager draw helper with two parameter orders, block by value/by reference, a cached block count)
// CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive(C3DPrimitive2* primitive, const CMatrix3x4& transform) -- draw every static block of a mesh primitive with the manager's occlusion vertex shader and no textures: shader and world transform set, the dirty shader state flushed, then one indexed draw per block
// Leaf: RenderUntexturedStaticMeshPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B903D0, length 464 bytes (body 0x1d0), `ret 8`.
// Callees: CShaderRenderManager::ApplyVertexShader(const CVertexShader&, bool) 0x00988020, CShaderRenderManager::
//   SetWorldTransform 0x009881F0 / CRenderManagerCore::SetWorldTransform 0x00A0B140 (by FixedFunctionVertexMode), the
//   inlined CShaderRenderManager::PrepareForRender(1) (model of the landed RenderRainBox 0x00B54100: SetVSConstants
//   0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760,
//   UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateCombinedProjectionTransform 0x00988A50),
//   CRenderStateManager::RealiseRenderState 0x00A058C0; IDirect3DDevice9 SetStreamSource (+0x190), SetIndices (+0x1a0),
//   DrawIndexedPrimitive (+0x148).
// Globals: the shader render manager @ 0x01436E14, the render core @ 0x01436E18.
// Layout: CShaderRenderManager retail offsets (PDB less 0x10 here): RenderManager +0x1d0, D3DDevice +0x1d4,
//   FixedFunctionVertexMode +0x395, UpdateFlags +0x3d8 (as the landed RenderRainBox). C3DPrimitive2 retail (PDB less 4
//   past the 12-byte ClothPrimitives CArray): VertexBuffer +0x30, IndexBuffer +0x34, StaticBlockCount +0x40,
//   StaticBlocks +0x44 (0x20-byte CStaticBlock whose base holds count/start/strip), VertexCount +0x58.
//   CVertexBufferWin32: D3D buffer +4, stride byte +0x10; CIndexBuffer: D3D buffer +4, PrimitiveType +0xc.
//   `this` (PDB CEnginePrimitiveManagerOcclusionBase): VertexShader +0xc; no engine header exists for it.
// Shape: the block's base is copied by value; its strip flag maps to D3DPT_TRIANGLESTRIP (5) / TRIANGLELIST (4) and an
//   unset type (-1) falls back to the index buffer's own type.
struct CMatrix3x4;
struct CVertexShader { unsigned char _body[8]; };
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct CVertexBufferWin32;
struct CIndexBuffer;

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
    virtual void __stdcall v140();
    virtual long __stdcall DrawPrimitive(long primitiveType, unsigned long startVertex, unsigned long primitiveCount);   // slot 81 (+0x144)
    virtual long __stdcall DrawIndexedPrimitive(long primitiveType, long baseVertexIndex, unsigned long minVertexIndex,
                                                unsigned long numVertices, unsigned long startIndex, unsigned long primitiveCount);   // slot 82 (+0x148)
    virtual long __stdcall DrawPrimitiveUP(long primitiveType, unsigned long primitiveCount, const void* vertices, unsigned long stride);   // slot 83 (+0x14C)
    virtual long __stdcall DrawIndexedPrimitiveUP(long primitiveType, unsigned long minVertexIndex, unsigned long numVertices,
                                                  unsigned long primitiveCount, const void* indices, long indexFormat,
                                                  const void* vertices, unsigned long stride);   // slot 84 (+0x150)
    virtual void __stdcall v154(); virtual void __stdcall v158(); virtual void __stdcall v15c(); virtual void __stdcall v160();
    virtual void __stdcall v164(); virtual void __stdcall v168(); virtual void __stdcall v16c(); virtual void __stdcall v170();
    virtual void __stdcall v174(); virtual void __stdcall v178(); virtual void __stdcall v17c(); virtual void __stdcall v180();
    virtual void __stdcall v184(); virtual void __stdcall v188(); virtual void __stdcall v18c();
    virtual long __stdcall SetStreamSource(unsigned long stream, IDirect3DVertexBuffer9* data, unsigned long offset, unsigned long stride);   // slot 100 (+0x190)
    virtual void __stdcall v194(); virtual void __stdcall v198(); virtual void __stdcall v19c();
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indices);   // slot 104 (+0x1a0)
};


template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
struct C4DVector { float X, Y, Z, W; };
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

struct CRenderStateManager {
    void RealiseRenderState();                          // 0x00A058C0
};

class CShaderRenderManager {
public:
    void ApplyVertexShader(const CVertexShader& shader, bool force);   // 0x00988020
    void SetWorldTransform(const CMatrix3x4& transform);               // 0x009881F0

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

    __forceinline void DrawIndexed(CVertexBufferWin32* vertices, CIndexBuffer* indices, long type, unsigned long vertexCount,
                                   unsigned long startIndex, unsigned long primitiveCount);

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1c8];                       // +0x008
    CRenderStateManager* RenderManager;                 // +0x1d0
    IDirect3DDevice9* D3DDevice;                        // +0x1d4
    unsigned char _pad1d8[0x10];                        // +0x1d8
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x395 - 0x1e9];               // +0x1e9
    bool FixedFunctionVertexMode;                       // +0x395
    unsigned char _pad396[0x3d8 - 0x396];               // +0x396
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
    void SetWorldTransform(const CMatrix3x4& transform);    // 0x00A0B140
};
extern CRenderManagerCore* g_pRenderManagerCore;        // 0x01436E18

struct CVertexBufferWin32 {
    void*                   _vptr;                      // +0x00
    IDirect3DVertexBuffer9* PD3DVertexBuffer;           // +0x04
    unsigned char           _pad08[8];
    unsigned char           Stride;                     // +0x10
};
struct CIndexBuffer {
    void*                  _vptr;                       // +0x00
    IDirect3DIndexBuffer9* PD3DIndexBuffer;             // +0x04
    unsigned long          ByteLength;                  // +0x08
    long                   PrimitiveType;               // +0x0c EPrimitiveType
};

__forceinline void CShaderRenderManager::DrawIndexed(CVertexBufferWin32* vertices, CIndexBuffer* indices, long type,
                                                     unsigned long vertexCount, unsigned long startIndex, unsigned long primitiveCount)
{
    D3DDevice->SetStreamSource(0, vertices->PD3DVertexBuffer, 0, vertices->Stride);
    D3DDevice->SetIndices(indices->PD3DIndexBuffer);
    if (type == -1)
        type = indices->PrimitiveType;
    RenderManager->RealiseRenderState();
    D3DDevice->DrawIndexedPrimitive(type, 0, 0, vertexCount, startIndex, primitiveCount);
}

struct CPrimitiveBlock {                                // the CStaticBlock base (12 bytes)
    unsigned long PrimitiveCount;                       // +0x00
    unsigned long StartIndex;                           // +0x04
    bool          IsStrip;                              // +0x08
};
struct CStaticBlock : CPrimitiveBlock {                 // 0x20 bytes
    unsigned char _pad0c[0x14];
};

struct C3DPrimitive2 {
    unsigned char       _pad00[0x30];
    CVertexBufferWin32* VertexBuffer;                   // +0x30
    CIndexBuffer*       IndexBuffer;                    // +0x34
    unsigned char       _pad38[8];
    unsigned long       StaticBlockCount;               // +0x40
    CStaticBlock*       StaticBlocks;                   // +0x44
    unsigned char       _pad48[0x10];
    unsigned long       VertexCount;                    // +0x58
};

struct CEnginePrimitiveManagerOcclusionBase {
    unsigned char _pad00[0xc];
    CVertexShader VertexShader;                         // +0x0c
    void RenderUntexturedStaticMeshPrimitive(C3DPrimitive2* primitive, const CMatrix3x4& transform);
};

void CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive(C3DPrimitive2* primitive, const CMatrix3x4& transform)
{
    g_pShaderRenderManager->ApplyVertexShader(VertexShader, false);
    if (g_pShaderRenderManager->FixedFunctionVertexMode)
        g_pRenderManagerCore->SetWorldTransform(transform);
    else
        g_pShaderRenderManager->SetWorldTransform(transform);
    g_pShaderRenderManager->PrepareForRender(1);
    unsigned long count = primitive->StaticBlockCount;
    unsigned long vertexCount = primitive->VertexCount;
    for (unsigned long i = 0; i < count; ++i) {
        const CPrimitiveBlock& block = primitive->StaticBlocks[i];
        g_pShaderRenderManager->DrawIndexed(primitive->VertexBuffer, primitive->IndexBuffer, block.IsStrip ? 5 : 4, vertexCount,
                                            block.StartIndex, block.PrimitiveCount);
    }
}

// Grade: DIFFER(329v336) -- the inline indexed draw: retail homes primitive count / start index / vertex count in frame words and keeps the shader manager in ebx (device pointer in edi, index buffer in ebp); ours keeps them in registers and re-reads the manager from the frame; the primitive pointer is in esi (ours edi)
// CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4* boneMatrices) -- for every animated block: gather the block's bone matrices into the manager's palette, upload them to the bone vertex-shader constant range, bind the primitive's index buffer and draw the block's triangles (strip or list)
// Leaf: RenderPrimitiveBlocksIndexBuffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCFB00, length 336 bytes (body 0x150), `ret 8`.
// PDB: ?RenderPrimitiveBlocksIndexBuffer@CEnginePrimitiveManagerAnimatedMeshesBase@@AAEXAAVC3DPrimitive2@@PBVCMatrix4x4@@@Z.
// Callees: CShaderRenderManager::SetVSConstants(long, const void*, unsigned long) @ 0x0098B930 (global @ 0x01436E14),
//   ~CBaseClass @ 0x0099A2E0 (the copied CShaderConstantRange, vtable 0x0129A04C), CRenderStateManager::RealiseRenderState
//   @ 0x00A058C0, IDirect3DDevice9::SetIndices (+0x1A0) and DrawIndexedPrimitive (+0x148).
// Globals: the engine @ 0x01436E98 (bone constant range at +0x2e50), the shader render manager @ 0x01436E14 (its render
//   state manager +0x1d0 and device +0x1d4).
// Layout: C3DPrimitive2 (Ego_r.pdb less 4 after the 12-byte CArray): IndexBuffer +0x34, AnimatedBlockCount +0x48,
//   AnimatedBlocks +0x4c (0x30-byte CAnimatedBlock: primitive count +0, start index +4, strip flag +8, GroupCount +0x17,
//   Groups[] +0x18), VertexCount +0x58; CIndexBuffer: PD3DIndexBuffer +4, PrimitiveType +0xc; the manager's bone palette
//   +0x330 (48-byte matrices copied from the 64-byte CMatrix4x4 array).
// Shape: an inline draw helper with a defaulted primitive type (-1 = the index buffer's own) that binds the indices, realises
//   the render state and draws; the bone constant range is copied into a local (its implicit derived dtor runs).
struct CMatrix4x4 { float M[16]; };
struct CPreTransposedBoneMatrix { float M[12]; };
struct IDirect3DIndexBuffer9;

struct IDirect3DDevice9 {
    virtual void __stdcall v00(); virtual void __stdcall v01(); virtual void __stdcall v02(); virtual void __stdcall v03();
    virtual void __stdcall v04(); virtual void __stdcall v05(); virtual void __stdcall v06(); virtual void __stdcall v07();
    virtual void __stdcall v08(); virtual void __stdcall v09(); virtual void __stdcall v10(); virtual void __stdcall v11();
    virtual void __stdcall v12(); virtual void __stdcall v13(); virtual void __stdcall v14(); virtual void __stdcall v15();
    virtual void __stdcall v16(); virtual void __stdcall v17(); virtual void __stdcall v18(); virtual void __stdcall v19();
    virtual void __stdcall v20(); virtual void __stdcall v21(); virtual void __stdcall v22(); virtual void __stdcall v23();
    virtual void __stdcall v24(); virtual void __stdcall v25(); virtual void __stdcall v26(); virtual void __stdcall v27();
    virtual void __stdcall v28(); virtual void __stdcall v29(); virtual void __stdcall v30(); virtual void __stdcall v31();
    virtual void __stdcall v32(); virtual void __stdcall v33(); virtual void __stdcall v34(); virtual void __stdcall v35();
    virtual void __stdcall v36(); virtual void __stdcall v37(); virtual void __stdcall v38(); virtual void __stdcall v39();
    virtual void __stdcall v40(); virtual void __stdcall v41(); virtual void __stdcall v42(); virtual void __stdcall v43();
    virtual void __stdcall v44(); virtual void __stdcall v45(); virtual void __stdcall v46(); virtual void __stdcall v47();
    virtual void __stdcall v48(); virtual void __stdcall v49(); virtual void __stdcall v50(); virtual void __stdcall v51();
    virtual void __stdcall v52(); virtual void __stdcall v53(); virtual void __stdcall v54(); virtual void __stdcall v55();
    virtual void __stdcall v56(); virtual void __stdcall v57(); virtual void __stdcall v58(); virtual void __stdcall v59();
    virtual void __stdcall v60(); virtual void __stdcall v61(); virtual void __stdcall v62(); virtual void __stdcall v63();
    virtual void __stdcall v64(); virtual void __stdcall v65(); virtual void __stdcall v66(); virtual void __stdcall v67();
    virtual void __stdcall v68(); virtual void __stdcall v69(); virtual void __stdcall v70(); virtual void __stdcall v71();
    virtual void __stdcall v72(); virtual void __stdcall v73(); virtual void __stdcall v74(); virtual void __stdcall v75();
    virtual void __stdcall v76(); virtual void __stdcall v77(); virtual void __stdcall v78(); virtual void __stdcall v79();
    virtual void __stdcall v80(); virtual void __stdcall v81();
    virtual long __stdcall DrawIndexedPrimitive(long primitiveType, long baseVertexIndex, unsigned long minVertexIndex,
                                                unsigned long numVertices, unsigned long startIndex, unsigned long primitiveCount);   // slot 82
    virtual void __stdcall v83(); virtual void __stdcall v84(); virtual void __stdcall v85(); virtual void __stdcall v86();
    virtual void __stdcall v87(); virtual void __stdcall v88(); virtual void __stdcall v89(); virtual void __stdcall v90();
    virtual void __stdcall v91(); virtual void __stdcall v92(); virtual void __stdcall v93(); virtual void __stdcall v94();
    virtual void __stdcall v95(); virtual void __stdcall v96(); virtual void __stdcall v97(); virtual void __stdcall v98();
    virtual void __stdcall v99(); virtual void __stdcall v100(); virtual void __stdcall v101(); virtual void __stdcall v102();
    virtual void __stdcall v103();
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indices);   // slot 104 (+0x1A0)
};

struct CIndexBuffer {
    void*                  _vptr;
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
    unsigned long          ByteLength;             // +0x08
    long                   PrimitiveType;          // +0x0c
};

class CRenderStateManager {
public:
    void RealiseRenderState();                     // 0x00A058C0
};

class CBaseClass {
public:
    ~CBaseClass();                                 // 0x0099A2E0
};
class CShaderConstantRange : public CBaseClass {   // vtable 0x0129A04C
public:
    long          Offset;                          // +0x04
    unsigned long Count;                           // +0x08
    virtual void v00();
};

struct CEngineGlobal_01436e98 {
    unsigned char        _pad[0x2e50];
    CShaderConstantRange BoneRange;                // +0x2e50
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

class CShaderRenderManager {
public:
    unsigned char        _pad[0x1d0];
    CRenderStateManager* RenderStateManager;       // +0x1d0
    IDirect3DDevice9*    PD3DDevice;               // +0x1d4
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    __forceinline void DrawIndexed(CIndexBuffer* indices, unsigned long numVertices, unsigned long startIndex,
                                   unsigned long primitiveCount, long primitiveType = -1)
    {
        PD3DDevice->SetIndices(indices->PD3DIndexBuffer);
        if (primitiveType == -1)
            primitiveType = indices->PrimitiveType;
        RenderStateManager->RealiseRenderState();
        PD3DDevice->DrawIndexedPrimitive(primitiveType, 0, 0, numVertices, startIndex, primitiveCount);
    }
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

struct CAnimatedBlock {                            // 0x30 bytes
    unsigned long PrimitiveCount;                  // +0x00
    unsigned long StartIndex;                      // +0x04
    bool          StripFlag;                       // +0x08
    unsigned char _pad09[0x0e];
    unsigned char GroupCount;                      // +0x17
    unsigned char Groups[18];                      // +0x18
    void*         Data;                            // +0x2c
};

struct C3DPrimitive2 {
    unsigned char   _pad00[0x34];
    CIndexBuffer*   IndexBuffer;                   // +0x34
    unsigned char   _pad38[0x10];
    unsigned long   AnimatedBlockCount;            // +0x48
    CAnimatedBlock* AnimatedBlocks;                // +0x4c
    unsigned char   _pad50[0x08];
    unsigned long   VertexCount;                   // +0x58
};

class CEnginePrimitiveManagerAnimatedMeshesBase {
public:
    unsigned char            _pad[0x330];
    CPreTransposedBoneMatrix BonePalette[18];      // +0x330
    void RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4* boneMatrices);
};

void CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4* boneMatrices)
{
    unsigned long count = primitive.AnimatedBlockCount;
    for (unsigned long i = 0; i < count; ++i) {
        CAnimatedBlock& block = primitive.AnimatedBlocks[i];
        unsigned long groups = block.GroupCount;
        for (unsigned long j = 0; j < groups; ++j)
            BonePalette[j] = *(const CPreTransposedBoneMatrix*)&boneMatrices[block.Groups[j]];
        {
            CShaderConstantRange range = g_pEngine_01436e98->BoneRange;
            g_pShaderRenderManager->SetVSConstants(range.Offset, BonePalette, groups * 3);
        }
        g_pShaderRenderManager->DrawIndexed(primitive.IndexBuffer, primitive.VertexCount, block.StartIndex, block.PrimitiveCount,
                                            block.StripFlag ? 5 : 4);
    }
}

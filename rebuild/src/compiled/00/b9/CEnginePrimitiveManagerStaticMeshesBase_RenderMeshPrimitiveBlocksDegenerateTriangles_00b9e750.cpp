// CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksDegenerateTriangles(C3DPrimitive2& primitive, const C3DTextureStages& textureStages) -- unbind the material's diffuse / bump texture stages, then draw every static block flagged DegenerateTriangles of the mesh primitive through its index buffer (strip or list)
// Leaf: ?RenderMeshPrimitiveBlocksDegenerateTriangles@CEnginePrimitiveManagerStaticMeshesBase@@CIXAAVC3DPrimitive2@@ABVC3DTextureStages@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9E750, 361 bytes, private static __fastcall (primitive ecx, textureStages edx),
//   plain `ret`.
// Identity (proven): the Ego_r.pdb and FableWin.pdb name sets both carry
//   ?RenderMeshPrimitiveBlocksDegenerateTriangles@CEnginePrimitiveManagerStaticMeshesBase@@CIXAAVC3DPrimitive2@@ABVC3DTextureStages@@@Z,
//   and the landed caller RenderMeshPrimitiveBlocks 0x00BA1110 binds this address to it (its other caller is 0x00BA60FE);
//   donor order IndexBuffer (0x00B9E5B0) then DegenerateTriangles matches retail. Manifest label correct.
// Callees: IDirect3DDevice9::SetTexture (+0x104, through the inline CRenderManagerCore texture cache), ::SetIndices
//   (+0x1A0), ::DrawIndexedPrimitive (+0x148); CRenderStateManager::RealiseRenderState 0x00A058C0.
// Globals: g_pRenderManagerCore 0x01436E18 (CurrentTextures +0x3D00, NumTextureStagesUsed +0x3CB8, PD3DDevice +0x3CF0;
//   the spelling of the landed SetupFirstTextures 0x00B9D670 / CEngine::Uninitialise 0x00B25D80);
//   NEngineInternal::GPShaderRenderManager 0x01436E14 (RenderManager +0x1D0, D3DDevice +0x1D4, as the landed DrawBuffersIndexed 0x00BA7DB0).
// Layout: C3DPrimitive2 (Ego_r.pdb, retail offsets 4 lower past the 12-byte ClothPrimitives CArray): IndexBuffer +0x34,
//   StaticBlockCount +0x40, StaticBlocks +0x44 (CStaticBlock, 0x20 bytes, base N3DPrimitive::CBaseBlock {PrimitiveCount,
//   StartIndex, IsStrip, DegenerateTriangles, ChangeFlags}), VertexCount +0x58. No engine header exists for C3DPrimitive2,
//   C3DTextureStages, CStaticBlock or CRenderManagerCore (CRenderStateManager.h is the 0x3A3C Ego_r base only).
// Shape: the two stage unbinds are CRenderManagerCore::AttachTextureToStage(stage, 0) inlined (core pointer re-read after
//   each device call); the block draw is the inline six-argument CShaderRenderManager::DrawBuffersIndexed (SetIndices,
//   the -1 type default from the index buffer, RealiseRenderState, DrawIndexedPrimitive) with IsStrip ? strip : list.
//   The block count is read once into a local (retail's count-down loop and the primitive spilled from edi); the draw
//   arguments are named locals in the order primitiveCount, startIndex, vertexCount (that order puts PrimitiveCount in
//   ebx and spills StartIndex / VertexCount as retail does; the direct-argument form swaps them).
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h: CRenderManager : CRenderManager2D :
//   CRenderManagerCore : CRenderStateManager, all at offset 0); the stage unbinds are its PDB inline AttachTextureToStage (repo opt-in
//   FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS), on the shared IDirect3DDevice9 / CIndexBuffer / CTexture and the shared
//   EPrimitiveType (engine/EPrimitiveType.h). Was g_pRenderManagerCore with local IDirect3DDevice9 / CRenderStateManager /
//   CRenderManagerCore / CRenderManager models and a local EPrimitiveType.
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "engine/EPrimitiveType.h"
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

class CShaderRenderManager {
public:
    unsigned char     _pad000[0x1d0];
    CRenderManager*   RenderManager;               // +0x1d0
    IDirect3DDevice9* D3DDevice;                   // +0x1d4
    FABLE_FORCEINLINE void DrawBuffersIndexed(unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
    {
        D3DDevice->SetIndices(indexBuffer.PD3DIndexBuffer);
        if (primitiveType == PT_NULL)
            primitiveType = *(EPrimitiveType*)indexBuffer.PrimitiveType;
        RenderManager->RealiseRenderState();
        D3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, numVertices, startIndex, primitiveCount);
    }
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }   // 0x01436E14

template <class T> class scoped_ptr {              // boost::scoped_ptr (one pointer)
public:
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
private:
    T* ptr;
};

namespace N3DPrimitive {
class CBaseBlock {                                 // Ego_r.pdb, 12 bytes
public:
    unsigned long PrimitiveCount;                  // +0x00
    unsigned long StartIndex;                      // +0x04
    bool          IsStrip;                         // +0x08
    bool          DegenerateTriangles;             // +0x09
    unsigned char ChangeFlags;                     // +0x0a
};
class CStaticBlock : public CBaseBlock {           // Ego_r.pdb, 32 bytes
public:
    unsigned long DiffuseTextureFixupOffset;       // +0x0c
    unsigned long BumpTextureFixupOffset;          // +0x10
    CTexture*     DiffuseTexture;                  // +0x14
    CTexture*     BumpTexture;                     // +0x18
    long          MaterialIndex;                   // +0x1c
};
}

class C3DPrimitive2 {
public:
    unsigned char             _pad00[0x34];
    scoped_ptr<CIndexBuffer>  IndexBuffer;         // +0x34
    unsigned long             VertexBufferStatus;  // +0x38
    unsigned long             IndexBufferStatus;   // +0x3c
    unsigned long             StaticBlockCount;    // +0x40
    N3DPrimitive::CStaticBlock* StaticBlocks;      // +0x44
    unsigned long             AnimatedBlockCount;  // +0x48
    void*                     AnimatedBlocks;      // +0x4c
    unsigned long             AnimatedBlocksStatus;// +0x50
    unsigned long             RepeatedBlocksStatus;// +0x54
    unsigned long             VertexCount;         // +0x58
};

class C3DTextureStages {                           // Ego_r.pdb, 12 bytes
public:
    long DiffuseTextureStage;                      // +0x00
    long BumpTextureStage;                         // +0x04
    bool UseBlankTextures;                         // +0x08
};

class CEnginePrimitiveManagerStaticMeshesBase {
private:
    static void __fastcall RenderMeshPrimitiveBlocksDegenerateTriangles(C3DPrimitive2& primitive, const C3DTextureStages& textureStages);
};

void __fastcall CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksDegenerateTriangles(C3DPrimitive2& primitive,
                                                                                                      const C3DTextureStages& textureStages)
{
    if (textureStages.DiffuseTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.DiffuseTextureStage, 0);
    if (textureStages.BumpTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.BumpTextureStage, 0);

    const unsigned long blockCount = primitive.StaticBlockCount;
    for (unsigned long i = 0; i < blockCount; ++i) {
        N3DPrimitive::CStaticBlock& block = primitive.StaticBlocks[i];
        if (block.DegenerateTriangles) {
            const unsigned long primitiveCount = block.PrimitiveCount;
            const unsigned long startIndex = block.StartIndex;
            const unsigned long vertexCount = primitive.VertexCount;
            CIndexBuffer& indexBuffer = *primitive.IndexBuffer;
            NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(0, vertexCount, indexBuffer, startIndex, primitiveCount,
                                                       block.IsStrip ? PT_TRIANGLE_STRIP : PT_TRIANGLE_LIST);
        }
    }
}
// CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const C3DTextureStages& textureStages) -- draw every static block of the mesh primitive that is not flagged DegenerateTriangles through its index buffer (strip or list), first binding the block's diffuse texture (change flag 1) and bump texture (change flag 2, when the bump stage is in use)
// Grade: DIFFER(403v402) -- callee-saved register assignment: textureStages lands in ebp (retail edi) and the inlined AttachTextureToStage core pointer / stage in ebx / edi (retail ebp / ebx); the ebp-based [ebp+0] stage load is the extra byte
// Leaf: ?RenderMeshPrimitiveBlocksIndexBuffer@CEnginePrimitiveManagerStaticMeshesBase@@CIXAAVC3DPrimitive2@@ABVC3DTextureStages@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9E5B0, 402 bytes, private static __fastcall (primitive ecx, textureStages edx),
//   plain `ret`.
// Identity (proven): the Ego_r.pdb and FableWin.pdb name sets both carry
//   ?RenderMeshPrimitiveBlocksIndexBuffer@CEnginePrimitiveManagerStaticMeshesBase@@CIXAAVC3DPrimitive2@@ABVC3DTextureStages@@@Z,
//   and the landed caller RenderMeshPrimitiveBlocks 0x00BA1110 binds this address to it; donor order IndexBuffer then
//   DegenerateTriangles (0x00B9E750, landed) matches retail. Manifest label correct (class added).
// Callees: IDirect3DDevice9::SetTexture (+0x104, through the inline CRenderManagerCore texture cache), ::SetIndices
//   (+0x1A0), ::DrawIndexedPrimitive (+0x148); CRenderStateManager::RealiseRenderState 0x00A058C0.
// Model: the landed sibling 0x00B9E750's TU (same types, globals and inline helpers; see its header for the layout
//   evidence). Shape: the block loop over the count read once; per block the two inline AttachTextureToStage calls (the
//   diffuse stage untested, the bump stage tested against -1, as retail) and the inline DrawBuffersIndexed with the
//   sibling's named-local argument order. AttachTextureToStage is `__forceinline` here: retail
//   expands both calls, plain inline leaves the bump one out of line (338v402).
#define COM_SLOT(n) virtual void __stdcall slot##n()

struct IDirect3DBaseTexture9;
struct IDirect3DTexture9;
struct IDirect3DIndexBuffer9;

struct IDirect3DDevice9 {                          // only the slots used are typed
    COM_SLOT(0);
    COM_SLOT(1);
    COM_SLOT(2);
    COM_SLOT(3);
    COM_SLOT(4);
    COM_SLOT(5);
    COM_SLOT(6);
    COM_SLOT(7);
    COM_SLOT(8);
    COM_SLOT(9);
    COM_SLOT(10);
    COM_SLOT(11);
    COM_SLOT(12);
    COM_SLOT(13);
    COM_SLOT(14);
    COM_SLOT(15);
    COM_SLOT(16);
    COM_SLOT(17);
    COM_SLOT(18);
    COM_SLOT(19);
    COM_SLOT(20);
    COM_SLOT(21);
    COM_SLOT(22);
    COM_SLOT(23);
    COM_SLOT(24);
    COM_SLOT(25);
    COM_SLOT(26);
    COM_SLOT(27);
    COM_SLOT(28);
    COM_SLOT(29);
    COM_SLOT(30);
    COM_SLOT(31);
    COM_SLOT(32);
    COM_SLOT(33);
    COM_SLOT(34);
    COM_SLOT(35);
    COM_SLOT(36);
    COM_SLOT(37);
    COM_SLOT(38);
    COM_SLOT(39);
    COM_SLOT(40);
    COM_SLOT(41);
    COM_SLOT(42);
    COM_SLOT(43);
    COM_SLOT(44);
    COM_SLOT(45);
    COM_SLOT(46);
    COM_SLOT(47);
    COM_SLOT(48);
    COM_SLOT(49);
    COM_SLOT(50);
    COM_SLOT(51);
    COM_SLOT(52);
    COM_SLOT(53);
    COM_SLOT(54);
    COM_SLOT(55);
    COM_SLOT(56);
    COM_SLOT(57);
    COM_SLOT(58);
    COM_SLOT(59);
    COM_SLOT(60);
    COM_SLOT(61);
    COM_SLOT(62);
    COM_SLOT(63);
    COM_SLOT(64);
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture);   // slot 65 (+0x104)
    COM_SLOT(66); COM_SLOT(67); COM_SLOT(68); COM_SLOT(69); COM_SLOT(70); COM_SLOT(71); COM_SLOT(72); COM_SLOT(73); COM_SLOT(74); COM_SLOT(75);
    COM_SLOT(76); COM_SLOT(77); COM_SLOT(78); COM_SLOT(79); COM_SLOT(80); COM_SLOT(81);
    virtual long __stdcall DrawIndexedPrimitive(unsigned long type, long baseVertexIndex, unsigned long minVertexIndex,
                                                unsigned long numVertices, unsigned long startIndex,
                                                unsigned long primCount);                  // slot 82 (+0x148)
    COM_SLOT(83); COM_SLOT(84); COM_SLOT(85); COM_SLOT(86); COM_SLOT(87); COM_SLOT(88); COM_SLOT(89); COM_SLOT(90); COM_SLOT(91); COM_SLOT(92);
    COM_SLOT(93); COM_SLOT(94); COM_SLOT(95); COM_SLOT(96); COM_SLOT(97); COM_SLOT(98); COM_SLOT(99); COM_SLOT(100); COM_SLOT(101);
    COM_SLOT(102); COM_SLOT(103);
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indexData);                     // slot 104 (+0x1a0)
};

enum EPrimitiveType { PRIMITIVE_TYPE_DEFAULT = -1, PrimitiveTriangleList = 4, PrimitiveTriangleStrip = 5 };

#include "engine/CIndexBuffer.h"                   // Ego_r.pdb, 0x14 bytes (PD3DIndexBuffer +4, PrimitiveType +0xc)
#include "engine/CTexture.h"                       // Ego_r.pdb, 8 bytes (PD3DTexture +0)

class CRenderStateManager {
public:
    void RealiseRenderState();                     // 0x00A058C0
};

class CRenderManagerCore : public CRenderStateManager {   // retail offsets as the landed SetupFirstTextures 0x00B9D670
public:
    unsigned char          _pad0[0x3cb8];
    long                   NumTextureStagesUsed;   // +0x3cb8
    unsigned char          _pad3cbc[0x3cf0 - 0x3cbc];
    IDirect3DDevice9*      PD3DDevice;             // +0x3cf0
    void*                  CurrentIndices;         // +0x3cf4
    unsigned char          _pad3cf8[8];
    IDirect3DBaseTexture9* CurrentTextures[4];     // +0x3d00
    __forceinline void AttachTextureToStage(long stage, const CTexture* texture)
    {
        AttachD3DTextureToStage(stage, texture ? (IDirect3DBaseTexture9*)texture->PD3DTexture : 0);
    }
private:
    void AttachD3DTextureToStage(long stage, IDirect3DBaseTexture9* texture)
    {
        if (CurrentTextures[stage] != texture) {
            CurrentTextures[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (NumTextureStagesUsed < stage + 1)
            NumTextureStagesUsed = stage + 1;
    }
};
extern CRenderManagerCore* g_pRenderManagerCore;   // 0x01436E18

class CRenderManager : public CRenderManagerCore { };

class CShaderRenderManager {
public:
    unsigned char     _pad000[0x1d0];
    CRenderManager*   RenderManager;               // +0x1d0
    IDirect3DDevice9* D3DDevice;                   // +0x1d4
    __forceinline void DrawBuffersIndexed(unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
    {
        D3DDevice->SetIndices(indexBuffer.PD3DIndexBuffer);
        if (primitiveType == PRIMITIVE_TYPE_DEFAULT)
            primitiveType = *(EPrimitiveType*)indexBuffer.PrimitiveType;
        RenderManager->RealiseRenderState();
        D3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, numVertices, startIndex, primitiveCount);
    }
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

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
    static void __fastcall RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const C3DTextureStages& textureStages);
};

void __fastcall CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive,
                                                                                              const C3DTextureStages& textureStages)
{
    const unsigned long blockCount = primitive.StaticBlockCount;
    for (unsigned long i = 0; i < blockCount; ++i) {
        N3DPrimitive::CStaticBlock& block = primitive.StaticBlocks[i];
        if (!block.DegenerateTriangles) {
            if (block.ChangeFlags & 1)
                g_pRenderManagerCore->AttachTextureToStage(textureStages.DiffuseTextureStage, block.DiffuseTexture);
            if ((block.ChangeFlags & 2) && textureStages.BumpTextureStage != -1)
                g_pRenderManagerCore->AttachTextureToStage(textureStages.BumpTextureStage, block.BumpTexture);
            const unsigned long primitiveCount = block.PrimitiveCount;
            const unsigned long startIndex = block.StartIndex;
            const unsigned long vertexCount = primitive.VertexCount;
            CIndexBuffer& indexBuffer = *primitive.IndexBuffer;
            g_pShaderRenderManager->DrawBuffersIndexed(0, vertexCount, indexBuffer, startIndex, primitiveCount,
                                                       block.IsStrip ? PrimitiveTriangleStrip : PrimitiveTriangleList);
        }
    }
}

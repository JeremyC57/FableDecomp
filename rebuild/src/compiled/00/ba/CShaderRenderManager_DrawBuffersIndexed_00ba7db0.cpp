// CShaderRenderManager::DrawBuffersIndexed(const CTexture* textures[], unsigned long textureCount, CVertexBufferWin32& vertexBuffer, unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer, unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType) -- bind the vertex buffer and the first textureCount textures (through the render manager's cached SetTexture), then the index buffer, realise the render state and draw an indexed primitive
// Leaf: ?DrawBuffersIndexed@CShaderRenderManager@@QAEXQAPBVCTexture
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA7DB0, 236 bytes, thiscall, `ret 0x24`.
// Identity: Ego_d.pdb / FableWin.pdb ?DrawBuffersIndexed@CShaderRenderManager@@QAEXQAPBVCTexture@@KAAVCVertexBufferWin32@@KKAAVCIndexBuffer@@KKW4EPrimitiveType@@@Z
//   (inline in the Ego_r build). Same class and layout as the seven-argument overload 0x00B8EB00: CRenderManager* at
//   +0x1D0 (whose CRenderManagerCore part holds the texture cache, CurrentTextures +0x3D00, NumTextureStagesUsed +0x3CB8,
//   PD3DDevice +0x3CF0), IDirect3DDevice9* at +0x1D4. No direct callers in retail. Catalog
//   "CEngineScreen_RenderFrameAdvanced" is wrong.
// Callees: IDirect3DDevice9::SetStreamSource (+0x190), ::SetTexture (+0x104), ::SetIndices (+0x1A0),
//   ::DrawIndexedPrimitive (+0x148), CRenderStateManager::RealiseRenderState 0x00A058C0.
// Shape: SelectVertexBuffer inline; the texture loop runs one induction pointer over textures[] and CurrentTextures[]
//   (`[edx+edi-0x3D00]`, edi from 0x3D00), the render manager re-read each pass. Each texture goes through a
//   CRenderManagerCore::SetTexture(long, const CTexture*) overload (__forceinline) that selects the D3D texture after the
//   render manager is loaded -- the inline select at the call site swaps ebx/edi and shrink-wraps `push ebx` (235v236).
//   Then the six-argument DrawBuffersIndexed overload __forceinline as in 0x00B8EB00.
#include "engine/CIndexBuffer.h"

#define COM_SLOT(n) virtual void __stdcall slot##n()

struct IDirect3DVertexBuffer9;

struct IDirect3DDevice9 {                          // only the slots used are typed
    COM_SLOT(0); COM_SLOT(1); COM_SLOT(2); COM_SLOT(3); COM_SLOT(4); COM_SLOT(5); COM_SLOT(6); COM_SLOT(7); COM_SLOT(8); COM_SLOT(9);
    COM_SLOT(10); COM_SLOT(11); COM_SLOT(12); COM_SLOT(13); COM_SLOT(14); COM_SLOT(15); COM_SLOT(16); COM_SLOT(17); COM_SLOT(18); COM_SLOT(19);
    COM_SLOT(20); COM_SLOT(21); COM_SLOT(22); COM_SLOT(23); COM_SLOT(24); COM_SLOT(25); COM_SLOT(26); COM_SLOT(27); COM_SLOT(28); COM_SLOT(29);
    COM_SLOT(30); COM_SLOT(31); COM_SLOT(32); COM_SLOT(33); COM_SLOT(34); COM_SLOT(35); COM_SLOT(36); COM_SLOT(37); COM_SLOT(38); COM_SLOT(39);
    COM_SLOT(40); COM_SLOT(41); COM_SLOT(42); COM_SLOT(43); COM_SLOT(44); COM_SLOT(45); COM_SLOT(46); COM_SLOT(47); COM_SLOT(48); COM_SLOT(49);
    COM_SLOT(50); COM_SLOT(51); COM_SLOT(52); COM_SLOT(53); COM_SLOT(54); COM_SLOT(55); COM_SLOT(56); COM_SLOT(57); COM_SLOT(58); COM_SLOT(59);
    COM_SLOT(60); COM_SLOT(61); COM_SLOT(62); COM_SLOT(63); COM_SLOT(64);
    virtual long __stdcall SetTexture(unsigned long stage, void* texture);   // slot 65 (+0x104)
    COM_SLOT(66); COM_SLOT(67); COM_SLOT(68); COM_SLOT(69); COM_SLOT(70); COM_SLOT(71); COM_SLOT(72); COM_SLOT(73); COM_SLOT(74); COM_SLOT(75);
    COM_SLOT(76); COM_SLOT(77); COM_SLOT(78); COM_SLOT(79); COM_SLOT(80); COM_SLOT(81);
    virtual long __stdcall DrawIndexedPrimitive(unsigned long type, long baseVertexIndex, unsigned long minVertexIndex,
                                                unsigned long numVertices, unsigned long startIndex,
                                                unsigned long primCount);                  // slot 82 (+0x148)
    COM_SLOT(83); COM_SLOT(84); COM_SLOT(85); COM_SLOT(86); COM_SLOT(87); COM_SLOT(88); COM_SLOT(89); COM_SLOT(90); COM_SLOT(91); COM_SLOT(92);
    COM_SLOT(93); COM_SLOT(94); COM_SLOT(95); COM_SLOT(96); COM_SLOT(97); COM_SLOT(98); COM_SLOT(99);
    virtual long __stdcall SetStreamSource(unsigned long streamNumber, IDirect3DVertexBuffer9* streamData,
                                           unsigned long offsetInBytes, unsigned long stride);   // slot 100 (+0x190)
    COM_SLOT(101); COM_SLOT(102); COM_SLOT(103);
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* indexData);                     // slot 104 (+0x1a0)
};

enum EPrimitiveType { PRIMITIVE_TYPE_DEFAULT = -1, PrimitiveTriangleList = 4 };

class CTexture;

class CRenderStateManager {
public:
    void RealiseRenderState();                     // 0x00A058C0
};

class CRenderManagerCore : public CRenderStateManager {   // CRenderManagerCore fields as the landed CEngine::Uninitialise 0x00B25D80
public:
    unsigned char     _pad0[0x3cb8];
    long              NumTextureStagesUsed;        // +0x3cb8
    unsigned char     _pad3cbc[0x3cf0 - 0x3cbc];
    IDirect3DDevice9* PD3DDevice;                  // +0x3cf0
    void*             CurrentIndices;              // +0x3cf4
    unsigned char     _pad3cf8[8];
    void*             CurrentTextures[4];          // +0x3d00 IDirect3DBaseTexture9*
    void SetTexture(long stage, void* texture)
    {
        if (CurrentTextures[stage] != texture) {
            CurrentTextures[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (NumTextureStagesUsed < stage + 1)
            NumTextureStagesUsed = stage + 1;
    }
    void SetTexture(long stage, const CTexture* texture);   // binds texture ? texture->PD3DTexture : 0
};

class CRenderManager : public CRenderManagerCore { };

class CVertexBufferWin32 {                         // retail layout (engine/CVertexBufferWin32.h keeps the body opaque)
public:
    void*                   __vftable;             // +0x00
    IDirect3DVertexBuffer9* PVertexBuffer;         // +0x04
    unsigned long           Length;                // +0x08
    long                    Type;                  // +0x0c EVertexType
    unsigned char           Stride;                // +0x10
};

struct IDirect3DTexture9;

class CTexture {                                   // Ego_r.pdb, 8 bytes
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      Bits;                       // +0x04 ByteLength:28, AllocationSource:4
};

__forceinline void CRenderManagerCore::SetTexture(long stage, const CTexture* texture)
{
    SetTexture(stage, texture ? texture->PD3DTexture : 0);
}

class CShaderRenderManager {
public:
    unsigned char     _pad000[0x1d0];
    CRenderManager*   RenderManager;               // +0x1d0
    IDirect3DDevice9* D3DDevice;                   // +0x1d4
    void SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
    {
        D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
    }
    __forceinline void DrawBuffersIndexed(unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                                          unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType)
    {
        D3DDevice->SetIndices(indexBuffer.PD3DIndexBuffer);
        if (primitiveType == PRIMITIVE_TYPE_DEFAULT)
            primitiveType = *(EPrimitiveType*)indexBuffer.PrimitiveType;
        RenderManager->RealiseRenderState();
        D3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, numVertices, startIndex, primitiveCount);
    }
    void DrawBuffersIndexed(const CTexture* textures[], unsigned long textureCount, CVertexBufferWin32& vertexBuffer,
                            unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer,
                            unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType);
};

void CShaderRenderManager::DrawBuffersIndexed(const CTexture* textures[], unsigned long textureCount,
                                              CVertexBufferWin32& vertexBuffer, unsigned long minVertexIndex,
                                              unsigned long numVertices, CIndexBuffer& indexBuffer,
                                              unsigned long startIndex, unsigned long primitiveCount,
                                              EPrimitiveType primitiveType)
{
    SelectVertexBuffer(vertexBuffer);
    for (unsigned long i = 0; i < textureCount; ++i)
        RenderManager->SetTexture(i, textures[i]);
    DrawBuffersIndexed(minVertexIndex, numVertices, indexBuffer, startIndex, primitiveCount, primitiveType);
}
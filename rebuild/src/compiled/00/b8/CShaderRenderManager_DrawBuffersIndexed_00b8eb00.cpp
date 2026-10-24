// CShaderRenderManager::DrawBuffersIndexed(CVertexBufferWin32& vertexBuffer, unsigned long minVertexIndex, unsigned long numVertices, CIndexBuffer& indexBuffer, unsigned long startIndex, unsigned long primitiveCount, EPrimitiveType primitiveType) -- bind the vertex and index buffers, realise the render state and draw an indexed primitive (the index buffer's own primitive type when -1 is passed)
// Leaf: ?DrawBuffersIndexed@CShaderRenderManager@@QAEXAAVCVertexBufferWin32
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8EB00, 127 bytes, thiscall, `ret 0x1c`.
// Identity: Ego_d.pdb / FableWin.pdb ?DrawBuffersIndexed@CShaderRenderManager@@QAEXAAVCVertexBufferWin32@@KKAAVCIndexBuffer@@KKW4EPrimitiveType@@@Z
//   (inline in the Ego_r build, so absent from its name set); `this` holds the CRenderManager* at +0x1D0 and the
//   IDirect3DDevice9* at +0x1D4 (Ego_r.pdb CShaderRenderManager RenderManager +0x1E0 / D3DDevice +0x1E4, less 0x10 for four
//   12-byte retail CArrays; the landed AllocateLights 0x0098B420 offsets agree). No direct callers in retail. Catalog
//   "CEngineScreen_RenderFrame" is wrong.
// Callees: IDirect3DDevice9::SetStreamSource (+0x190), ::SetIndices (+0x1A0), ::DrawIndexedPrimitive (+0x148),
//   CRenderStateManager::RealiseRenderState 0x00A058C0 on the render manager.
// Layout: CVertexBufferWin32 PVertexBuffer +4, Stride +0x10 (a byte); engine/CIndexBuffer.h PD3DIndexBuffer +4,
//   PrimitiveType +0xC.
// Shape: SelectVertexBuffer(vertexBuffer) and the six-argument DrawBuffersIndexed overload (both Ego_d.pdb names) inlined:
//   (the overload __forceinline: plain in-class inline leaves it a call here); the overload's by-value copies of indexBuffer and primitiveType are loaded at the inline call, before SetIndices; the
//   device is re-read from `this` for every call; the -1 default is resolved after SetIndices;
//   DrawIndexedPrimitive(type, 0, minVertexIndex, numVertices, startIndex, primitiveCount).
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

class CRenderStateManager {
public:
    void RealiseRenderState();                     // 0x00A058C0
};

class CRenderManager : public CRenderStateManager { };

class CVertexBufferWin32 {                         // retail layout (engine/CVertexBufferWin32.h keeps the body opaque)
public:
    void*                   __vftable;             // +0x00
    IDirect3DVertexBuffer9* PVertexBuffer;         // +0x04
    unsigned long           Length;                // +0x08
    long                    Type;                  // +0x0c EVertexType
    unsigned char           Stride;                // +0x10
};

class CTexture;

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
    void DrawBuffersIndexed(CVertexBufferWin32& vertexBuffer, unsigned long minVertexIndex, unsigned long numVertices,
                            CIndexBuffer& indexBuffer, unsigned long startIndex, unsigned long primitiveCount,
                            EPrimitiveType primitiveType);
};

void CShaderRenderManager::DrawBuffersIndexed(CVertexBufferWin32& vertexBuffer, unsigned long minVertexIndex,
                                              unsigned long numVertices, CIndexBuffer& indexBuffer,
                                              unsigned long startIndex, unsigned long primitiveCount,
                                              EPrimitiveType primitiveType)
{
    SelectVertexBuffer(vertexBuffer);
    DrawBuffersIndexed(minVertexIndex, numVertices, indexBuffer, startIndex, primitiveCount, primitiveType);
}
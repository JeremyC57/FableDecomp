// CRenderManagerCore::DrawCurrentBuffersIndexed(unsigned long minVertexIndex, unsigned long vertexCount, unsigned long startIndex, unsigned long primitiveCount) -- draw the bound buffers with the primitive type of the selected index buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AD40, length 61 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+3CF4h] ; push edi ; mov edi,[eax+0Ch] ; call RealiseRenderState ;
//                mov edx,[esp+18h] ; mov eax,[esp+14h] ; mov esi,[esi+3CF0h] ; mov ecx,[esi] ; push edx ; mov edx,[esp+14h] ;
//                push eax ; mov eax,[esp+14h] ; push edx ; push eax ; push 0 ; push edi ; push esi ; call [ecx+148h] ; pop edi ; pop esi ; ret 10h
// engine/CRenderManagerCore.h: +0x3CF4 CIndexBuffer* PSelectedIndexBuffer, +0x3CF0 IDirect3DDevice9* PD3DDevice;
// engine/CIndexBuffer.h: +0x0C EPrimitiveType PrimitiveType (held as 4 opaque bytes in the generated header).
// Shape: the primitive type is read into a local (edi) BEFORE RealiseRenderState (0x00A058C0) runs, so it is captured
// up front; then IDirect3DDevice9::DrawIndexedPrimitive (COM slot 82, +0x148) with base vertex index 0.
#define CRenderManagerCore CRenderManagerCoreLayout
#include "engine/CRenderManagerCore.h"
#undef CRenderManagerCore

class CRenderStateManager
{
public:
    void RealiseRenderState();   // 0x00A058C0
};
class CRenderManagerCore : public CRenderStateManager, public CRenderManagerCoreLayout
{
};
#include "engine/CIndexBuffer.h"

// d3d9.h is not on the verifier include path, so the IDirect3DDevice9 vtable is declared locally:
// COM methods are __stdcall with `this` pushed. Slots used here: 81 (+0x144) DrawPrimitive,
// 82 (+0x148) DrawIndexedPrimitive, 83 (+0x14C) DrawPrimitiveUP, 84 (+0x150) DrawIndexedPrimitiveUP.
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
};

struct CRenderManagerCore_Methods : CRenderManagerCore {
    long DrawCurrentBuffersIndexed(unsigned long minVertexIndex, unsigned long vertexCount,
                                   unsigned long startIndex, unsigned long primitiveCount);
};

long CRenderManagerCore_Methods::DrawCurrentBuffersIndexed(unsigned long minVertexIndex, unsigned long vertexCount,
                                                           unsigned long startIndex, unsigned long primitiveCount)
{
    long primitiveType = *reinterpret_cast<const long*>(PSelectedIndexBuffer->PrimitiveType);
    RealiseRenderState();
    return PD3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, vertexCount, startIndex, primitiveCount);
}
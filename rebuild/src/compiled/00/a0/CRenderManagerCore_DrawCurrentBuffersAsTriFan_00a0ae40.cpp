// CRenderManagerCore::DrawCurrentBuffersAsTriFan(unsigned long vertexCount, unsigned long triangleCount) -- realise the pending render states, then draw the bound vertex buffer from vertex 0 as a triangle fan
// Leaf: DrawCurrentBuffersAsTriFan
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AE40, length 36 bytes (+12 int3 padding).
// Retail bytes:  push esi ; mov esi,ecx ; call RealiseRenderState ; mov ecx,[esp+0Ch] ; mov esi,[esi+3CF0h] ; mov eax,[esi] ;
//                push ecx ; push 0 ; push 6 ; push esi ; call [eax+144h] ; pop esi ; ret 8
// Manifest label "DrawCurrentBuffersAsPointList" is propagated from its twin at 0x00A0AE70 (which draws D3DPT_POINTLIST, 1);
// this body draws D3DPT_TRIANGLEFAN (6), so it is named for what it does. The first argument is not read (ret 8 keeps it).
// Callees: CRenderStateManager::RealiseRenderState @ 0x00A058C0; IDirect3DDevice9::DrawPrimitive (COM slot 81, +0x144)
// on PD3DDevice (+0x3CF0), start vertex 0, primitive count = the second argument.
#include "engine/CRenderManagerCore.h"

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
    void RealiseRenderState();      // 0x00A058C0 (CRenderStateManager, the primary base at +0)
    long DrawCurrentBuffersAsTriFan(unsigned long vertexCount, unsigned long triangleCount);
};

long CRenderManagerCore_Methods::DrawCurrentBuffersAsTriFan(unsigned long vertexCount, unsigned long triangleCount)
{
    RealiseRenderState();
    return PD3DDevice->DrawPrimitive(6 /* D3DPT_TRIANGLEFAN */, 0, triangleCount);
}
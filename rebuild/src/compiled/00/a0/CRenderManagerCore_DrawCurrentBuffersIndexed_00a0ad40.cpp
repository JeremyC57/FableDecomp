// CRenderManagerCore::DrawCurrentBuffersIndexed(unsigned long minVertexIndex, unsigned long vertexCount, unsigned long startIndex, unsigned long primitiveCount) -- draw the bound buffers with the primitive type of the selected index buffer
// Leaf: ?DrawCurrentBuffersIndexed@CRenderManagerCore@@QAEXKKKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AD40, length 61 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+3CF4h] ; push edi ; mov edi,[eax+0Ch] ; call RealiseRenderState ;
//                mov edx,[esp+18h] ; mov eax,[esp+14h] ; mov esi,[esi+3CF0h] ; mov ecx,[esi] ; push edx ; mov edx,[esp+14h] ;
//                push eax ; mov eax,[esp+14h] ; push edx ; push eax ; push 0 ; push edi ; push esi ; call [ecx+148h] ; pop edi ; pop esi ; ret 10h
// engine/CRenderManagerCore.h: +0x3CF4 CIndexBuffer* PSelectedIndexBuffer, +0x3CF0 IDirect3DDevice9* PD3DDevice;
// engine/CIndexBuffer.h: +0x0C EPrimitiveType PrimitiveType (held as 4 opaque bytes in the generated header).
// Shape: the primitive type is read into a local (edi) BEFORE RealiseRenderState (0x00A058C0) runs, so it is captured
// NF150: ported onto the retail branch of engine/CRenderManagerCore.h (its declaration ?DrawCurrentBuffersIndexed@CRenderManagerCore@@QAEXKKKK@Z,
//   Ego_r.pdb / FableWin.pdb, void) and engine/IDirect3DDevice9.h; was a local CRenderManagerCore_Methods stand-in returning long over a
//   renamed-include CRenderManagerCoreLayout model and a local device vtable.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#include "engine/CRenderManagerCore.h"

void CRenderManagerCore::DrawCurrentBuffersIndexed(unsigned long minVertexIndex, unsigned long vertexCount,
                                                   unsigned long startIndex, unsigned long primitiveCount)
{
    long primitiveType = *reinterpret_cast<const long*>(PSelectedIndexBuffer->PrimitiveType);
    RealiseRenderState();
    PD3DDevice->DrawIndexedPrimitive(primitiveType, 0, minVertexIndex, vertexCount, startIndex, primitiveCount);
}

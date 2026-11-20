// CRenderManagerCore::DrawAsIndexedTris(const void* vertices, long stride, long vertexCount, void* indices, long triangleCount) -- realise the pending render states, then draw user-memory vertices/16-bit indices as an indexed triangle list
// Leaf: ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AF40, length 58 bytes (+6 int3 padding).
// Retail bytes:  push esi ; mov esi,ecx ; call RealiseRenderState ; mov ecx,[esp+0Ch] ; mov edx,[esp+8] ; mov esi,[esi+3CF0h] ;
//                mov eax,[esi] ; push ecx ; mov ecx,[esp+18h] ; push edx ; mov edx,[esp+20h] ; push 65h ; push ecx ;
//                mov ecx,[esp+20h] ; push edx ; push ecx ; push 0 ; push 4 ; push esi ; call [eax+150h] ; pop esi ; ret 14h
// Callees: CRenderStateManager::RealiseRenderState @ 0x00A058C0; IDirect3DDevice9::DrawIndexedPrimitiveUP (COM slot 84,
// +0x150) on PD3DDevice (+0x3CF0): D3DPT_TRIANGLELIST (4), min vertex 0, vertex count, triangle count, indices,
// D3DFMT_INDEX16 (0x65), vertices, stride -- the pushes read the five arguments as (verts, stride, count, indices, tris).
// NF150: ported onto the retail branch of engine/CRenderManagerCore.h (opt-in FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER:
//   ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z, Ego_r.pdb / FableWin.pdb: void, long stride / count, non-const void* indices) and
//   engine/IDirect3DDevice9.h; was a local CRenderManagerCore_Methods stand-in over a renamed-include model and a local device vtable.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#include "engine/CRenderManagerCore.h"

void CRenderManagerCore::DrawAsIndexedTris(const void* vertices, long stride, long vertexCount, void* indices, long triangleCount)
{
    RealiseRenderState();
    PD3DDevice->DrawIndexedPrimitiveUP(4 /* D3DPT_TRIANGLELIST */, 0, vertexCount, triangleCount, indices,
                                       0x65 /* D3DFMT_INDEX16 */, vertices, stride);
}

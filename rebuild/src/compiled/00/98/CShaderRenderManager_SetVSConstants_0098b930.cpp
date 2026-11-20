// CShaderRenderManager::SetVSConstants(long index, const void* data, unsigned long count) -- upload `count` vertex-shader constant rows starting at register `index` (IDirect3DDevice9::SetVertexShaderConstantF); nothing is sent for a zero count
// Leaf: ?SetVSConstants@CShaderRenderManager@@AAEXJPBXK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098B930, 37 bytes, `ret 0Ch`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetVSConstants@CShaderRenderManager@@AAEXJPBXK@Z (private). Parameters and local: Ego_d.pdb (debug length
//   536): this, index, data, count, hr (T_HRESULT).
// Shape: Ego_d.pdb line table (lines 1166..1184): 1168 a debug-only check (274 bytes), 1170 `if (count == 0)` (6 bytes: cmp + jne), 1172
//   `return;` (5 bytes: jmp), 1180 the device call assigned to hr (43 bytes), 1183 a debug-only check of hr (155 bytes). The two checks
//   emit nothing in retail and are not written.
// Callee: IDirect3DDevice9::SetVertexShaderConstantF (vtable +0x178) on D3DDevice (+0x1D4).
// Re-port (2026-11-19): the view-struct stand-in (CShaderRenderManagerSetVSConstantsView, a padded struct and a raw vtable-slot call) is replaced
//   by the member of the shared engine/CShaderRenderManager.h in Ego_d's spelling. This text is also the same-TU copy in the two
//   CShaderRenderManager rows that expand it (0x00989CB0, 0x00989230).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include "engine/CShaderRenderManager.h"
typedef long HRESULT;
void CShaderRenderManager::SetVSConstants(long index, const void* data, unsigned long count)
{
    if (count == 0)
        return;
    HRESULT hr = D3DDevice->SetVertexShaderConstantF(index, (const float*)data, count);
}
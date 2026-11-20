// CDisplayManager::BeginRender() -- open a device scene; on success open the render manager's frame and mark the display rendering
// Leaf: ?BeginRender@CDisplayManager@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BEF20, 41 bytes, thiscall, plain `ret`. Ego_r.pdb 0x00B23C30 / FableWin.pdb 0x0301EBA0
//   ?BeginRender@CDisplayManager@@QAE_NXZ (public, returns bool). IDirect3DDevice9::BeginScene (slot 41, +0xA4) on D3DDevice (+0x58);
//   when it succeeds, CRenderManagerCore::BeginRender 0x00A0B480 (Ego_r.pdb 0x00B74A10 / FableWin.pdb 0x030EA8B0
//   ?BeginRender@CRenderManagerCore@@QAE_NXZ; result dropped) on PRenderManager (+0x08), then Rendering (+0x214) = true.
// NF139 (2026-11-20, q4292 / q6168): moved off the CDisplayManagerBeginRenderView stand-in (and its void CRenderManagerCore::BeginRender
//   declaration) onto engine/CDisplayManager.h, engine/CRenderManager.h and engine/IDirect3DDevice9.h. ToggleRendering is a label
//   invention (no PDB has it).
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"

bool CDisplayManager::BeginRender()
{
    if (D3DDevice->BeginScene() >= 0)
    {
        PRenderManager->BeginRender();
        Rendering = true;
        return true;
    }
    return false;
}

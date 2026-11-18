// CTexture::GenerateMipmapsWithGPU() -- inside a render-state capture block, render every mip level 1..n-1 of the texture by attaching that level's surface to a render target and blitting with the GPU, then restore the display's previous render target and viewport
// Leaf: ?GenerateMipmapsWithGPU@CTexture@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FAF70, 407 bytes, plain `ret` (Ego_r.pdb 0x00B6B010 / FableWin.pdb 0x030DB2D0).
// Ego_d texture TU: locals cb (CRenderStateScopedCaptureBlock), display_manager, old_viewport (const C2DBoxI&), rt, old_rt (CRenderTarget);
//   { mm (long) }. Same-TU bodies in Ego_d order before the row (inlined here, out-of-line copies elsewhere): CTexture::GetNoLevels
//   (Ego_r 0x00B69E00; MATCH at 0x009F9DF0) and CTexture::GetSurfaceLevel (Ego_r 0x00B69E10; RELOCATION_MATCH at 0x009F9E00) -- without
//   them the row is 341 bytes. Opt-in set of the landed same-TU BltToRenderTargetWithGPU 0x009FA780 with the CTexture
//   WATERDYNAMIC / SIZED declarations, CDisplayManager GETRENDERTARGET / SETRENDERTARGET, CSurface COPYFROMMEMORY (inline virtual dtor)
//   and the new overlay opt-in FABLE_RETAIL_CSURFACE_TEXTUREMIPMAP (copy ctor 0x009F2D60, InitialiseFromTextureMipmap 0x009F2F10).
// Callees: GFGetSystemManager 0x009A4EC0, CRenderTarget(CSurface&, CSurface&) 0x00A10DC0, CBaseClass ctor / dtor 0x0099A2D0 / 0x0099A2E0,
//   DetachTarget 0x00A10E10, AttachTarget 0x00A10E60, CSurface::Uninitialise 0x009F2E20, CDisplayManager::SetRenderTarget 0x009BF160 /
//   SetViewport 0x009BEF80, BltToRenderTargetWithGPU 0x009FA780, CRenderStateManager::RestoreCaptureBlock 0x00A05840 (StateManager 0x013CB508).
#define FABLE_RETAIL_CTVERTEXRHWCOLTEX1_ORIGINAL_BASE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_BOOKMARK
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET
#define FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_BLTTORENDERTARGET
#define FABLE_RETAIL_CTEXTURE_WATERDYNAMIC
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_TEXTUREMIPMAP
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include <string.h>
#include "engine/IDirect3DDevice9.h"
#include "engine/Direct3D9Types.h"
#include "stlport/fable_containers.h"
#include "engine/CRenderManager2D.h"
#include "engine/CRenderManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CTVertexRHWColTex1.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CRenderTarget.h"

CSurface CTexture::GetSurfaceLevel(long level) const
{
    IDirect3DSurface9* d3d_surface;
    PD3DTexture->GetSurfaceLevel(level, &d3d_surface);
    CSurface surface;
    surface.InitialiseFromTextureMipmap(d3d_surface);
    return surface;
}

long CTexture::GetNoLevels() const
{
    return PD3DTexture->GetLevelCount();
}

void CTexture::GenerateMipmapsWithGPU()
{
    CRenderStateScopedCaptureBlock cb;
    CDisplayManager* display_manager = GFGetSystemManager()->GetDisplayManager();
    const C2DBoxI& old_viewport = display_manager->GetViewport();
    CRenderTarget old_rt = display_manager->GetRenderTarget();
    CRenderTarget rt;
    for (long mm = 1; mm < GetNoLevels(); ++mm) {
        rt.DetachTarget();
        rt.AttachTarget(GetSurfaceLevel(mm));
        display_manager->SetRenderTarget(rt);
        BltToRenderTargetWithGPU(true, true, false);
    }
    display_manager->SetRenderTarget(old_rt);
    display_manager->SetViewport(old_viewport);
}
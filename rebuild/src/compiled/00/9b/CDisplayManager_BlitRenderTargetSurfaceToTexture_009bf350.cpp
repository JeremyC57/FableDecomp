// CDisplayManager::BlitRenderTargetSurfaceToTexture(CTexture&, const CSurface&) -- suspend scene, copy render target, retry compatible surfaces, resume
// Retail009BF350 310bytes. CDisplayManager donor Rendering+214,CanBeginRender+215,D3DDevice+58 agree.
// Native D3D9 stdcall slots34 StretchRect,41 BeginScene,42 EndScene.
// Texture GetSurfaceLevel009F9E00 returns an owning CSurface; vtable0122F84C and release009F2E20 on scope exit.
// Port (NF98, round 8, q11274): the CTexture_Methods / CSurface_Methods / CPixelFormat_Methods / CRenderManagerCore_Methods / BlitD3DDevice /
//   local CDisplayManager / C2DExtentsI stand-ins replaced by the shared headers: own leaf now the Ego_r.pdb 0x00B24330 / FableWin.pdb
//   0x0301CED0 spelling (CTexture&, const CSurface&; NF98 opt-in FABLE_RETAIL_CDISPLAYMANAGER_BLITRENDERTARGETSURFACETOTEXTURE), callees
//   ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z, CSurface GetPixelFormat / GetDimensions / CopyFromSurface / Uninitialise (inline
//   virtual dtor) with FableWin's inline GetWidth / GetHeight, ?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z through FableWin's inline
//   ??8CPixelFormat@@QBE_NABV0@@Z (NF98 opt-in FABLE_RETAIL_CPIXELFORMAT_OPERATOREQUAL), CRenderManagerCore BeginRender / EndRender through PRenderManager, and the
//   IDirect3DDevice9 slots of engine/IDirect3DDevice9.h. 0x013CA810 keeps its landed descriptive name (in no PDB or data-name set).
// Leaf: ?BlitRenderTargetSurfaceToTexture@CDisplayManager@@QAEXAAVCTexture@@ABVCSurface@@@Z
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BLITRENDERTARGETSURFACETOTEXTURE
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_CSURFACE_PIXELFORMAT
#define FABLE_RETAIL_CSURFACE_GETD3DSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_ISEQUIVALENT
#define FABLE_RETAIL_CPIXELFORMAT_OPERATOREQUAL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#include "fable_string.h"
#include "engine/Direct3D9Types.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
extern void* g_DisplayRenderingEnabled; //013CA810 native nullable render gate

void CDisplayManager::BlitRenderTargetSurfaceToTexture(CTexture& texture, const CSurface& source)
{
    bool rendering = Rendering;
    if (rendering && g_DisplayRenderingEnabled) {
        PRenderManager->EndRender();
        if (D3DDevice->EndScene() >= 0) { Rendering = false; CanBeginRender = false; }
    }
    CSurface destination = texture.GetSurfaceLevel(0);
    if (D3DDevice->StretchRect(source.GetD3DSurface(), 0, destination.GetD3DSurface(), 0, 0) < 0) {
        if (source.GetPixelFormat() == destination.GetPixelFormat() &&
            source.GetWidth() == destination.GetWidth() &&
            source.GetHeight() == destination.GetHeight())
            destination.CopyFromSurface(0, source, 0, ~0u);
    }
    if (rendering && g_DisplayRenderingEnabled) {
        if (D3DDevice->BeginScene() >= 0) { PRenderManager->BeginRender(); Rendering = true; }
    }
}

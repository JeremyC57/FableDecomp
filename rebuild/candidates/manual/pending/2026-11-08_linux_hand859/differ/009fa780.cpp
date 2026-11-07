// CTexture::BltToRenderTargetWithGPU(bool fullRenderTarget, bool scaleHalf, bool pointSample) const -- draw this texture as a screen quad over the viewport / render target
// Grade: DIFFER(2028v2028) -- SetRenderState bookmark test/or operand order in the AlphaBlendEnable block (0x009FAA34: retail mov ecx,[esi+2814] / mov edx,[eax+8], ours swaps them); every other byte relocation-equal. Lane gap per engine/CRenderStateManager.h + GOTCHAS 2026-11-03.; chunk 859: 1 regrade (lane gap, GOTCHAS 2026-11-03); chunk 859 total 111 grades, cross-product grids of behaviour-identical spellings (render manager pointer x du/dv forms (3) x loop pointer-walk/assignment x negated restore test x texture array x dims direct (110 of 144)): best unchanged
// Next idea: none in C++ known -- the operand order varies block by block within one retail function (0x00B70230 / 0x00B761D0 / 0x00B751F0 parked on the same gap).
// Identity: Ego_r.pdb ?BltToRenderTargetWithGPU@CTexture@@QBEX_N00@Z (0x00B6A840 in the Ego_r image); retail 0x009FA780 matches its body
// shape (capture block, CScopedShadersEnable(false,false), viewport, 4 RHW vertices, DrawAsIndexedTris 0x00A0AEA0, ret 0xc).
// Key spellings: (float)width / (float)height inline at each use (named float locals give the wrong frame slots), column-major
// vertex init, loop body RHW before Z, C2DBoxI user copy ctor (FABLE_RETAIL_C2DBOXI_COPY) for the viewport copy.
// Leaf: ?BltToRenderTargetWithGPU@CTexture@@QBEX_N00@Z
// Flags: /GS
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_BLTTORENDERTARGET
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
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

void CTexture::BltToRenderTargetWithGPU(bool fullRenderTarget, bool scaleHalf, bool pointSample) const
{
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(false, false);
    CDisplayManager* displayManager = GFGetSystemManager()->GetDisplayManager();
    CRenderManagerCore& renderManager = displayManager->GetRenderManager();
    C2DBoxI oldViewport = displayManager->GetViewport();
    long width;
    long height;
    if (!fullRenderTarget && !scaleHalf) {
        width = oldViewport.BRX - oldViewport.TLX;
        height = oldViewport.BRY - oldViewport.TLY;
    } else {
        C2DExtentsI dims = displayManager->GetRenderTargetDimensions();
        width = dims.X;
        height = dims.Y;
        C2DBoxI viewport;
        viewport.Initialise(0, 0, width, height);
        displayManager->SetViewport(viewport);
    }
    CTVertexRHWColTex1 verts[4];
    verts[0].X = 0.0f;
    verts[1].X = (float)width;
    verts[2].X = 0.0f;
    verts[3].X = (float)width;
    verts[0].Y = 0.0f;
    verts[1].Y = 0.0f;
    verts[2].Y = (float)height;
    verts[3].Y = (float)height;
    verts[0].U1 = 0.0f;
    verts[1].U1 = 1.0f;
    verts[2].U1 = 0.0f;
    verts[3].U1 = 1.0f;
    verts[0].V1 = 0.0f;
    verts[1].V1 = 0.0f;
    verts[2].V1 = 1.0f;
    verts[3].V1 = 1.0f;
    float du = 0.0f;
    float dv = 0.0f;
    if (scaleHalf) {
        du = 1.0f / ((float)width * 0.5f);
        dv = 1.0f / ((float)height * 0.5f);
    } else if (pointSample) {
        du = 0.5f / (float)width;
        dv = 0.5f / (float)height;
    }
    for (long i = 0; i < 4; ++i) {
        verts[i].RHW = 1.0f;
        verts[i].Z = 0.0f;
        verts[i].U1 += du;
        verts[i].V1 += dv;
    }
    renderManager.SetZBufferWriteEnable(false);
    renderManager.SetAlphaBlendEnable(true);
    renderManager.SetAlphaTestEnable(false);
    renderManager.SetDitherEnable(false);
    if (pointSample) {
        renderManager.SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
        renderManager.SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
        renderManager.SetTextureMipMapMode(0, TEXTURE_MIPMAP_DISABLE);
    }
    renderManager.SetVertexFormat(0x144);
    renderManager.SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
    renderManager.SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);
    renderManager.SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
    renderManager.SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG1);
    renderManager.SetTextureMipmapLODBias(0, -1.0f);
    unsigned short indices[6] = { 0, 1, 2, 1, 3, 2 };
    const CTexture* texture = this;
    renderManager.DrawAsIndexedTris(&texture, 1, verts, sizeof(CTVertexRHWColTex1), 4, indices, 2);
    if (fullRenderTarget || scaleHalf)
        displayManager->SetViewport(oldViewport);
}

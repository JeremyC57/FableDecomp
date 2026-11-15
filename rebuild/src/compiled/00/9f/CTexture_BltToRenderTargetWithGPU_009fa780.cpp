// CTexture::BltToRenderTargetWithGPU(bool ignore_viewport, bool target_is_mipmap, bool disable_filtering) const -- draw this texture as a screen quad over the view / render target
// Identity: Ego_r.pdb ?BltToRenderTargetWithGPU@CTexture@@QBEX_N00@Z (0x00B6A840 in the Ego_r image); retail 0x009FA780 matches its body
// shape (cb block, CScopedShadersEnable(false,false), view, 4 RHW vertices, DrawAsIndexedTris 0x00A0AEA0, ret 0xc).
// Key spellings: (float)width / (float)height inline at each use (named float locals give the wrong frame slots), column-major
// vertex init, loop body RHW before Z, C2DBoxI user copy ctor (FABLE_RETAIL_C2DBOXI_COPY) for the view copy.
// Leaf: ?BltToRenderTargetWithGPU@CTexture@@QBEX_N00@Z
// Worker: relocation-masked exact across2028bytes/498instructions; independent main verification pending.
// Flags: /GS
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

void CTexture::BltToRenderTargetWithGPU(bool ignore_viewport, bool target_is_mipmap, bool disable_filtering) const
{
    CRenderStateScopedCaptureBlock cb;
    CScopedShadersEnable disable_shaders(false, false);
    CDisplayManager* display_manager = GFGetSystemManager()->GetDisplayManager();
    CRenderManager& render_manager = display_manager->GetRenderManager();
    C2DBoxI old_v = display_manager->GetViewport();
    long width;
    long height;
    if (ignore_viewport || target_is_mipmap) {
        C2DExtentsI dim = display_manager->GetRenderTargetDimensions();
        width = dim.X;
        height = dim.Y;
        C2DBoxI view(0, 0, width, height);
        display_manager->SetViewport(view);
    } else {
        width = old_v.BRX - old_v.TLX;
        height = old_v.BRY - old_v.TLY;
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
    float tex_offset_u = 0.0f;
    float tex_offset_v = 0.0f;
    if (target_is_mipmap) {
        tex_offset_u = 1.0f / ((float)width * 0.5f);
        tex_offset_v = 1.0f / ((float)height * 0.5f);
    } else if (disable_filtering) {
        tex_offset_u = 1.0f / (float)width / 2.0f;
        tex_offset_v = 1.0f / (float)height / 2.0f;
    }
    for (long n = 0; n < 4; ++n) {
        verts[n].RHW = 1.0f;
        verts[n].Z = 0.0f;
        verts[n].U1 += tex_offset_u;
        verts[n].V1 += tex_offset_v;
    }
    render_manager.SetZBufferWriteEnable(false);
    render_manager.SetAlphaBlendEnable(true);
    render_manager.SetAlphaTestEnable(false);
    render_manager.SetDitherEnable(false);
    if (disable_filtering) {
        render_manager.SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
        render_manager.SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
        render_manager.SetTextureMipMapMode(0, TEXTURE_MIPMAP_DISABLE);
    }
    render_manager.SetVertexFormat(0x144);
    render_manager.SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
    render_manager.SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);
    render_manager.SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
    render_manager.SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG1);
    render_manager.SetTextureMipmapLODBias(0, -1.0f);
    unsigned short ids[6] = { 0, 1, 2, 1, 3, 2 };
    const CTexture* texture = this;
    render_manager.DrawAsIndexedTris(&texture, 1, verts, sizeof(CTVertexRHWColTex1), 4, ids, 2);
    if (ignore_viewport || target_is_mipmap)
        display_manager->SetViewport(old_v);
}
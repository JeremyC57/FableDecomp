// CRenderManager2D::DrawBufferedPolys() -- draw the buffered 3D quick-draw triangles / lines (opaque list, then the alpha list sorted back to front), batching runs that share texture, blend mode, window and primitive type, then clear the lists
// Grade: DIFFER(3451v3435) -- 17 lines: the memset-style zero register (retail `mov ecx,[ebx+4] ; xor eax,eax` then four `[esp+68h..74h]` stores from eax; ours `xor edx,edx` first, the stores from edx and a separate `xor eax,eax`) plus the pads that follow; chunk 864 (1 grade, probe): the zero constant is a candidate with two pieces here (edx for the stores, eax for the later use) where retail has one (eax) -- research 2.3 zero-register threshold (count of zero stores before tail merging); 100+ earlier grades
// Next idea: lever 4 of the research doc: cross the 3/5 zero-store threshold by writing the four zero stores as one aggregate init or splitting them per arm; probe the const-0 candidate pieces
// Leaf: ?DrawBufferedPolys@CRenderManager2D@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D9C80, 3435 bytes, thiscall, plain `ret`, aligned frame (`and esp,-8`), no EH frame.
// Identity: engine/CRenderManager2D.h declares ?DrawBufferedPolys@CRenderManager2D@@QAEXXZ at this address (Ego_r 0x00B2D610 / FableWin
//   0x030E36A0 name sets; Ego_r donor order DrawBufferedPolys < Render2DDrawList < Draw2DTexture matches retail 0x009D9C80 < 0x009DA9F0
//   < 0x009DC870). Body = the landed Render2DDrawList 0x009DA9F0 without the text path, over QuickDrawTriInfo / QuickDrawVertices
//   (+0x3E64 / +0x3E58) and then QuickDrawAlphaTriInfo / QuickDrawAlphaVertices (+0x3E7C / +0x3E70) after the two qsorts
//   (CompareQuickDrawTriDepth 0x009D8C30 over the records, CompareTriangleAverageDepth 0x009D8CB0 over vertex triples).
// Model: the landed Render2DDrawList TU (same defines, includes and render-state setter sequence with z enabled).
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_C2DBOXF
#include <string.h>
#include <stdlib.h>
#include "engine/IDirect3DDevice9.h"
#include "engine/Direct3D9Types.h"
#include "stlport/fable_containers.h"
#include "engine/CRenderManager2D.h"
#include "engine/CDisplayManager.h"
#include "engine/CFontBank.h"
#include "engine/C3DVector.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CVertexShaderConstantLayoutEmpty.h"
static CVertexShaderConstantLayoutEmpty GVertexShaderConstantLayoutEmpty;
int __cdecl CompareQuickDrawTriDepth(const void* left, const void* right);       // 0x009D8C30
int __cdecl CompareTriangleAverageDepth(const void* left, const void* right);    // 0x009D8CB0

void CRenderManager2D::DrawBufferedPolys()
{
    CTexture texture;
    CTexture current_texture;
    bool is_line=false;
    bool toggle_is_line=false;
    bool change_window;
    unsigned long blend_mode;
    CScopedShadersEnable enabled(true,false);
    CRenderStateScopedCaptureBlock rsblock;
    SetPolyCullMode(POLY_CULL_CCW);
    SetPolyFillMode(POLY_FILL_SOLID);
    SetPolyShadeMode(POLY_SHADE_GOURAUD);
    SetTextureFilterModeMin(0,TEXTURE_FILTER_POINT);
    SetTextureFilterModeMag(0,TEXTURE_FILTER_POINT);
    SetTextureMipMapMode(0,TEXTURE_MIPMAP_POINT);
    SetTextureUVModeU(0,TEXTURE_UV_CLAMP);
    SetTextureUVModeV(0,TEXTURE_UV_CLAMP);
    SetFogEnable(false);
    SetZBufferEnable(true);
    SetZBufferTestFunc(ZBUFFER_FUNC_LESS_EQUAL);
    SetZBufferWriteEnable(true);
    SetClippingEnable(true);
    unsigned long current_blend_mode=1;
    LibraryStateBlockFunctions[1]->Apply();
    CShaderRenderManager& shader_manager=CShaderRenderManager::Get();
    shader_manager.RegisterVertexShaderConstantLayout(0,&GVertexShaderConstantLayoutEmpty);
    CScopedVertexShaderConstantLayout constant_layout(0);
    shader_manager.ApplyVertexShader(VertexShader2D,false);
    shader_manager.PrepareForRender(0);

    long vertices_in=0;
    C2DBoxF current_window(0.0f,0.0f,0.0f,0.0f);
    long c1=0,start_tri=0;
    for(;c1<QuickDrawTriInfo.size();++c1) {
        blend_mode=QuickDrawTriInfo[c1].BlendMode;
        change_window=memcmp(&current_window,&QuickDrawTriInfo[c1].Window,sizeof(C2DBoxF))!=0;
        if(QuickDrawTriInfo[c1].UploadedTexture.IsValid()) {
            texture=QuickDrawTriInfo[c1].UploadedTexture;
            QuickDrawTriInfo[c1].UploadedTexture.Uninitialise();
        } else texture.Uninitialise();
        if(is_line!=QuickDrawTriInfo[c1].Line)toggle_is_line=true;
        if(blend_mode!=current_blend_mode || texture.GetD3DTexture()!=current_texture.GetD3DTexture() || change_window || toggle_is_line) {
            long pending=c1-start_tri;
            if(pending>0) {
                AttachTextureToStage(0,&current_texture);
                if(is_line) {
                    RealiseRenderState();
                    GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(2 /* D3DPT_LINELIST */,pending,&QuickDrawVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
                    vertices_in+=pending*2;
                } else {
                    RealiseRenderState();
                    GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(4 /* D3DPT_TRIANGLELIST */,pending,&QuickDrawVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
                    vertices_in+=pending*3;
                }
                start_tri=c1;
            }
            current_texture=texture;
            if(blend_mode!=current_blend_mode) {
                LibraryStateBlockFunctions[blend_mode]->Apply();current_blend_mode=blend_mode;
            }
            if(change_window) {SetAWindow(QuickDrawTriInfo[c1].Window);current_window=QuickDrawTriInfo[c1].Window;}
            if(toggle_is_line) {is_line=!is_line;toggle_is_line=false;}
        }
    }
    if(c1-start_tri>0) {
        AttachTextureToStage(0,&texture);
        if(is_line) {
            RealiseRenderState();
            GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(2 /* D3DPT_LINELIST */,c1-start_tri,&QuickDrawVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
        } else {
            RealiseRenderState();
            GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(4 /* D3DPT_TRIANGLELIST */,c1-start_tri,&QuickDrawVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
        }
    }

    qsort(&QuickDrawAlphaTriInfo[0],QuickDrawAlphaTriInfo.size(),sizeof(CQuickDrawTriInfo),CompareQuickDrawTriDepth);
    qsort(&QuickDrawAlphaVertices[0],QuickDrawAlphaTriInfo.size(),sizeof(CTVertexRHWColSpecTex1)*3,CompareTriangleAverageDepth);
    vertices_in=0;
    memset(&current_window,0,sizeof(C2DBoxF));
    c1=0;
    start_tri=0;
    for(;c1<QuickDrawAlphaTriInfo.size();++c1) {
        blend_mode=QuickDrawAlphaTriInfo[c1].BlendMode;
        if(QuickDrawAlphaTriInfo[c1].UploadedTexture.IsValid()) {
            texture=QuickDrawAlphaTriInfo[c1].UploadedTexture;
            QuickDrawAlphaTriInfo[c1].UploadedTexture.Uninitialise();
        } else texture.Uninitialise();
        change_window=memcmp(&current_window,&QuickDrawAlphaTriInfo[c1].Window,sizeof(C2DBoxF))!=0;
        if(is_line!=QuickDrawAlphaTriInfo[c1].Line)toggle_is_line=true;
        if(blend_mode!=current_blend_mode || texture.GetD3DTexture()!=current_texture.GetD3DTexture() || change_window==true || toggle_is_line) {
            long pending=c1-start_tri;
            if(pending>0) {
                AttachTextureToStage(0,&current_texture);
                if(is_line) {
                    RealiseRenderState();
                    GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(2 /* D3DPT_LINELIST */,pending,&QuickDrawAlphaVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
                    vertices_in+=pending*2;
                } else {
                    RealiseRenderState();
                    GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(4 /* D3DPT_TRIANGLELIST */,pending,&QuickDrawAlphaVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
                    vertices_in+=pending*3;
                }
                start_tri=c1;
            }
            current_texture=texture;
            if(blend_mode!=current_blend_mode) {
                LibraryStateBlockFunctions[blend_mode]->Apply();current_blend_mode=blend_mode;
            }
            if(change_window==true) {SetAWindow(QuickDrawAlphaTriInfo[c1].Window);current_window=QuickDrawAlphaTriInfo[c1].Window;}
            if(toggle_is_line) {is_line=!is_line;toggle_is_line=false;}
        }
    }
    if(c1-start_tri>0) {
        AttachTextureToStage(0,&texture);
        if(is_line) {
            RealiseRenderState();
            GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(2 /* D3DPT_LINELIST */,c1-start_tri,&QuickDrawAlphaVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
        } else {
            RealiseRenderState();
            GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(4 /* D3DPT_TRIANGLELIST */,c1-start_tri,&QuickDrawAlphaVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
        }
    }
    QuickDrawTriInfo.clear();
    QuickDrawVertices.clear();
    QuickDrawAlphaTriInfo.clear();
    QuickDrawAlphaVertices.clear();
    AttachTextureToStage(0,0);
}

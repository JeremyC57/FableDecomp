// CRenderManager2D::Render2DDrawList(bool zbuffer) -- render and clear buffered 2D primitives and text, restoring shader and render state scopes.
// Leaf: ?Render2DDrawList@CRenderManager2D@@QAEX_N@Z
#define FABLE_RETAIL_RENDER2D_TYPES
#define FABLE_RETAIL_RENDERSTATE_EXPAND_SET
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include <string.h>
#include "engine/Direct3D9Device.h"
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

void CRenderManager2D::Render2DDrawList(bool zbuffer)
{
    unsigned long blend_mode=1;
    CTexture texture;
    CTexture current_texture;
    C2DBoxF current_window;
    bool change_window=false;
    bool is_line=false;
    bool toggle_is_line=false;
    bool was_text=false;
    if(QuickDraw2DTriInfo.size()==0)return;
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
    SetZBufferEnable(zbuffer);
    SetZBufferTestFunc(ZBUFFER_FUNC_LESS_EQUAL);
    SetZBufferWriteEnable(zbuffer);
    SetClippingEnable(true);
    current_texture=CTexture(0);
    unsigned long current_blend_mode=1;
    LibraryStateBlockFunctions[current_blend_mode]->Apply();
    CShaderRenderManager::Get().RegisterVertexShaderConstantLayout(0,&GVertexShaderConstantLayoutEmpty);
    CScopedVertexShaderConstantLayout constant_layout(0);
    CShaderRenderManager::Get().ApplyVertexShader(VertexShader2D,false);
    CShaderRenderManager::Get().PrepareForRender(0);
    current_window=QuickDraw2DTriInfo[0].Window;
    SetAWindow(current_window);
    long vertices_in=0;
    long c1=0,start_tri=0;
    for(;c1<QuickDraw2DTriInfo.size();++c1) {
        CQuickDrawTriInfo& tri_info=QuickDraw2DTriInfo[c1];
        if(!tri_info.IsText) {
            if(was_text) {
                CShaderRenderManager::Get().ApplyVertexShader(VertexShader2D,false);
                CShaderRenderManager::Get().PrepareForRender(0);
                was_text=false;
            }
            blend_mode=tri_info.BlendMode;
            if(tri_info.UploadedTexture.IsValid()) {
                texture=tri_info.UploadedTexture;
                tri_info.UploadedTexture.Uninitialise();
            } else texture.Uninitialise();
            change_window=memcmp(&current_window,&tri_info.Window,sizeof(C2DBoxF))!=0;
            if(is_line!=tri_info.Line)toggle_is_line=true;
        }
        long pending=c1-start_tri;
        if(pending>2000 || tri_info.IsText || texture.GetD3DTexture()!=current_texture.GetD3DTexture() || current_blend_mode!=blend_mode || change_window || toggle_is_line) {
            if(pending>0) {
                AttachTextureToStage(0,&current_texture);
                if(is_line) {
                    RealiseRenderState();
                    GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(D3DPT_LINELIST,pending,&QuickDraw2DVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
                    vertices_in+=(pending)*2;
                } else {
                    RealiseRenderState();
                    GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,pending,&QuickDraw2DVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
                    vertices_in+=(pending)*3;
                }
                start_tri=c1;
            }
            if(tri_info.IsText) {
                float tlx=tri_info.Window.TLX+tri_info.StringX;
                float tly=tri_info.Window.TLY+tri_info.StringY;
                float textz=tri_info.StringZ;
                if(memcmp(&current_window,&tri_info.Window,sizeof(C2DBoxF))!=0) {
                    SetAWindow(tri_info.Window);current_window=tri_info.Window;
                }
                CScopedShadersEnable shader_scope_block(false,false);
                current_blend_mode=9999;
                tri_info.PFont->PrepareToRender();
                tri_info.PFont->Render(tri_info.String,C3DVector(tlx,tly,textz),tri_info.StringScale,tri_info.Colour,0);
                start_tri=c1+1;
                is_line=false;
                current_texture=CTexture(0);
                toggle_is_line=false;
                was_text=true;
            } else {
                if(current_texture.GetD3DTexture()!=texture.GetD3DTexture())current_texture=texture;
                if(blend_mode!=current_blend_mode) {
                    LibraryStateBlockFunctions[blend_mode]->Apply();current_blend_mode=blend_mode;
                }
                if(change_window) {SetAWindow(tri_info.Window);current_window=tri_info.Window;}
                if(toggle_is_line) {is_line=!is_line;toggle_is_line=false;}
            }
        }
    }
    if(c1-start_tri>0) {
        AttachTextureToStage(0,&current_texture);
        if(is_line) {
            RealiseRenderState();
            GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(D3DPT_LINELIST,c1-start_tri,&QuickDraw2DVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
        } else {
            RealiseRenderState();
            GetDisplayManager().GetD3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,c1-start_tri,&QuickDraw2DVertices[vertices_in],sizeof(CTVertexRHWColSpecTex1));
        }
    }
    QuickDraw2DTriInfo.clear();
    QuickDraw2DVertices.clear();
    AttachTextureToStage(0,0);
}

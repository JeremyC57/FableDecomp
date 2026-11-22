// 00b4f8e0 CEngineShadowRenderer::InitialiseTextures
// Leaf: ?InitialiseTextures@CEngineShadowRenderer@@AAE_NXZ
// Flags:
#define FABLE_RETAIL_SHADOW_BUFFERS_PASS27
#define FABLE_RETAIL_SHADOW_BUF_ENUM_PASS27
#define FABLE_RETAIL_CENGINESHADOWRENDERER_INITIALISETEXTURES
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#define FABLE_RETAIL_CDISPLAYMANAGER_ALLTEXTUREFORMATS
#define FABLE_RETAIL_CSURFACE_ASSIGN
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_BUILDDEPTHPACKING
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_LOADGRAPHICFRAME
#define FABLE_RETAIL_CSURFACE_SETPIXEL
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
class C2DExtentsI;
class CPixelFormat;
#include "engine/ESurfaceMultisampleType.h"
#include "engine/CMatrix4x4.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderTarget.h"
#include "engine/CShadowClipVolume.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CRGBColour.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/ESurfacePool.h"
#include "engine/CDisplayManager.h"
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }
using NEngineInternal::GPDisplayManager;
bool CEngineShadowRenderer::InitialiseTextures()
{
    UninitialiseTextures();
    RenderingTechnique = SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS;
    if (UserVideoOptionShadowBufferSize > 0) {
        CPixelFormat depth_format(D3DFMT_D16);
        if (GPDisplayManager->IsValidTextureFormat(depth_format)) {
            CPixelFormat col_surf_pf;
            GPDisplayManager->GetBestOpaqueTextureFormat(16,col_surf_pf,true);
            if (GPDisplayManager->IsValidRenderTargetFormat(col_surf_pf,depth_format)) {
                long buffer_size=UserVideoOptionShadowBufferSize;
                if (ShadowBufferTexture.Initialise(C2DExtentsI(buffer_size,buffer_size),1,depth_format,2,SURFACE_POOL_DEFAULT_LOCATION,false,true)) {
                    if (ShadowBufferColourTexture.Initialise(C2DExtentsI(buffer_size,buffer_size),1,col_surf_pf,1,SURFACE_POOL_DEFAULT_LOCATION,false,true)) {
                        RenderingTechnique=SHADOW_RENDERING_TECHNIQUE_DEPTH_BUFFER;
                        ShadowBufferDepthSurface=ShadowBufferTexture.GetSurfaceLevel(0);
                        ShadowBufferColourSurface=ShadowBufferColourTexture.GetSurfaceLevel(0);
                    } else ShadowBufferTexture.Uninitialise();
                }
            }
        }
        if (RenderingTechnique==SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS && GPDisplayManager->GetPixelShaderVersion()>=1400) {
            const long format_count=8;
            const _D3DFORMAT colour_buffer_formats[format_count]={D3DFMT_A8R8G8B8,D3DFMT_A8B8G8R8};
            CPixelFormat colour_format;
            for (long fmt=0;fmt<format_count;++fmt) {
                CPixelFormat test_format(colour_buffer_formats[fmt]);
                if (GPDisplayManager->IsValidTextureFormat(test_format) && GPDisplayManager->IsValidRenderTargetFormat(test_format,depth_format)) {
                    colour_format=test_format;
                    break;
                }
            }
            if (colour_format.IsValid()) {
                long buffer_size=UserVideoOptionShadowBufferSize;
                if (ShadowBufferTexture.Initialise(C2DExtentsI(buffer_size,buffer_size),1,colour_format,1,SURFACE_POOL_DEFAULT_LOCATION,false,true)) {
                    if (ShadowBufferDepthSurface.InitialiseAsDepthBuffer(C2DExtentsI(buffer_size,buffer_size),depth_format,SURFACE_MULTISAMPLE_NONE,true)) {
                        RenderingTechnique=SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER;
                        ShadowBufferColourTexture=ShadowBufferTexture;
                        ShadowBufferColourSurface=ShadowBufferTexture.GetSurfaceLevel(0);
                        BuildDepthPackingLookupTexture();
                    } else ShadowBufferTexture.Uninitialise();
                }
            }
        }
    }
    if (RenderingTechnique==SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS) {
        RenderTarget.Uninitialise();
        ShadowBufferColourSurface.Uninitialise();
        ShadowBufferDepthSurface.Uninitialise();
        ShadowBufferColourTexture.Uninitialise();
        ShadowBufferTexture.Uninitialise();
        TexturesAllocated=false;
        return UserVideoOptionShadowBufferSize==0;
    }
    RenderTarget.AttachTarget(ShadowBufferColourSurface);
    RenderTarget.AttachZBuffer(ShadowBufferDepthSurface);
    TexturesAllocated=true;
    return true;
}
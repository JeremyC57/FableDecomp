#include <stdio.h>
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/ESurfacePool.h"
#include "engine/ESurfaceMultisampleType.h"
#define FABLE_RETAIL_SHADOW_BUFFERS_PASS27
#define protected public
#define FABLE_RETAIL_CPIXELFORMAT
#include "engine/CPixelFormat.h"
#undef protected
void CPixelFormat::Initialise(_D3DFORMAT f) { FormatIndex=(long)f; }
struct Scenario {
    bool depth,opaque,depthTexture,colourTexture,shader,firstFormat,secondFormat,fallbackTarget,fallbackTexture,depthSurface;
    unsigned validCalls,targetChecks,attach,lookup,reset,depthCreates;
    bool bad;
} cfg;
class CDisplayManager {
public:
    bool IsValidTextureFormat(const CPixelFormat& p) {
        ++cfg.validCalls;
        if(p.FormatIndex==D3DFMT_D16)return cfg.depth;
        if(p.FormatIndex==D3DFMT_A8R8G8B8)return cfg.firstFormat;
        if(p.FormatIndex==D3DFMT_A8B8G8R8)return cfg.secondFormat;
        return false;
    }
    void GetBestOpaqueTextureFormat(unsigned long bits,CPixelFormat& out,bool alpha) {
        if(bits!=16||!alpha)cfg.bad=true;
        out.FormatIndex=D3DFMT_R5G6B5;
    }
    bool IsValidRenderTargetFormat(const CPixelFormat& c,const CPixelFormat& z) {
        ++cfg.targetChecks;
        if(z.FormatIndex!=D3DFMT_D16)cfg.bad=true;
        return c.FormatIndex==D3DFMT_R5G6B5?cfg.opaque:cfg.fallbackTarget;
    }
    unsigned long GetPixelShaderVersion() const {return cfg.shader?1400:1399;}
};
CDisplayManager device;
CDisplayManager* GPDisplayManager=&device;
class CSurface {
public:
    int mark;
    void Uninitialise(){mark=0;}
    bool InitialiseAsDepthBuffer(const C2DExtentsI& e,const CPixelFormat& p,ESurfaceMultisampleType m,bool b) {
        ++cfg.depthCreates;
        if(e.X!=32||e.Y!=32||p.FormatIndex!=D3DFMT_D16||m!=SURFACE_MULTISAMPLE_NONE||!b) cfg.bad=true;
        mark=cfg.depthSurface?77:0;return cfg.depthSurface;
    }
};
class CTexture {
public:
    int mark;
    void Uninitialise(){mark=0;}
    CSurface GetSurfaceLevel(long n) const { if(n!=0||!mark)cfg.bad=true;CSurface s;s.mark=mark;return s; }
    bool Initialise(const C2DExtentsI& e,long n,const CPixelFormat& f,unsigned long usage,ESurfacePool pool,bool flag,bool allocate) {
        if(e.X!=32||e.Y!=32||n!=1||pool!=SURFACE_POOL_DEFAULT_LOCATION||flag||!allocate)cfg.bad=true;
        bool ok=false;
        if(usage==2&&f.FormatIndex==D3DFMT_D16)ok=cfg.depthTexture;
        else if(usage==1&&f.FormatIndex==D3DFMT_R5G6B5)ok=cfg.colourTexture;
        else if(usage==1&&(f.FormatIndex==D3DFMT_A8R8G8B8||f.FormatIndex==D3DFMT_A8B8G8R8))ok=cfg.fallbackTexture;
        else cfg.bad=true;
        mark=ok?int(f.FormatIndex):0;return ok;
    }
};
class CRenderTarget {
public:
    int colour,depth;
    void Uninitialise(){colour=depth=0;}
    void AttachTarget(const CSurface& s){++cfg.attach;colour=s.mark;}
    void AttachZBuffer(const CSurface& s){++cfg.attach;depth=s.mark;}
};
class CEngineShadowRenderer {
public:
    enum EShadowRenderingTechnique {SHADOW_RENDERING_TECHNIQUE_NO_SHADOWS,SHADOW_RENDERING_TECHNIQUE_DEPTH_BUFFER,SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER};
    EShadowRenderingTechnique RenderingTechnique;
    CRenderTarget RenderTarget;
    CSurface ShadowBufferColourSurface,ShadowBufferDepthSurface;
    CTexture ShadowBufferColourTexture,ShadowBufferTexture;
    bool TexturesAllocated;
    long UserVideoOptionShadowBufferSize;
    void UninitialiseTextures(){ ++cfg.reset;RenderTarget.Uninitialise();ShadowBufferTexture.Uninitialise();ShadowBufferColourTexture.Uninitialise();ShadowBufferColourSurface.Uninitialise();ShadowBufferDepthSurface.Uninitialise(); }
    void BuildDepthPackingLookupTexture(){++cfg.lookup;}
    bool InitialiseTextures();
};
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

int main(){
    for(int mode=-1;mode<=1;++mode)for(unsigned mask=0;mask<1024;++mask){
        cfg.depth=(mask&1)!=0;cfg.opaque=(mask&2)!=0;cfg.depthTexture=(mask&4)!=0;cfg.colourTexture=(mask&8)!=0;
        cfg.shader=(mask&16)!=0;cfg.firstFormat=(mask&32)!=0;cfg.secondFormat=(mask&64)!=0;
        cfg.fallbackTarget=(mask&128)!=0;cfg.fallbackTexture=(mask&256)!=0;cfg.depthSurface=(mask&512)!=0;
        cfg.reset=cfg.attach=cfg.lookup=cfg.validCalls=cfg.targetChecks=cfg.depthCreates=0;cfg.bad=false;
        CEngineShadowRenderer r;r.UserVideoOptionShadowBufferSize=mode>0?32:mode;
        bool primary=mode>0&&cfg.depth&&cfg.opaque&&cfg.depthTexture&&cfg.colourTexture;
        bool fallback=mode>0&&!primary&&cfg.shader&&(cfg.firstFormat||cfg.secondFormat)&&cfg.fallbackTarget&&cfg.fallbackTexture&&cfg.depthSurface;
        bool ok=r.InitialiseTextures();
        if(cfg.bad||cfg.reset!=1||ok!=(mode==0||primary||fallback)||r.TexturesAllocated!=(primary||fallback)||
            int(r.RenderingTechnique)!=(primary?1:fallback?2:0)||cfg.lookup!=unsigned(fallback)||cfg.attach!=(primary||fallback?2u:0u))return 1;
        int color=primary?D3DFMT_R5G6B5:fallback?(cfg.firstFormat?D3DFMT_A8R8G8B8:D3DFMT_A8B8G8R8):0;
        int depth=primary?D3DFMT_D16:fallback?77:0;
        if(r.RenderTarget.colour!=color||r.RenderTarget.depth!=depth||r.ShadowBufferColourTexture.mark!=color||
           r.ShadowBufferTexture.mark!=(primary?depth:color))return 2;
    }
    puts("00b4f8e0_TEST PASS");return 0;
}
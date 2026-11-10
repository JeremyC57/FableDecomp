#include <stdio.h>
class C2DExtentsI{public:long X,Y;C2DExtentsI(){}C2DExtentsI(long x,long y):X(x),Y(y){}};
class CPixelFormat{public:long FormatIndex;CPixelFormat(long f=-1):FormatIndex(f){}};
enum ESurfacePool{SURFACE_POOL_DEFAULT_LOCATION=0};
class CSurface{public:long Format;mutable int Calls;CPixelFormat GetPixelFormat()const{++Calls;return CPixelFormat(Format);}};
class CTexture{public:bool Accept;int Calls;C2DExtentsI Size;long Format,Levels;unsigned long Usage;ESurfacePool Pool;bool Dynamic,Render;
 bool Initialise(const C2DExtentsI& size,long levels,const CPixelFormat& format,unsigned long usage,ESurfacePool pool,bool dynamic,bool render){++Calls;Size=size;Levels=levels;Format=format.FormatIndex;Usage=usage;Pool=pool;Dynamic=dynamic;Render=render;return Accept;}
};
class CDisplayManager{public:CSurface RenderTargetSurface;CTexture BackBufferCopyTexture;struct Caps{unsigned long TextureCaps;}D3DDeviceCaps;C2DExtentsI RenderTargetDimensions;
 CSurface& GetRenderTargetSurface(){return RenderTargetSurface;}bool IsNeedingPower2Textures(){return(D3DDeviceCaps.TextureCaps>>1)&1;}
 const C2DExtentsI GetRenderTargetDimensions();bool SetupDepthAndBackBufferTextures();
};
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){return C2DExtentsI(RenderTargetDimensions.X,RenderTargetDimensions.Y);}
bool CDisplayManager::SetupDepthAndBackBufferTextures(){
 CPixelFormat pf=GetRenderTargetSurface().GetPixelFormat();
 C2DExtentsI dimensions;
 if(IsNeedingPower2Textures()){dimensions.X=1024;dimensions.Y=1024;}
 else dimensions=GetRenderTargetDimensions();
 if(!BackBufferCopyTexture.Initialise(dimensions,1,pf,1,SURFACE_POOL_DEFAULT_LOCATION,false,false))return false;
 return true;
}

int main(){
 for(int caps=0;caps<256;++caps)for(int dims=0;dims<4;++dims)for(int success=0;success<2;++success){
  CDisplayManager row;row.D3DDeviceCaps.TextureCaps=caps;row.RenderTargetDimensions=C2DExtentsI(dims*257,dims*193);row.RenderTargetSurface.Format=17+dims;row.RenderTargetSurface.Calls=0;row.BackBufferCopyTexture.Calls=0;row.BackBufferCopyTexture.Accept=success!=0;
  bool got=row.SetupDepthAndBackBufferTextures();long x=(caps&2)?1024:dims*257;long y=(caps&2)?1024:dims*193;CTexture& texture=row.BackBufferCopyTexture;
  if(got!=(success!=0)||row.RenderTargetSurface.Calls!=1||texture.Calls!=1||texture.Size.X!=x||texture.Size.Y!=y||texture.Levels!=1||texture.Format!=17+dims||texture.Usage!=1||texture.Pool!=SURFACE_POOL_DEFAULT_LOCATION||texture.Dynamic||texture.Render)return 1;
 }
 puts("009bee40_TEST PASS");return 0;
}
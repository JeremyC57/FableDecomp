// CDisplayManager::SetupDepthAndBackBufferTextures() -- create a render-format back-buffer copy texture at native or power-of-two size
// Leaf: ?SetupDepthAndBackBufferTextures@CDisplayManager@@AAE_NXZ
// Retail: 0x009BEE40,101 bytes,thiscall
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BUFFERTEXTURES
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_PIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){return C2DExtentsI(RenderTargetDimensions.X,RenderTargetDimensions.Y);}
bool CDisplayManager::SetupDepthAndBackBufferTextures(){
 CPixelFormat pf=GetRenderTargetSurface().GetPixelFormat();
 C2DExtentsI dimensions;
 if(IsNeedingPower2Textures()){dimensions.X=1024;dimensions.Y=1024;}
 else dimensions=GetRenderTargetDimensions();
 if(!BackBufferCopyTexture.Initialise(dimensions,1,pf,1,SURFACE_POOL_DEFAULT_LOCATION,false,false))return false;
 return true;
}
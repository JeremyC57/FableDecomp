// CGlowRendererMethods::InitialiseTextures() -- allocates four glow render targets on a new scratch page.
// Retail00b5ece0 length286. Actual glow header offsets agree. Resource-manager donor
// GPU manager+40e4 differs from retail+3190, represented as a documented native view.
#include "engine/CEngineScreenEffectGlowRenderer.h"
#include "engine/CPixelFormat.h"
#define CEngineResourceManager CEngineResourceManagerDonorLayout
#include "engine/CEngineResourceManager.h"
#undef CEngineResourceManager

struct CGlowPixelFormat:CPixelFormat {CGlowPixelFormat(){FormatIndex=-1;}void Initialise(unsigned format);}; //009e3830
struct CGlowExtents {int Width,Height;CGlowExtents(int w,int h):Width(w),Height(h){}};
struct CGlowScratchManager {
 void ReleaseMemoryPage(long); //00f37030
 long AllocateMemoryPage(long,bool); //00f37660
 long AllocateTexture(long,const CGlowExtents&,long,const CPixelFormat&,unsigned,bool); //00f37440
};
struct CGlowResourceManager {unsigned char UnrelatedResources[0x3190];CGlowScratchManager Scratch;};
extern CGlowResourceManager* gGlowResources; //01436e98
struct CGlowRendererMethods:CEngineScreenEffectGlowRenderer {int InitialiseTextures();};
int CGlowRendererMethods::InitialiseTextures(){
 if(DynamicTexturePageHandle>=0)gGlowResources->Scratch.ReleaseMemoryPage(DynamicTexturePageHandle);
 CGlowPixelFormat format;format.Initialise(21);
 CGlowScratchManager* manager=&gGlowResources->Scratch;
 DynamicTexturePageHandle=manager->AllocateMemoryPage(0,false);
 CGlowExtents extents(256,256);
 GlowTexture256Handle=manager->AllocateTexture(DynamicTexturePageHandle,extents,1,format,1,false);
 extents.Width=64;extents.Height=64;
 GlowTexture64Handle=manager->AllocateTexture(DynamicTexturePageHandle,extents,1,format,1,false);
 extents.Width=64;extents.Height=64;
 GlowTexture64AltHandle=manager->AllocateTexture(DynamicTexturePageHandle,extents,1,format,1,false);
 extents.Width=16;extents.Height=16;
 GlowTexture16Handle=manager->AllocateTexture(DynamicTexturePageHandle,extents,1,format,1,false);
 return GlowTexture256Handle>=0&&GlowTexture64Handle>=0&&GlowTexture64AltHandle>=0&&GlowTexture16Handle>=0?1:0;
}
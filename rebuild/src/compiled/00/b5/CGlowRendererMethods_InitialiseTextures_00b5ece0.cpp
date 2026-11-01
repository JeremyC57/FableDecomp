// CGlowRendererMethods::InitialiseTextures() -- allocates four glow render targets on a new scratch page.
// Leaf: ?InitialiseTextures@CGlowRendererMethods@@QAEHXZ
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail00b5ece0 length286. Actual glow header offsets agree. Resource-manager donor
// GPU manager+40e4 differs from retail+3190, represented as a documented native view.
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
#include "engine/CEngineScreenEffectGlowRenderer.h"
#define CPixelFormat CPixelFormatLayout
#include "engine/CPixelFormat.h"
#undef CPixelFormat
#define CEngineResourceManager CEngineResourceManagerDonorLayout
#include "engine/CEngineResourceManager.h"
#undef CEngineResourceManager

class CPixelFormat : public CPixelFormatLayout { public: CPixelFormat(){FormatIndex=-1;}void Initialise(_D3DFORMAT format);}; //009e3830 (was a CGlowPixelFormat stand-in)
struct CGlowExtents {int Width,Height;CGlowExtents(int w,int h):Width(w),Height(h){}};
class CGPUScratchBufferTextureManager { public:
    void FreeMemoryPage(long a0);   // 0x00F37030 (Ego_r.pdb ?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z)
};
struct CGlowScratchManager {
 long AllocateMemoryPage(long,bool); //00f37660
 long AllocateTexture(long,const CGlowExtents&,long,const CPixelFormat&,unsigned,bool); //00f37440
};
struct CGlowResourceManager {unsigned char UnrelatedResources[0x3190];CGlowScratchManager Scratch;};
extern CGlowResourceManager* gGlowResources; //01436e98
struct CGlowRendererMethods:CEngineScreenEffectGlowRenderer {int InitialiseTextures();};
int CGlowRendererMethods::InitialiseTextures(){
 if(DynamicTexturePageHandle>=0)((CGPUScratchBufferTextureManager*)&(gGlowResources->Scratch))->FreeMemoryPage(DynamicTexturePageHandle);
 CPixelFormat format;format.Initialise(D3DFMT_A8R8G8B8);
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
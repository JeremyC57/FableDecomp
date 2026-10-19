// CEngineScreenEffectRadialBlurRenderer::InitialiseTextures() -- replace scratch page and allocate two 256-square textures.
// Retail 00B5BF10,185 bytes. Shared header matches retail handle offsets14/18/1c.
// Calls F37030 FreeMemoryPage,F37660 AllocteMemoryPage,F37440 AllocateTexture; global1436e98 pool3190.
#ifdef _MSC_VER
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
#include "engine/CPixelFormat.h"
#else
// Host semantic view: independent native pointer and handle members stay separate on LP64.
struct CEngineScreenEffectRadialBlurRenderer { void* BlurTexture256_1; void* BlurTexture256_2; long BlurTextureHandle256_1,BlurTextureHandle256_2,DynamicTexturePageHandle; };
struct CPixelFormat { long FormatIndex; };
#endif
struct PixelFormatMethods : CPixelFormat { PixelFormatMethods(long f) { FormatIndex=-1; Initialise(f); } void Initialise(long); }; // 009e3830
struct C2DExtentsI { long X,Y; C2DExtentsI(long x,long y):X(x),Y(y){} };
struct CGPUScratchBufferTextureManager {
 void FreeMemoryPage(long); // 00f37030
 long AllocteMemoryPage(long,bool); // 00f37660
 long AllocateTexture(long,const C2DExtentsI&,long,const CPixelFormat&,unsigned long,bool); // 00f37440
};
struct EngineTextureGlobal { unsigned char Pad[0x3190]; CGPUScratchBufferTextureManager Scratch; };
extern EngineTextureGlobal* gEngine; //01436e98
struct RadialBlurMethods : CEngineScreenEffectRadialBlurRenderer { long InitialiseTextures(); };
long RadialBlurMethods::InitialiseTextures() {
 if(DynamicTexturePageHandle>=0) gEngine->Scratch.FreeMemoryPage(DynamicTexturePageHandle);
 PixelFormatMethods format(21);
 CGPUScratchBufferTextureManager* pool=&gEngine->Scratch;
 DynamicTexturePageHandle=pool->AllocteMemoryPage(0,false);
 BlurTextureHandle256_1=pool->AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
 BlurTextureHandle256_2=pool->AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
 return BlurTextureHandle256_1>=0 && BlurTextureHandle256_2>=0;
}
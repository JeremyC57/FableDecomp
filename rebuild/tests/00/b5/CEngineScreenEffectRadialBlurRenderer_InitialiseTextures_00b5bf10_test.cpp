// CEngineScreenEffectRadialBlurRenderer::InitialiseTextures() -- replace scratch page and allocate two 256-square textures.
// Retail 00B5BF10,185 bytes. Shared header matches retail handle offsets14/18/1c.
// Calls F37030 FreeMemoryPage,F37660 AllocteMemoryPage,F37440 AllocateTexture; global1436e98 pool3190.
#ifdef _MSC_VER
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
#include "engine/CPixelFormat.h"
#else
// Host semantic view: independent native pointer and handle members stay separate on LP64.
class CEngineScreenEffectRadialBlurRenderer { public: void* BlurTexture256_1; void* BlurTexture256_2; long BlurTextureHandle256_1,BlurTextureHandle256_2,DynamicTexturePageHandle; };
class CPixelFormat { public: long FormatIndex; };
#endif
struct PixelFormatMethods : CPixelFormat { PixelFormatMethods(long f) { FormatIndex=-1; Initialise(f); } void Initialise(long); }; // 009e3830
class C2DExtentsI { public: long X,Y; C2DExtentsI(long x,long y):X(x),Y(y){} };
class CGPUScratchBufferTextureManager { public:
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

#include <stdio.h>
EngineTextureGlobal* gEngine;
static long freed, page, results[2], calls, initArg;
static int bad;
void PixelFormatMethods::Initialise(long f) {initArg=f;FormatIndex=7;}
void CGPUScratchBufferTextureManager::FreeMemoryPage(long p) {freed=p;}
long CGPUScratchBufferTextureManager::AllocteMemoryPage(long n,bool b) {if(n||b)bad=1;return page;}
long CGPUScratchBufferTextureManager::AllocateTexture(long p,const C2DExtentsI& e,long n,const CPixelFormat& f,unsigned long flags,bool b) {
 if(p!=page||e.X!=256||e.Y!=256||n!=1||f.FormatIndex!=7||flags!=1||b)bad=1;
 return results[calls++];
}
int main(){
 EngineTextureGlobal global;gEngine=&global;RadialBlurMethods r;
 for(int old=-1;old<2;++old)for(int a=0;a<2;++a)for(int b=0;b<2;++b){
 r.DynamicTexturePageHandle=old;r.BlurTextureHandle256_1=99;r.BlurTextureHandle256_2=98;
 freed=-99;calls=0;bad=0;page=73;results[0]=a?19:-1;results[1]=b?27:-2;
 long answer=r.InitialiseTextures();
 if(answer!=(a&&b)||calls!=2||bad||initArg!=21||freed!=(old<0?-99:old)||r.DynamicTexturePageHandle!=page||r.BlurTextureHandle256_1!=results[0]||r.BlurTextureHandle256_2!=results[1])return 1;
 }
 printf("00b5bf10_TEST PASS\n");return 0;
}
// CEngineScreenEffectRadialBlurRenderer::InitialiseTextures() -- replace scratch page and allocate two 256-square textures.
// Leaf: ?InitialiseTextures@RadialBlurMethods@@QAEJXZ
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail 00B5BF10,185 bytes. Shared header matches retail handle offsets14/18/1c.
// Calls F37030 FreeMemoryPage,F37660 AllocteMemoryPage,F37440 AllocateTexture; global1436e98 pool3190.
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
#ifdef _MSC_VER
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
#define CPixelFormat CPixelFormatLayout
#include "engine/CPixelFormat.h"
#undef CPixelFormat
#else
// Host semantic view: independent native pointer and handle members stay separate on LP64.
class CEngineScreenEffectRadialBlurRenderer { public: void* BlurTexture256_1; void* BlurTexture256_2; long BlurTextureHandle256_1,BlurTextureHandle256_2,DynamicTexturePageHandle; };
class CPixelFormat { public: long FormatIndex; };
#endif
#ifdef _MSC_VER
class CPixelFormat : public CPixelFormatLayout { public: CPixelFormat() { FormatIndex=-1; } void Initialise(_D3DFORMAT); }; // 009e3830 (was a PixelFormatMethods stand-in)
#endif
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
 CPixelFormat format; format.Initialise(D3DFMT_A8R8G8B8);
 CGPUScratchBufferTextureManager* pool=&gEngine->Scratch;
 DynamicTexturePageHandle=pool->AllocteMemoryPage(0,false);
 BlurTextureHandle256_1=pool->AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
 BlurTextureHandle256_2=pool->AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
 return BlurTextureHandle256_1>=0 && BlurTextureHandle256_2>=0;
}
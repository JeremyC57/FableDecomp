// CEngineShadowRenderer::BuildDepthPackingLookupTexture -- create 256-square ARGB lookup texture and encode column/row parity into pixels.
// Retail00b4f420 complete231bytes. Native shared shadow DepthPackingLookupTexture32c and pixel-format/CSurface layouts; real virtual native Surface view avoids adding vptr to plain shared layout.
// Callees9e3790formatInitialise,9fa280textureInitialise,9f9e00GetSurface,9f33e0Lock,9f31c0SetPixel,9f2dc0Unlock,9f2e20Uninitialise;surfacevtable0122f84c.
#ifndef _MSC_VER
#define __forceinline inline
#endif
#ifdef _MSC_VER
#include "engine/CEngineShadowRenderer.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#else
class CEngineShadowRenderer { public:unsigned char DepthPackingLookupTexture[16];};class CPixelFormat { public:long FormatIndex;};class CTexture { public:void* Texture;long Flags;};
#endif
struct PixelMethods:CPixelFormat {PixelMethods(long r,long g,long b,long a,bool p){Initialise(r,g,b,a,p);}void Initialise(long,long,long,long,bool);}; //9e3790
struct ExtentsNative {long X,Y;ExtentsNative(long x,long y):X(x),Y(y){}};
struct ColourNative {unsigned char R,G,B,A;ColourNative(unsigned char r,unsigned char g,unsigned char b,unsigned char a){G=g;B=b;R=r;A=a;}};
struct LockedRectNative {long Width,Height,Pitch;void* Data;};
struct SurfaceNative {virtual ~SurfaceNative(){Uninitialise();}void* PD3DSurface;unsigned long AllocationSource;void* PAllocatedMemory;
 void Uninitialise(); //009f2e20
 LockedRectNative Lock(unsigned long); //009f33e0
 void Unlock(); //009f2dc0
 void SetPixel(long,long,const ColourNative&,const LockedRectNative&); //009f31c0
};
struct TextureMethods:CTexture {
 bool Initialise(const ExtentsNative&,long,const CPixelFormat&,unsigned long,long,bool,bool); //009fa280
 SurfaceNative GetSurface(long); //009f9e00
};
struct ShadowLookupMethods:CEngineShadowRenderer {void BuildDepthPackingLookupTexture();};
void ShadowLookupMethods::BuildDepthPackingLookupTexture(){
 PixelMethods format(8,8,8,8,false);
 TextureMethods* texture=reinterpret_cast<TextureMethods*>(DepthPackingLookupTexture);
 texture->Initialise(ExtentsNative(256,256),1,format,0,1,false,false);
 SurfaceNative surface=texture->GetSurface(0);
 LockedRectNative rect=surface.Lock(0);
 for(long row=0;row<256;++row){long col=0;unsigned char green=(unsigned char)(row&~1);for(;col<256;++col)surface.SetPixel(col,row,ColourNative(0,green,(unsigned char)col,0),rect);}
 surface.Unlock();
}
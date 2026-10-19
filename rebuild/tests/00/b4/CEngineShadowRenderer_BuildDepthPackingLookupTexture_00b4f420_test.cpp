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
struct CEngineShadowRenderer {unsigned char DepthPackingLookupTexture[16];};struct CPixelFormat {long FormatIndex;};struct CTexture {void* Texture;long Flags;};
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

#include <stdio.h>
static int bad,phase,pixels,unlocks,cleaned;static SurfaceNative* lockedSurface;static TextureMethods* expectedTexture;
static int surfaceToken,pixelStorage;static const LockedRectNative* seenRect;
void PixelMethods::Initialise(long r,long g,long b,long a,bool p){if(r!=8||g!=8||b!=8||a!=8||p||phase!=0)bad=1;phase=1;FormatIndex=9;}
bool TextureMethods::Initialise(const ExtentsNative& e,long levels,const CPixelFormat& f,unsigned long usage,long pool,bool a,bool b){if(this!=expectedTexture||e.X!=256||e.Y!=256||levels!=1||f.FormatIndex!=9||usage||pool!=1||a||b||phase!=1)bad=1;phase=2;return false;}
SurfaceNative TextureMethods::GetSurface(long level){if(this!=expectedTexture||level||phase!=2)bad=1;phase=3;SurfaceNative s;s.PD3DSurface=&surfaceToken;s.AllocationSource=0;s.PAllocatedMemory=0;return s;}
LockedRectNative SurfaceNative::Lock(unsigned long flags){if(flags||phase!=3||PD3DSurface!=&surfaceToken)bad=1;phase=4;lockedSurface=this;LockedRectNative r;r.Width=r.Height=256;r.Pitch=1024;r.Data=&pixelStorage;return r;}
void SurfaceNative::SetPixel(long x,long y,const ColourNative& c,const LockedRectNative& rect){
 if(phase!=4||this!=lockedSurface||x!=pixels%256||y!=pixels/256||c.R||c.G!=(y&~1)||c.B!=x||c.A||rect.Width!=256||rect.Height!=256||rect.Pitch!=1024||rect.Data!=&pixelStorage)bad=1;
 if(pixels&&seenRect!=&rect)bad=1;seenRect=&rect;++pixels;
}
void SurfaceNative::Unlock(){if(this!=lockedSurface||phase!=4||pixels!=65536)bad=1;phase=5;++unlocks;}
void SurfaceNative::Uninitialise(){if(this==lockedSurface&&phase==5){++cleaned;phase=6;}else if(phase>=4)bad=1;}
int main(){ShadowLookupMethods sh;expectedTexture=reinterpret_cast<TextureMethods*>(sh.DepthPackingLookupTexture);sh.BuildDepthPackingLookupTexture();
 if(bad||phase!=6||pixels!=65536||unlocks!=1||cleaned!=1)return 1;
 printf("00b4f420_TEST PASS\n");return 0;}
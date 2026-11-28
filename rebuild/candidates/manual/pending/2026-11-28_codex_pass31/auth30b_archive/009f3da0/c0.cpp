// CSurface::CopyToMemoryB8G8R8A8 -- copy surface texels with reversed channel bytes.
// Leaf: ?CopyToMemoryB8G8R8A8@CSurface@@QAEXPAEPBVC2DBoxI@@@Z
// Flags: (none)
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_SIGNED
#define FABLE_RETAIL_CSURFACELOCKEDRECT_ORIGINAL_IMPLICIT_CTORS_PASS26
#define FABLE_RETAIL_CSURFACE_TEXTUREMIPMAP
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY_BODY
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_CSURFACE_LOCK
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_DIMENSIONS_INLINE
#define FABLE_RETAIL_CSURFACE_COPYTOMEMORY_PASS26
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#include "rebuild_abi.h"
#include "engine/Direct3DSurface9.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CSurface.h"
// Real same-TU Lock body used by the adjacent CopyFromMemory row.
#ifdef _MSC_VER
__declspec(noinline)
#endif
CSurfaceLockedRect CSurface::Lock(unsigned long flags)
{
    C2DExtentsI dimensions = GetDimensions();
    tagRECT rect; rect.top = 0; rect.left = 0; rect.bottom = dimensions.Y; rect.right = dimensions.X;
    _D3DLOCKED_RECT locked;
    if (PD3DSurface->LockRect(&locked, &rect, flags) >= 0) {
        CSurfaceLockedRect result; result.Dimensions = dimensions; result.Pitch = locked.Pitch; result.PMem = (unsigned char*)locked.pBits; return result;
    }
    CSurfaceLockedRect result; result.Pitch = 0; result.PMem = 0; result.Dimensions = C2DExtentsI(0, 0); return result;
}

void CSurface::CopyToMemoryB8G8R8A8(unsigned char* dest,const C2DBoxI* psrc_area)
{
 C2DBoxI src_area;
 if(!psrc_area) {
  src_area.TLX=0;
  src_area.TLY=0;
  src_area.BRX=GetWidth();
  src_area.BRY=GetHeight();
 }else src_area=*psrc_area;
 const long width=src_area.BRX-src_area.TLX;
 const long height=src_area.BRY-src_area.TLY;
 const bool pf_is_32bit_ARGB=GetPixelFormat().GetAsD3DPixelFormat()==D3DFMT_A8R8G8B8;
 CSurface surface;
 CSurfaceLockedRect lock;
 if(pf_is_32bit_ARGB)lock=Lock(0);
 else {
  CPixelFormat pf;
  pf.Initialise(D3DFMT_A8R8G8B8);
  surface.InitialiseAsImage(GetDimensions(),pf,false);
  surface.CopyFromSurface(0,*this,0,0xffffffff);
  lock=surface.Lock(0);
 }
 const long src_pitch=lock.Pitch/4;
 CRGBColour* psrc=reinterpret_cast<CRGBColour*>(lock.PMem);
 CRGBColour* pdst=reinterpret_cast<CRGBColour*>(dest);
 for(long cy=src_area.TLY;cy<height;++cy) {
  CRGBColour* src_temp=psrc;
  CRGBColour* dst_temp=pdst;
  for(long cx=src_area.TLX;cx<width;++cx) {
   CRGBColour src=*src_temp;
   CRGBColour& dst=*dst_temp;
   dst.A=src.B;
   dst.R=src.G;
   dst.G=src.R;
   dst.B=src.A;
   ++src_temp;
   ++dst_temp;
  }
  pdst+=width;
  psrc+=src_pitch;
 }
 if(pf_is_32bit_ARGB)Unlock();
 else {surface.Unlock();surface.Uninitialise();}
}

// CSurface::Clear() -- zero all bytes in the surface and unlock it
// Leaf: ?Clear@CSurface@@QAEXXZ
// Retail: 0x009F40A0,120 bytes,thiscall
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_PIXELFORMAT
#define FABLE_RETAIL_CSURFACE_CLEAR
#include <stddef.h>
#include "engine/Direct3DSurface9.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurface.h"
CSurfaceLockedRect CSurface::Lock(unsigned long flags){
 _D3DSURFACE_DESC desc;PD3DSurface->GetDesc(&desc);
 tagRECT rect;rect.left=0;rect.top=0;rect.right=desc.Width;rect.bottom=desc.Height;
 _D3DLOCKED_RECT locked;
 CSurfaceLockedRect result;
 if(PD3DSurface->LockRect(&locked,&rect,flags)>=0){
  result.Dimensions=C2DExtentsI(desc.Width,desc.Height);result.Pitch=locked.Pitch;result.PMem=(unsigned char*)locked.pBits;
 }else {result.Dimensions=C2DExtentsI(0,0);result.Pitch=0;result.PMem=0;}
 return result;
}
CPixelFormat CSurface::GetPixelFormat()const{_D3DSURFACE_DESC desc;PD3DSurface->GetDesc(&desc);return CPixelFormat(desc.Format);}
extern "C" void* FABLE_CDECL memset(void*,int,size_t);
#ifdef _MSC_VER
#pragma intrinsic(memset)
#endif
void CSurface::Clear(){CSurfaceLockedRect locked=Lock(0);unsigned char* pmem=locked.PMem;memset(pmem,0,GetByteLength());Unlock();}
#include <stdio.h>
#include <string.h>
class C2DExtentsI{public:long X,Y;C2DExtentsI(){}C2DExtentsI(long x,long y):X(x),Y(y){}};
class CSurfaceLockedRect{public:C2DExtentsI Dimensions;long Pitch;unsigned char* PMem;CSurfaceLockedRect(){}CSurfaceLockedRect(const CSurfaceLockedRect& o):Dimensions(o.Dimensions),Pitch(o.Pitch),PMem(o.PMem){}};
enum _D3DFORMAT{FORMAT8=8,FORMAT16=16,FORMAT32=32};
struct _D3DSURFACE_DESC{_D3DFORMAT Format;unsigned int Width,Height;};struct tagRECT{long left,top,right,bottom;};struct _D3DLOCKED_RECT{int Pitch;void* pBits;};
struct IDirect3DSurface9{_D3DSURFACE_DESC Desc;unsigned char Bytes[128];int Descs,Locks,Unlocks;bool Success;tagRECT Captured;
 long GetDesc(_D3DSURFACE_DESC* d){++Descs;*d=Desc;return 0;}
 long LockRect(_D3DLOCKED_RECT* r,const tagRECT* box,unsigned long flags){++Locks;Captured=*box;if(flags)return -1;r->Pitch=64;r->pBits=Bytes;return Success?0:-1;}
 long UnlockRect(){++Unlocks;return 0;}
};
class CPixelFormat{long FormatIndex;public:CPixelFormat(_D3DFORMAT f):FormatIndex((long)f){}unsigned long GetColourDepth()const{return (unsigned long)FormatIndex;}};
class CSurface{public:IDirect3DSurface9* PD3DSurface;CSurfaceLockedRect Lock(unsigned long);CPixelFormat GetPixelFormat()const;unsigned long GetByteLength()const{_D3DSURFACE_DESC desc;PD3DSurface->GetDesc(&desc);return GetPixelFormat().GetColourDepth()*desc.Width*desc.Height/8;}void Unlock()const{PD3DSurface->UnlockRect();}void Clear();};
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
void CSurface::Clear(){CSurfaceLockedRect locked=Lock(0);unsigned char* pmem=locked.PMem;memset(pmem,0,GetByteLength());Unlock();}

int main(){
 const _D3DFORMAT formats[]={FORMAT8,FORMAT16,FORMAT32};
 for(int w=0;w<=4;++w)for(int h=0;h<=3;++h)for(int f=0;f<3;++f){
  IDirect3DSurface9 device;device.Desc.Width=w;device.Desc.Height=h;device.Desc.Format=formats[f];device.Descs=device.Locks=device.Unlocks=0;device.Success=true;memset(device.Bytes,0xcd,sizeof(device.Bytes));
  CSurface surface;surface.PD3DSurface=&device;surface.Clear();unsigned int bytes=w*h*(unsigned int)formats[f]/8;
  for(unsigned int i=0;i<sizeof(device.Bytes);++i)if(device.Bytes[i]!=(i<bytes?0:0xcd))return 1;
  if(device.Descs!=3||device.Locks!=1||device.Unlocks!=1||device.Captured.left||device.Captured.top||device.Captured.right!=w||device.Captured.bottom!=h)return 2;
  device.Success=false;CSurfaceLockedRect failed=surface.Lock(0);if(failed.Dimensions.X||failed.Dimensions.Y||failed.Pitch||failed.PMem)return 3;
 }
 puts("009f40a0_TEST PASS");return 0;
}
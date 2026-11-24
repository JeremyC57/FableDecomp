#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <vector>
struct CRGBColour {
 unsigned char B,G,R,A;
 CRGBColour(){}CRGBColour(const CRGBColour&colour){Initialise(colour.R,colour.G,colour.B,colour.A);}
 void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}
};
typedef char VerifyColourWidth[sizeof(CRGBColour)==4?1:-1];
struct C2DBoxI {long TLX,TLY,BRX,BRY;};
struct C2DExtentsI {long X,Y;C2DExtentsI(long x,long y):X(x),Y(y){}};
const int D3DFMT_A8R8G8B8=21;
struct CPixelFormat {int format;CPixelFormat():format(-1){}CPixelFormat(int f):format(f){}void Initialise(int f){format=f;}int GetAsD3DPixelFormat()const{return format;}};
struct CSurfaceLockedRect {long Pitch;unsigned char*PMem;};
static int initialises,copies,locks,unlocks,releases,dimension_reads;
struct Resource {int width,height,pitch,format,start;std::vector<unsigned char>bytes;};
struct CSurface {
 Resource*r;bool own;CSurface():r(0),own(false){}virtual~CSurface(){Uninitialise();}
 unsigned long GetWidth()const{return GetDimensions().X;}
 unsigned long GetHeight()const{return GetDimensions().Y;}
 C2DExtentsI GetDimensions()const{++dimension_reads;return C2DExtentsI(r->width,r->height);}
 CPixelFormat GetPixelFormat()const{return CPixelFormat(r->format);}
 bool InitialiseAsImage(const C2DExtentsI&d,const CPixelFormat&pf,bool dynamic){assert(!dynamic&&pf.format==21&&!r);++initialises;r=new Resource;r->width=d.X;r->height=d.Y;r->pitch=r->width*4;r->start=0;r->format=pf.format;r->bytes.resize(r->pitch*r->height);own=true;return true;}
 void CopyFromSurface(const C2DBoxI*dst,const CSurface&src,const C2DBoxI*area,unsigned long filter){assert(!dst&&!area&&filter==0xffffffff&&own&&r->width==src.r->width&&r->height==src.r->height);++copies;for(int y=0;y<r->height;++y)memcpy(&r->bytes[y*r->pitch],&src.r->bytes[src.r->start+y*src.r->pitch],r->width*4);}
 CSurfaceLockedRect Lock(unsigned long flags){assert(flags==0&&r);++locks;CSurfaceLockedRect q;q.Pitch=r->pitch;q.PMem=r->bytes.empty()?0:&r->bytes[r->start];return q;}
 void Unlock()const{assert(r);++unlocks;}
 void Uninitialise(){if(own&&r){++releases;delete r;r=0;own=false;}}
 void CopyToMemoryB8G8R8A8(unsigned char*dest,const C2DBoxI*psrc_area);
};

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
int main(){
 for(int converted=0;converted<2;++converted)for(int rectangle=0;rectangle<4;++rectangle)for(int reverse_rows=0;reverse_rows<2;++reverse_rows){
  initialises=copies=locks=unlocks=releases=dimension_reads=0;
  Resource input;input.width=4;input.height=3;input.format=converted?22:21;input.pitch=reverse_rows?-20:20;input.start=reverse_rows?40:0;input.bytes.resize(60);
  for(int y=0;y<3;++y)for(int x=0;x<4;++x)for(int c=0;c<4;++c)input.bytes[input.start+y*input.pitch+x*4+c]=(unsigned char)(1+y*40+x*7+c);
  C2DBoxI area;area.TLX=rectangle==2?1:0;area.TLY=rectangle==2?1:0;area.BRX=rectangle==0||rectangle==2?4:(rectangle==1?2:0);area.BRY=rectangle==0||rectangle==2?3:(rectangle==1?2:0);
  std::vector<unsigned char>out(80,0xcc),expected(80,0xcc);const C2DBoxI*pa=rectangle==0?0:&area;
  int w=pa?area.BRX-area.TLX:4,h=pa?area.BRY-area.TLY:3;int top=pa?area.TLY:0,left=pa?area.TLX:0;
  // Independently bound iteration counts preserve the original rectangle quirk: no initial pixel offset.
  int rows=h>top?h-top:0,cols=w>left?w-left:0;
  for(int y=0;y<rows;++y)for(int x=0;x<cols;++x)for(int c=0;c<4;++c)expected[y*w*4+x*4+c]=input.bytes[input.start+y*input.pitch+x*4+3-c];
  CSurface surface;surface.r=&input;surface.CopyToMemoryB8G8R8A8(&out[0],pa);
  assert(out==expected&&surface.r==&input&&locks==1&&unlocks==1&&initialises==converted&&copies==converted&&releases==converted);
  assert(dimension_reads==(pa?0:2)+converted);
 }
 puts("009f3da0_TEST PASS");return 0;
}

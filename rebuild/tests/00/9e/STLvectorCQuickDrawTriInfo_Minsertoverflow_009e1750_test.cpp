// 009e1750 std::vector<CQuickDrawTriInfo>::_M_insert_overflow
// Flags: /O2 /Oy /W3 
// Leaf: _M_insert_overflow
#include <stdlib.h>
#include <new>
#include "engine/CTexture.h"
namespace QuickDraw {
// Layout: retail vector has start/finish/storage at +0,+4,+8; 60-byte objects use the landed copy constructor 009de890.
struct C2DBoxF{float TLX,TLY,BRX,BRY;};
struct CWideStringData;struct CWideString{CWideStringData*Object;};
// Layout: PDB CQuickDrawTriInfo60: Line/IsText bits0/1; Window+4, String+20, active CTexture+24, union+32, StringZ+40, StringScale+44, StringFlags+48, Colour+52, PFont+56.
struct CQuickDrawTriInfo {unsigned char Line:1;unsigned char IsText:1;C2DBoxF Window;CWideString String;::CTexture UploadedTexture;unsigned PositionOrIndices[2];float StringZ,StringScale;unsigned StringFlags;unsigned char Colour[4];void*PFont;CQuickDrawTriInfo();CQuickDrawTriInfo(const CQuickDrawTriInfo&);~CQuickDrawTriInfo();};
struct __false_type {__false_type(){}};
template<class Tag>__declspec(noinline) CQuickDrawTriInfo* __fastcall uninitialized_copy(CQuickDrawTriInfo*a,CQuickDrawTriInfo*b,CQuickDrawTriInfo*c,const Tag&){while(a!=b){if(c)new(c)CQuickDrawTriInfo(*a);++a;++c;}return c;}
template<class T>inline const T&max(const T&a,const T&b){return a<b?b:a;}
namespace std {
template<class T>struct vector {T*_M_start,*_M_finish,*_M_end_of_storage;
 void _M_clear();void _M_insert_overflow(T*,const T&,const __false_type&,unsigned,bool);
};
template<class T>void vector<T>::_M_insert_overflow(T*pos,const T&v,const __false_type&,unsigned n,bool atEnd){
 unsigned old=_M_finish-_M_start;unsigned capacity=old+max(old,n);
 T*buffer=capacity?(T*)malloc(capacity*sizeof(T)):0;
 T*out=buffer;
 for(T*p=_M_start;p!=pos;++p,++out)if(out)new(out)T(*p);
 if(n==1){if(out)new(out)T(v);++out;}
 else for(unsigned count=n;count>0;--count,++out)if(out)new(out)T(v);
 if(!atEnd)out=uninitialized_copy(pos,_M_finish,out,__false_type());
 _M_clear();_M_start=buffer;_M_finish=out;_M_end_of_storage=buffer+capacity;
}
template struct vector<CQuickDrawTriInfo>;
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
struct IDirect3DTexture9{virtual int QueryInterface()=0;virtual unsigned AddRef()=0;virtual unsigned Release()=0;};
struct Texture:IDirect3DTexture9{unsigned refs;Texture():refs(1){}int QueryInterface(){return 0;}unsigned AddRef(){return ++refs;}unsigned Release(){return --refs;}};
namespace QuickDraw {
// Fixture storage follows retail wide string16-byte data, refs+12 and free-owned text.
struct CWideStringData{unsigned short*text,*finish,*storage;int refs;};
CQuickDrawTriInfo::CQuickDrawTriInfo(){Line=0;IsText=0;memset(&Window,0,sizeof(Window));String.Object=0;memset(&UploadedTexture,0,sizeof(UploadedTexture));memset(PositionOrIndices,0,sizeof(PositionOrIndices));StringZ=0;StringScale=0;StringFlags=0;memset(Colour,0,sizeof(Colour));PFont=0;}
CQuickDrawTriInfo::CQuickDrawTriInfo(const CQuickDrawTriInfo&r){Line=r.Line;IsText=r.IsText;Window=r.Window;String=r.String;if(String.Object)++String.Object->refs;UploadedTexture=r.UploadedTexture;if(UploadedTexture.PD3DTexture)UploadedTexture.PD3DTexture->AddRef();PositionOrIndices[0]=r.PositionOrIndices[0];PositionOrIndices[1]=r.PositionOrIndices[1];StringZ=r.StringZ;StringScale=r.StringScale;StringFlags=r.StringFlags;memcpy(Colour,r.Colour,4);PFont=r.PFont;}
CQuickDrawTriInfo::~CQuickDrawTriInfo(){if(String.Object&&--String.Object->refs<=0){free(String.Object->text);delete String.Object;}if(UploadedTexture.PD3DTexture)UploadedTexture.PD3DTexture->Release();}
namespace std {template<>void vector<CQuickDrawTriInfo>::_M_clear(){for(CQuickDrawTriInfo*p=_M_start;p!=_M_finish;++p)p->~CQuickDrawTriInfo();free(_M_start);}}
}
int main(){using namespace QuickDraw;Texture texture;QuickDraw::std::vector<CQuickDrawTriInfo>v;memset(&v,0,sizeof(v));CQuickDrawTriInfo seed;seed.Line=1;seed.IsText=1;seed.PositionOrIndices[0]=17;seed.String.Object=new CWideStringData;seed.String.Object->refs=1;seed.String.Object->text=(unsigned short*)malloc(4*sizeof(unsigned short));seed.String.Object->finish=seed.String.Object->text+1;seed.String.Object->storage=seed.String.Object->text+4;seed.String.Object->text[0]=65;seed.String.Object->text[1]=0;seed.UploadedTexture.PD3DTexture=&texture;texture.AddRef();__false_type tag;
v._M_insert_overflow(v._M_start,seed,tag,1,false);assert(v._M_finish-v._M_start==1&&v._M_start[0].PositionOrIndices[0]==17&&seed.String.Object->refs==2&&texture.refs==3);
v._M_insert_overflow(v._M_start,seed,tag,2,false);assert(v._M_finish-v._M_start==3&&v._M_start[2].Line&&v._M_start[2].IsText&&seed.String.Object->refs==4&&texture.refs==5);
v._M_insert_overflow(v._M_finish,seed,tag,0,true);assert(v._M_finish-v._M_start==3&&seed.String.Object->refs==4);
v._M_insert_overflow(v._M_finish,seed,tag,1,true);assert(v._M_finish-v._M_start==4&&seed.String.Object->refs==5);v._M_clear();assert(seed.String.Object->refs==1&&texture.refs==2);puts("009e1750_TEST PASS");return 0;}
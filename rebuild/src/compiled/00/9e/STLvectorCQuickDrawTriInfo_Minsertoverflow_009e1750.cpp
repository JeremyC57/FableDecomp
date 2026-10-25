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
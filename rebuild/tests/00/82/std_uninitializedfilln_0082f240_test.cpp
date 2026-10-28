#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
class CScriptedMapBrush { public: class CScriptedMapCell { public: float Height; unsigned long Theme[3]; unsigned char ThemeBlend[2]; }; };
namespace std { struct __false_type { __false_type() {} }; template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); } template<class Iter,class Size,class T> Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&) { Iter current=first; for(;count>0;--count,++current) _Construct(&*current,value); return current; } }

typedef CScriptedMapBrush::CScriptedMapCell Element;
template Element* __fastcall std::__uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&,const std::__false_type&);
int main() { Element src[3]; memset(src,0,sizeof(src)); for(int i=0;i<3;++i) { src[i].Height=float(i)+0.5f; for(int j=0;j<3;++j) src[i].Theme[j]=i*7+j; src[i].ThemeBlend[0]=(unsigned char)(i+3); src[i].ThemeBlend[1]=(unsigned char)(i+9); } Element* dst=(Element*)malloc(3*sizeof(Element)); memset(dst,0,3*sizeof(Element)); Element* end;
end=std::__uninitialized_fill_n(dst,3U,src[0],std::__false_type()); if(end!=dst+3) goto fail; for(int i=0;i<3;++i) if(memcmp(dst+i,src,sizeof(Element))) goto fail; end=std::__uninitialized_fill_n(dst,0U,src[0],std::__false_type()); if(end!=dst) goto fail;
free(dst); puts("0082f240_TEST PASS"); return 0; fail: puts("0082f240_TEST FAIL"); return 1; }
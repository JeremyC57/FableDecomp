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
class CMatrix3x4 { public: float M[12]; }; namespace N3DAnimationBuilder { class CSequenceInfo { public: long OriginalIndex; CMatrix3x4 GeneratedTransform; }; }
namespace std { struct __false_type { __false_type() {} }; template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); } template<class Input,class Output> Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) { Output current=result; for(;first!=last;++first,++current) _Construct(&*current,*first); return current; } }

typedef N3DAnimationBuilder::CSequenceInfo Element;
template Element* __fastcall std::__uninitialized_copy<Element*,Element*>(Element*,Element*,Element*,const std::__false_type&);
int main() { Element src[3]; memset(src,0,sizeof(src)); for(int i=0;i<3;++i) { src[i].OriginalIndex=i+5; for(int j=0;j<12;++j) src[i].GeneratedTransform.M[j]=float(i*12+j)+0.5f; } Element* dst=(Element*)malloc(3*sizeof(Element)); memset(dst,0,3*sizeof(Element)); Element* end;
end=std::__uninitialized_copy(src,src+3,dst,std::__false_type()); if(end!=dst+3 || memcmp(src,dst,sizeof(src))) goto fail; end=std::__uninitialized_copy(src,src,dst,std::__false_type()); if(end!=dst) goto fail;
free(dst); puts("00ab13a0_TEST PASS"); return 0; fail: puts("00ab13a0_TEST FAIL"); return 1; }
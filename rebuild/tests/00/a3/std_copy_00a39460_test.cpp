#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
enum EConversationAttitude {};
namespace std { template<class A,class B> struct pair { A first; B second; pair():first(A()),second(B()){} pair(const pair& p):first(p.first),second(p.second){} }; }
namespace std {
struct __false_type { __false_type() {} };
struct input_iterator_tag {}; struct forward_iterator_tag:input_iterator_tag {}; struct bidirectional_iterator_tag:forward_iterator_tag {}; struct random_access_iterator_tag:bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> __forceinline Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) { for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result; }
template<class Input,class Output> Output __fastcall __copy_ptrs(Input first,Input last,Output result,const __false_type&) { return __copy(first,last,result,random_access_iterator_tag(),(int*)0); }
}

typedef std::pair<unsigned long,long> Element;
template Element* __fastcall std::__copy_ptrs<const Element*,Element*>(const Element*,const Element*,Element*,const std::__false_type&);
namespace std { template<class Input,class Output> Output __fastcall copy(Input first,Input last,Output result) { return __copy_ptrs(first,last,result,__false_type()); } }
template Element* __fastcall std::copy<const Element*,Element*>(const Element*,const Element*,Element*);
int main() { Element src[3],dst[3]; int i; for(i=0;i<3;++i) { src[i].first=i*7+2; src[i].second=-17-i; dst[i].first=0; }
 Element* end=std::copy((const Element*)src,(const Element*)src+3,dst); if(end!=dst+3) goto fail;
 for(i=0;i<3;++i) if(dst[i].first!=src[i].first || dst[i].second!=src[i].second) goto fail;
 end=std::copy((const Element*)src,(const Element*)src,dst); if(end!=dst) goto fail;
 end=std::copy((const Element*)src,(const Element*)src+1,src); if(end!=src+1 || src[0].first!=(0*7+2)) goto fail;
 puts("00a39460_TEST PASS"); return 0; fail: puts("00a39460_TEST FAIL"); return 1;
}
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

typedef std::pair<float,EConversationAttitude> Element;
template Element* __fastcall std::__copy_ptrs<const Element*,Element*>(const Element*,const Element*,Element*,const std::__false_type&);
int main() { Element src[3],dst[3]; int i; for(i=0;i<3;++i) { src[i].first=float(i)*3.0f+0.5f; src[i].second=(EConversationAttitude)0; dst[i].first=0; }
 Element* end=std::__copy_ptrs((const Element*)src,(const Element*)src+3,dst,std::__false_type()); if(end!=dst+3) goto fail;
 for(i=0;i<3;++i) if(dst[i].first!=src[i].first || dst[i].second!=src[i].second) goto fail;
 end=std::__copy_ptrs((const Element*)src,(const Element*)src,dst,std::__false_type()); if(end!=dst) goto fail;
 end=std::__copy_ptrs((const Element*)src,(const Element*)src+1,src,std::__false_type()); if(end!=src+1 || src[0].first!=(float(0)*3.0f+0.5f)) goto fail;
 puts("00718cc0_TEST PASS"); return 0; fail: puts("00718cc0_TEST FAIL"); return 1;
}
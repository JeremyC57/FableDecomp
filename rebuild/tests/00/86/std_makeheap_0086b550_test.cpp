#include <stdio.h>
class CRandomAppearanceMorph { public: class CTextureMorph { public: unsigned long NewTexture,GroupID; }; };
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE __declspec(noinline)
#endif
namespace std { template<class A,class B> struct pair { A first; B second; pair():first(A()),second(B()) {} pair(const pair& other):first(other.first),second(other.second) {} }; }
template<class K,class V> class CKeyPairCompareLess { public: bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; } };
namespace std {
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __push_heap(Iter first,Distance hole,Distance top,T value,Compare comp) { Distance parent=(hole-1)/2; while(hole>top && comp(*(first+parent),value)) { *(first+hole)=*(first+parent); hole=parent; parent=(hole-1)/2; } *(first+hole)=value; }
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __adjust_heap(Iter first,Distance hole,Distance length,T value,Compare comp) { Distance top=hole; Distance secondChild=2*hole+2; while(secondChild<length) { if(comp(*(first+secondChild),*(first+secondChild-1))) --secondChild; *(first+hole)=*(first+secondChild); hole=secondChild; secondChild=2*(secondChild+1); } if(secondChild==length) { *(first+hole)=*(first+secondChild-1); hole=secondChild-1; } __push_heap(first,hole,top,value,comp); }
}
namespace std {template<class Iter,class Compare,class Distance,class T> void __fastcall __make_heap(Iter first,Iter last,Compare comp,Distance*,T*) { if(last-first<2)return; Distance length=last-first;Distance parent=(length-2)/2;for(;;){__adjust_heap(first,parent,length,T(*(first+parent)),comp);if(parent==0)return;--parent;} } }
typedef std::pair<unsigned long,CRandomAppearanceMorph::CTextureMorph> Element;typedef CKeyPairCompareLess<unsigned long,CRandomAppearanceMorph::CTextureMorph> Compare;
template void __fastcall std::__make_heap<Element*,Compare,int,Element>(Element*,Element*,Compare,int*,Element*);
static Element value(int key){Element x;x.first=(unsigned long)key;x.second.NewTexture=key*3+7; x.second.GroupID=key*3+7;return x;}static bool payload(const Element& x){return x.second.NewTexture==long(int(x.first)*3+7) && x.second.GroupID==long(int(x.first)*3+7);}
int main(){Element x[9];int keys[9]={7,1,9,1,0,8,2,4,3};for(int i=0;i<9;++i)x[i]=value(keys[i]);std::__make_heap(x,x+9,Compare(),(int*)0,(Element*)0);for(int i=0;i<9;++i){if(!payload(x[i]) || (i && x[(i-1)/2].first<x[i].first))goto fail;}std::__make_heap(x,x,Compare(),(int*)0,(Element*)0);std::__make_heap(x,x+1,Compare(),(int*)0,(Element*)0);puts("0086b550_TEST PASS");return 0;fail:puts("0086b550_TEST FAIL");return 1;}
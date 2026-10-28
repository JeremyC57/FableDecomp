#include <stdio.h>
class CDefString { public: long TablePos; }; class CSkeletalMorphs { public: class CEntry { public: CDefString MorphName; short Priority; bool SupressesLowerMorphs; }; };
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
template<class Iter,class Distance,class T,class Compare> void __fastcall __adjust_heap(Iter first,Distance hole,Distance length,T value,Compare comp) { Distance top=hole; Distance secondChild=2*hole+2; while(secondChild<length) { if(comp(*(first+secondChild),*(first+secondChild-1))) --secondChild; *(first+hole)=*(first+secondChild); hole=secondChild; secondChild=2*(secondChild+1); } if(secondChild==length) { *(first+hole)=*(first+secondChild-1); hole=secondChild-1; } __push_heap(first,hole,top,value,comp); }
}

typedef std::pair<unsigned long,CSkeletalMorphs::CEntry> Element; typedef CKeyPairCompareLess<unsigned long,CSkeletalMorphs::CEntry> Compare;
template void __fastcall std::__adjust_heap<Element*,int,Element,Compare>(Element*,int,int,Element,Compare);
static Element value(unsigned long key) { Element x; x.first=key; x.second.MorphName.TablePos=key*3+7; x.second.Priority=key*3+7; x.second.SupressesLowerMorphs=bool(key&1); return x; }
static bool payload(const Element& x) { return x.second.MorphName.TablePos==long(x.first*3+7) && x.second.Priority==long(x.first*3+7) && x.second.SupressesLowerMorphs==bool(x.first&1); }
int main() { unsigned long keys[7]={9,7,8,1,2,3,4}; Element heap[7]; for(int i=0;i<7;++i) heap[i]=value(keys[i]); std::__adjust_heap(heap,0,7,value(0),Compare()); for(int i=0;i<7;++i) { if(!payload(heap[i]) || heap[i].first==9) goto fail; if(i && heap[(i-1)/2].first<heap[i].first) goto fail; }
 heap[0]=value(99); heap[1]=value(44); std::__adjust_heap(heap,0,2,value(0),Compare()); if(heap[0].first!=44 || heap[1].first || !payload(heap[0]) || !payload(heap[1])) goto fail;
 heap[0]=value(1); std::__adjust_heap(heap,0,1,value(0x80000000UL),Compare()); if(heap[0].first!=0x80000000UL ) goto fail;
 puts("0081ac20_TEST PASS"); return 0; fail: puts("0081ac20_TEST FAIL"); return 1; }
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
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __adjust_heap(Iter first,Distance hole,Distance length,T value,Compare comp) { Distance top=hole; Distance secondChild=2*hole+2; while(secondChild<length) { if(comp(*(first+secondChild),*(first+secondChild-1))) --secondChild; *(first+hole)=*(first+secondChild); hole=secondChild; secondChild=2*(secondChild+1); } if(secondChild==length) { *(first+hole)=*(first+secondChild-1); hole=secondChild-1; } __push_heap(first,hole,top,value,comp); }
}
template<class T> struct IterValue {}; template<class T> struct IterValue<T*> { typedef T type; };
namespace std {
template<class T> inline void swap(T& a,T& b) { T tmp=a; a=b; b=tmp; }
template<class Iter,class T,class Compare> Iter __fastcall __unguarded_partition(Iter first,Iter last,T pivot,Compare comp) { for(;;) { while(comp(*first,pivot)) ++first; --last; while(comp(pivot,*last)) --last; if(!(first<last)) return first; swap(*first,*last); ++first; } }
template<class Iter,class T,class Compare,class Distance> inline void __fastcall __pop_heap(Iter first,Iter last,Iter result,T value,Compare comp,Distance*) { *result=*first; __adjust_heap(first,Distance(0),Distance(last-first),value,comp); }
template<class Iter,class T,class Compare> void __fastcall __pop_heap_aux(Iter first,Iter last,T*,Compare comp) { __pop_heap(first,last-1,last-1,T(*(last-1)),comp,(int*)0); }
template<class Iter,class Compare> NOINLINE void __fastcall __make_heap(Iter first,Iter last,Compare comp,int*,void*) { if(last-first<2) return; int length=int(last-first); int parent=(length-2)/2; for(;;) { __adjust_heap(first,parent,length,*(first+parent),comp); if(parent==0) return; --parent; } }
template<class Iter,class T,class Compare> NOINLINE void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp) { Iter next=last; --next; while(comp(value,*next)) { *last=*next;last=next;--next; } *last=value; }
struct __false_type {}; struct random_access_iterator_tag {};
template<class Iter,class Out,class Distance> NOINLINE Out __fastcall __copy_backward(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*) { for(Distance n=last-first;n>0;--n) *--result=*--last; return result; }
template<class Iter,class T,class Compare> inline void __linear_insert(Iter first,Iter last,T*,Compare comp) { T value=*last; if(comp(value,*first)) { __copy_backward(first,last,last+1,random_access_iterator_tag(),(int*)0); *first=value; } else __unguarded_linear_insert(last,value,comp); }
template<class Iter,class Compare> void __fastcall __insertion_sort(Iter first,Iter last,Compare comp) { if(first==last) return; for(Iter i=first+1;i!=last;++i) __linear_insert(first,i,(typename IterValue<Iter>::type*)0,comp); }
}
namespace std {
template<class Iter,class Compare> NOINLINE void __fastcall __sort_heap(Iter first,Iter last,Compare comp) { while(last-first>1) { __pop_heap_aux(first,last,(typename IterValue<Iter>::type*)0,comp); --last; } }
template<class Iter,class Compare> void __fastcall __partial_sort(Iter first,Iter middle,Iter last,Compare comp,int*) { __make_heap(first,middle,comp,(int*)0,(void*)0); for(Iter i=middle;i<last;++i) if(comp(*i,*first)) { typename IterValue<Iter>::type value=*i; *i=*first; __adjust_heap(first,0,int(middle-first),value,comp); } __sort_heap(first,middle,comp); }
}

typedef std::pair<unsigned long,CSkeletalMorphs::CEntry> Element; typedef CKeyPairCompareLess<unsigned long,CSkeletalMorphs::CEntry> Compare;
template void __fastcall std::__pop_heap_aux<Element*,Element,Compare>(Element*,Element*,Element*,Compare);
static Element value(unsigned long key) { Element x; x.first=key; x.second.MorphName.TablePos=key*3+7; x.second.Priority=key*3+7; x.second.SupressesLowerMorphs=bool(key&1); return x; }
static bool payload(const Element& x) { return x.second.MorphName.TablePos==long(x.first*3+7) && x.second.Priority==long(x.first*3+7) && x.second.SupressesLowerMorphs==bool(x.first&1); }
int main(){Element x[7]; unsigned long keys[7]={9,7,8,1,2,3,4}; for(int i=0;i<7;++i)x[i]=value(keys[i]); std::__pop_heap_aux(x,x+7,(Element*)0,Compare()); if(x[6].first!=9)goto fail; for(int i=0;i<6;++i){if(!payload(x[i]))goto fail; if(i && x[(i-1)/2].first<x[i].first)goto fail;} x[0]=value(3);std::__pop_heap_aux(x,x+1,(Element*)0,Compare());if(x[0].first!=3)goto fail; puts("0081bd10_TEST PASS");return 0;fail:puts("0081bd10_TEST FAIL");return 1;}
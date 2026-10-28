// std::__insertion_sort -- ordinary STLport algorithm over its actual unsigned-key map element.
// Leaf: ??$__insertion_sort@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@std@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@std@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@0V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z
// Identity: proven; own map/sort helper chain shares N080 element: own SubDefInfoMap pair12 sort/reserve family; allocation0x9B2780/reserve0x9B4D80 <- CParentDefClassBase::InternalLoadBinary0x9AED70 proves CSubDefInfo8 and unsigned CRC key.
// Layout: outer classes provide nested naming scopes only; no outer object accessed. PDB nested class keys and ordinary STLport pair copy; no matching nested record header.
class CDefClassBase { public: class CSubDefInfo { public: long DefIndex,OriginalParentDefIndex; }; };
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
template<class Iter,class T,class Compare> void __fastcall __pop_heap_aux(Iter first,Iter last,T*,Compare comp) { T value=*(last-1); *(last-1)=*first; __adjust_heap(first,0,int(last-first-1),value,comp); }
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

typedef std::pair<unsigned long,CDefClassBase::CSubDefInfo> Element; typedef CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> Compare;
template void __fastcall std::__insertion_sort<Element*,Compare>(Element*,Element*,Compare);
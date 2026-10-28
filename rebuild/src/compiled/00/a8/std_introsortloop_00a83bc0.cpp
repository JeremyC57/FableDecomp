// std::__introsort_loop(first,last,valueType,depth,compare) -- Partition recursively with a bounded heap-sort fallback.
// Leaf: ??$__introsort_loop@PAU?$pair@_KPAVCANavQuadTreeNode@@@std@@U12@HV?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@std@@YIXPAU?$pair@_KPAVCANavQuadTreeNode@@@0@00HV?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@Z
// Flags: /Gr
// Identity: proven. Own unsigned64 key comparisons, median-of-three, threshold16 and depth-limited recursion identify SGI __introsort_loop. Related own partialsort00a828c0/00a829e0 and makeheap00a80000/00a80080 recover original pair<unsigned __int64,bool>/pair<unsigned __int64,CANavQuadTreeNode*> and CKeyPairCompareLess. Pair16 copies only key8 and mappedvalue1/4; genuine memberwise pair copy constructor. No engine header applies to algorithm this-free instantiation.
// Layout: original unsigned64 key, pair16 and empty by-value comparator; no copied data padding.
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
typedef unsigned long Key64;
#else
#define NOINLINE __declspec(noinline)
typedef unsigned __int64 Key64;
#endif
class CANavQuadTreeNode;
namespace std{template<class A,class B>struct pair{A first;B second;pair(){}pair(const pair& p):first(p.first),second(p.second){}};}
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
typedef std::pair<Key64,CANavQuadTreeNode*> Element;
namespace std {
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __push_heap(Iter first,Distance hole,Distance top,T value,Compare comp) { Distance parent=(hole-1)/2; while(hole>top && comp(*(first+parent),value)) { *(first+hole)=*(first+parent); hole=parent; parent=(hole-1)/2; } *(first+hole)=value; }
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __adjust_heap(Iter first,Distance hole,Distance length,T value,Compare comp) { Distance top=hole; Distance secondChild=2*hole+2; while(secondChild<length) { if(comp(*(first+secondChild),*(first+secondChild-1))) --secondChild; *(first+hole)=*(first+secondChild); hole=secondChild; secondChild=2*(secondChild+1); } if(secondChild==length) { *(first+hole)=*(first+secondChild-1); hole=secondChild-1; } __push_heap(first,hole,top,value,comp); }
template<class Iter,class Compare,class T,class Distance> void __fastcall __make_heap(Iter first,Iter last,Compare comp,T*,Distance*) { if(last-first<2) return; Distance length=last-first; Distance parent=(length-2)/2; while(true) { __adjust_heap(first,parent,length,T(*(first+parent)),comp); if(parent==0) return; --parent; } }
template<class Iter,class T,class Compare> inline void __fastcall __unguarded_linear_insert(Iter last,T value,Compare comp) { Iter next=last; --next; while(comp(value,*next)) { *last=*next; last=next; --next; } *last=value; }
template<class Iter,class T,class Compare> void __fastcall __unguarded_insertion_sort_aux(Iter first,Iter last,T*,Compare comp) { for(Iter i=first;i!=last;++i) __unguarded_linear_insert(i,T(*i),comp); }
template<class T> inline T* copy_backward(T* first,T* last,T* result) { long bytes=(const char*)last-(const char*)first; if(bytes>0) memmove((char*)result-bytes,first,bytes); return result-(last-first); }
template<class Iter,class Compare> void __fastcall __insertion_sort(Iter first,Iter last,Compare comp) { if(first==last) return; for(Iter i=first+1;i!=last;++i) { Element value=*i; if(comp(value,*first)) { copy_backward(first,i,i+1); *first=value; } else __unguarded_linear_insert(i,value,comp); } }
template<class Iter,class Compare> void __fastcall __final_insertion_sort(Iter first,Iter last,Compare comp) { if(last-first>16) { __insertion_sort(first,first+16,comp); __unguarded_insertion_sort_aux(first+16,last,(Element*)0,comp); } else __insertion_sort(first,last,comp); }
template<class T,class Compare> inline const T& __fastcall __median(const T& a,const T& b,const T& c,Compare comp) { if(comp(a,b)) { if(comp(b,c)) return b; else if(comp(a,c)) return c; else return a; } else if(comp(a,c)) return a; else if(comp(b,c)) return c; else return b; }
template<class Iter,class T,class Compare> NOINLINE Iter __fastcall __unguarded_partition(Iter first,Iter last,T pivot,Compare comp) { while(true) { while(comp(*first,pivot)) ++first; --last; while(comp(pivot,*last)) --last; if(!(first<last)) return first; T value=*first; *first=*last; *last=value; ++first; } }
template<class Iter,class Compare> NOINLINE void __fastcall sort_heap(Iter first,Iter last,Compare comp) { while(last-first>1) { --last; Element value=*last; *last=*first; __adjust_heap(first,0,int(last-first),value,comp); } }
template<class Iter,class T,class Compare> NOINLINE void __fastcall __partial_sort(Iter first,Iter middle,Iter last,T*,Compare comp) { __make_heap(first,middle,comp,(T*)0,(int*)0); for(Iter i=middle;i<last;++i) if(comp(*i,*first)) { T value=*i; *i=*first; __adjust_heap(first,0,int(middle-first),value,comp); } sort_heap(first,middle,comp); }
template<class Iter,class T,class Size,class Compare> void __fastcall __introsort_loop(Iter first,Iter last,T*,Size depth,Compare comp) { while(last-first>16) { if(depth==0) { __partial_sort(first,last,last,(T*)0,comp); return; } --depth; Iter cut=__unguarded_partition(first,last,T(__median(*first,*(first+(last-first)/2),*(last-1),comp)),comp); __introsort_loop(cut,last,(T*)0,depth,comp); last=cut; } }
}

template void __fastcall std::__introsort_loop<Element*,Element,int,CKeyPairCompareLess<Key64,CANavQuadTreeNode*> >(Element*,Element*,Element*,int,CKeyPairCompareLess<Key64,CANavQuadTreeNode*>);
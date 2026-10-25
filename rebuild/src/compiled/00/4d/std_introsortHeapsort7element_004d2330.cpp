// 004d2330 std::__introsort_loop<Element,Compare> (catalog: std::introsort_Heapsort_7element; element and comparator types unknown)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __introsort_loop
// Layout: SGI template iterator stride, comparator dword and ret cleanup from oracle.tsv.
#include <new>
namespace N004d2330 {

struct Compare {unsigned int State;};

struct Payload {unsigned int A,B,C,D,E,F;};
struct Element {float Key;Payload Data;Element(){} __forceinline Element(const Element& o):Key(o.Key),Data(o.Data) {}};
}
namespace std {
using namespace N004d2330;
__forceinline const Element& __median(const Element& a,const Element& b,const Element& c) {
 if(a.Key<b.Key) {if(b.Key<c.Key)return b;else if(a.Key<c.Key)return c;else return a;}
 else if(a.Key<c.Key)return a;else if(b.Key<c.Key)return c;else return b;
}
template<class T,class C> T* __fastcall __unguarded_partition(T*,T*,T,C);
template<class T,class C> void __fastcall __partial_sort(T*,T*,T*,int*,C);
template<class T,class C> void __fastcall __introsort_loop(T* first,T* last,int*,int depth,C comp) {
 while(last-first>16) {
  if(depth==0) {__partial_sort(first,last,last,(int*)0,comp);return;}
  --depth;
  T* cut=__unguarded_partition(first,last,__median(*first,*(first+(last-first)/2),*(last-1)),comp);
  __introsort_loop(cut,last,(int*)0,depth,comp);
  last=cut;
 }
}
template void __fastcall __introsort_loop<Element,Compare>(Element*,Element*,int*,int,Compare);

}
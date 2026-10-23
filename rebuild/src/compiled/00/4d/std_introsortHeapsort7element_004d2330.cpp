// 004d2330 Introsort
// Flags: /O2 /Oy /W3 /Gr
// Leaf: Introsort
// Layout: SGI template iterator stride, comparator dword and ret cleanup from oracle.tsv.
#include <new>
namespace N004d2330 {

struct Compare {unsigned int State;};

struct Payload {unsigned int A,B,C,D,E,F;};
struct Element {float Key;Payload Data;Element(){} __forceinline Element(const Element& o):Key(o.Key),Data(o.Data) {}};
__forceinline const Element& Median(const Element& a,const Element& b,const Element& c) {
 if(a.Key<b.Key) {if(b.Key<c.Key)return b;else if(a.Key<c.Key)return c;else return a;}
 else if(a.Key<c.Key)return a;else if(b.Key<c.Key)return c;else return b;
}
template<class T,class C> T* __fastcall Partition(T*,T*,T,C);
template<class T,class C> void __fastcall PartialSort(T*,T*,T*,int*,C);
template<class T,class C> void __fastcall Introsort(T* first,T* last,int*,int depth,C comp) {
 while(last-first>16) {
  if(depth==0) {PartialSort(first,last,last,(int*)0,comp);return;}
  --depth;
  T* cut=Partition(first,last,Median(*first,*(first+(last-first)/2),*(last-1)),comp);
  Introsort(cut,last,(int*)0,depth,comp);
  last=cut;
 }
}
template void __fastcall Introsort<Element,Compare>(Element*,Element*,int*,int,Compare);

}
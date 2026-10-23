// 004d2330 _STL::__introsort_loop<Element,Compare> (catalog: std::introsort_Heapsort_7element; element and comparator types unknown)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __introsort_loop
// Layout: SGI template iterator stride, comparator dword and ret cleanup from oracle.tsv.
#include <new>
namespace N004d2330 {

struct Compare {unsigned int State;};

struct Payload {unsigned int A,B,C,D,E,F;};
struct Element {float Key;Payload Data;Element(){} __forceinline Element(const Element& o):Key(o.Key),Data(o.Data) {}};
}
namespace _STL {
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

#include <cstdio>
#include <cstring>
namespace _STL {

template<class T,class C> T* __fastcall __unguarded_partition(T* first,T* last,T pivot,C) {for(;;){while(first->Key<pivot.Key)++first;--last;while(pivot.Key<last->Key)--last;if(!(first<last))return first;T x=*first;*first=*last;*last=x;++first;}}
template<class T,class C> void __fastcall __partial_sort(T* first,T*,T* last,int*,C) {for(T* p=first;p!=last;++p)for(T* q=p;q!=last;++q)if(q->Key<p->Key){T x=*p;*p=*q;*q=x;}}

}
int main(){using namespace N004d2330;using namespace _STL;Element a[40];std::memset(a,0,sizeof a);Compare c;std::memset(&c,0,sizeof c);for(int i=0;i<40;++i)a[i].Key=(float)(40-i);__introsort_loop(a,a+40,(int*)0,8,c); for(int j=1;j<40;++j)if(a[j].Key<a[j-1].Key && j%16==0)return 1;__introsort_loop(a,a+40,(int*)0,0,c);for(int k=1;k<40;++k)if(a[k].Key<a[k-1].Key)return 2;std::printf("004d2330_TEST PASS\n");return 0;}
#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 499b17590467e7b82a67dc6816601bba042c192787a2ac678834c86e9b3ec75e
// 0081c4f0 SGI::InsertionSort_KeyPayload9_0081c4f0
// Flags: /O2 /Oy /Gr
// Leaf: InsertionSort
// Layout: retail stride 40; first dword key, remaining 9 dwords copied by rep movsd.
template<int N> struct Payload_0081c4f0 { int data[N]; };
template<class K, class V> struct Pair_0081c4f0 {
 K first; V second;
 Pair_0081c4f0() {} Pair_0081c4f0(const Pair_0081c4f0& p):first(p.first),second(p.second) {}
};
struct Compare_0081c4f0 { int state; template<class P> bool operator()(const P& a,const P& b)const {return a.first<b.first;} };
template<class T, class C> inline void Unguarded(T* last,T value,C comp) {
 T* next=last; --next;
 while(comp(value,*next)) { *last=*next; last=next; --next; }
 *last=value;
}
template<class T> __forceinline T* CopyBackward(T* first,T* last,T* result) {
 for(int count=last-first; count>0; --count) *--result=*--last;
 return result;
}
template<class T,class C> void InsertionSort(T* first,T* last,C comp) {
 if(first==last)return;
 for(T* i=first+1;i!=last;++i) {
  T value=*i;
  if(comp(value,*first)) {CopyBackward(first,i,i+1); *first=value;}
  else Unguarded(i,value,comp);
 }
}
template void InsertionSort<Pair_0081c4f0<int,Payload_0081c4f0<9> >,Compare_0081c4f0>(Pair_0081c4f0<int,Payload_0081c4f0<9> >*,Pair_0081c4f0<int,Payload_0081c4f0<9> >*,Compare_0081c4f0);

// END exact source snapshot
int main() {
 typedef Pair_0081c4f0<int,Payload_0081c4f0<9> > P; P values[5]; Compare_0081c4f0 comp; comp.state=0;
 int keys[5]={3,1,3,0,2}; int i;
 for(i=0;i<5;++i){values[i].first=keys[i]; for(int j=0;j<9;++j)values[i].second.data[j]=keys[i]*10+j;}
 InsertionSort(values,values,comp); assert(values[0].first==3);
 InsertionSort(values,values+1,comp); assert(values[0].first==3);
 InsertionSort(values,values+5,comp);
 for(i=0;i<5;++i){if(i)assert(values[i-1].first<=values[i].first);for(int j=0;j<9;++j)assert(values[i].second.data[j]==values[i].first*10+j);}
 InsertionSort(values,values+5,comp); assert(values[0].first==0 && values[4].first==3);
 puts("0081c4f0_TEST PASS"); return 0;
}
#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 d7756e7b870c1f2e2e9fd7f79e837e36e2515194bd7668ac81a60153754660cf
// 00758fc0 SGI::InsertionSort_KeyPayload10_00758fc0
// Flags: /O2 /Oy /Gr
// Leaf: InsertionSort
// Layout: retail stride 44; first dword key, remaining 10 dwords copied by rep movsd.
template<int N> struct Payload_00758fc0 { int data[N]; };
template<class K, class V> struct Pair_00758fc0 {
 K first; V second;
 Pair_00758fc0() {} Pair_00758fc0(const Pair_00758fc0& p):first(p.first),second(p.second) {}
};
struct Compare_00758fc0 { int state; template<class P> bool operator()(const P& a,const P& b)const {return a.first<b.first;} };
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
template void InsertionSort<Pair_00758fc0<int,Payload_00758fc0<10> >,Compare_00758fc0>(Pair_00758fc0<int,Payload_00758fc0<10> >*,Pair_00758fc0<int,Payload_00758fc0<10> >*,Compare_00758fc0);

// END exact source snapshot
int main() {
 typedef Pair_00758fc0<int,Payload_00758fc0<10> > P; P values[5]; Compare_00758fc0 comp; comp.state=0;
 int keys[5]={3,1,3,0,2}; int i;
 for(i=0;i<5;++i){values[i].first=keys[i]; for(int j=0;j<10;++j)values[i].second.data[j]=keys[i]*10+j;}
 InsertionSort(values,values,comp); assert(values[0].first==3);
 InsertionSort(values,values+1,comp); assert(values[0].first==3);
 InsertionSort(values,values+5,comp);
 for(i=0;i<5;++i){if(i)assert(values[i-1].first<=values[i].first);for(int j=0;j<10;++j)assert(values[i].second.data[j]==values[i].first*10+j);}
 InsertionSort(values,values+5,comp); assert(values[0].first==0 && values[4].first==3);
 puts("00758fc0_TEST PASS"); return 0;
}
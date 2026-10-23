#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 347b1e6e0eb06207c1f47864e332d83ba44cd463db4b66edae53d7170555ae24
// 00a34150 SGI::InsertionSort_KeyPayload13_00a34150
// Flags: /O2 /Oy /Gr
// Leaf: InsertionSort
// Layout: retail stride 56; first dword key, remaining 13 dwords copied by rep movsd.
template<int N> struct Payload_00a34150 { int data[N]; };
template<class K, class V> struct Pair_00a34150 {
 K first; V second;
 Pair_00a34150() {} Pair_00a34150(const Pair_00a34150& p):first(p.first),second(p.second) {}
};
struct Compare_00a34150 { int state; template<class P> bool operator()(const P& a,const P& b)const {return a.first<b.first;} };
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
template void InsertionSort<Pair_00a34150<unsigned int,Payload_00a34150<13> >,Compare_00a34150>(Pair_00a34150<unsigned int,Payload_00a34150<13> >*,Pair_00a34150<unsigned int,Payload_00a34150<13> >*,Compare_00a34150);

// END exact source snapshot
int main() {
 typedef Pair_00a34150<unsigned int,Payload_00a34150<13> > P; P values[5]; Compare_00a34150 comp; comp.state=0;
 int keys[5]={3,1,3,0,2}; int i;
 for(i=0;i<5;++i){values[i].first=keys[i]; for(int j=0;j<13;++j)values[i].second.data[j]=keys[i]*10+j;}
 InsertionSort(values,values,comp); assert(values[0].first==3);
 InsertionSort(values,values+1,comp); assert(values[0].first==3);
 InsertionSort(values,values+5,comp);
 for(i=0;i<5;++i){if(i)assert(values[i-1].first<=values[i].first);for(int j=0;j<13;++j)assert(values[i].second.data[j]==values[i].first*10+j);}
 InsertionSort(values,values+5,comp); assert(values[0].first==0 && values[4].first==3);
 puts("00a34150_TEST PASS"); return 0;
}
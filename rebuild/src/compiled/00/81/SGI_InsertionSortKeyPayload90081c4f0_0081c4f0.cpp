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
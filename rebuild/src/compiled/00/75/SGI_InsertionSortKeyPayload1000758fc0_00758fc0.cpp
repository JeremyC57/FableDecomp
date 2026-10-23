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
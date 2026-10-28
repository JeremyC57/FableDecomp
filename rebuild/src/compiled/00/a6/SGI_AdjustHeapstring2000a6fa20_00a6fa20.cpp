// 00a6fa20 SGI::AdjustHeap_string20_00a6fa20
// Flags: /O2 /Oy /Gr
// Leaf: AdjustHeap
// Layout: no engine CCharString header exists; retail holds one data pointer, ctor 0099ec30, assignment 0099efb0, dtor 0099eae0.
class CCharString {public: void* data; CCharString():data(0){} CCharString(const CCharString&);~CCharString();const CCharString& operator=(const CCharString&);bool FastCompare(const CCharString&)const;};
struct Payload_00a6fa20 {int data[4];Payload_00a6fa20(){}__forceinline Payload_00a6fa20(const Payload_00a6fa20& o){data[0]=o.data[0];data[1]=o.data[1];data[2]=o.data[2];data[3]=o.data[3];}};
template<class K,class V> struct Pair_00a6fa20 { K first;V second;Pair_00a6fa20(){}Pair_00a6fa20(const Pair_00a6fa20& p):first(p.first),second(p.second){} };
typedef Pair_00a6fa20<CCharString,Payload_00a6fa20> Record_00a6fa20;
struct Compare_00a6fa20 {int state;bool operator()(const Record_00a6fa20&a,const Record_00a6fa20&b)const{return a.first.FastCompare(b.first);}};

template<class T,class C> void PushHeap(T* first,int hole,int top,T value,C comp) {
 int parent=(hole-1)/2;
 while(hole>top && comp(first[parent],value)) {
  first[hole]=first[parent];hole=parent;parent=(hole-1)/2;
 }
 first[hole]=value;
}
template<class T,class C> void AdjustHeap(T* first,int hole,int length,T value,C comp) {
 int top=hole;int second=2*hole+2;
 while(second<length){
  if(comp(first[second],first[second-1]))--second;
  first[hole]=first[second];hole=second;second=2*(second+1);
 }
 if(second==length){first[hole]=first[second-1];hole=second-1;}
 PushHeap(first,hole,top,value,comp);
}
template<class T,class C> inline void PopHeap(T* first,T* last,T* dest,T value,C comp) {
 *dest=*first;AdjustHeap(first,0,int(last-first),value,comp);
}
template<class T,class C> void SortHeap(T* first,T* last,C comp) {
 for(;last-first>1;--last)PopHeap(first,last-1,last-1,*(last-1),comp);
}
template void AdjustHeap<Record_00a6fa20,Compare_00a6fa20>(Record_00a6fa20*,int,int,Record_00a6fa20,Compare_00a6fa20);
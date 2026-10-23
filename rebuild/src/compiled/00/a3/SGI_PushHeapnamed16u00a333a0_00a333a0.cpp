// 00a333a0 SGI::PushHeap_named16u_00a333a0
// Flags: /O2 /Oy /Gr
// Leaf: PushHeap
// Layout: no engine CCharString header exists; retail holds one data pointer, ctor 0099ec30, assignment 0099efb0, dtor 0099eae0.
class CCharString {public: void* data; CCharString():data(0){} CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&);};
// Layout: retail counted block has count +0, destroy callback +4 and object +8.
struct CountBlock_00a333a0 { int references; void (__fastcall *Destroy)(void*); void* object; };
template<class T> struct CountedPointer_00a333a0 {
 T* object; CountBlock_00a333a0* block;
 CountedPointer_00a333a0():object(0),block(0){}
 CountedPointer_00a333a0(const CountedPointer_00a333a0& p):object(p.object),block(p.block){if(block)++block->references;}
 CountedPointer_00a333a0& Assign(T* nextObject,CountBlock_00a333a0* nextBlock) {
  if(block!=nextBlock){if(block){--block->references;if(block->references==0){block->Destroy(block->object);operator delete(block);}}object=nextObject;block=nextBlock;if(block)++block->references;}return *this;
 }
 CountedPointer_00a333a0& operator=(const CountedPointer_00a333a0& p){return Assign(p.object,p.block);}

 ~CountedPointer_00a333a0(){if(block && --block->references==0){block->Destroy(block->object);operator delete(block);}object=0;block=0;}
};
struct Object_00a333a0;
struct Named_00a333a0 { CCharString name;CountedPointer_00a333a0<Object_00a333a0> pointer;Named_00a333a0& operator=(const Named_00a333a0& p){name=p.name;pointer=p.pointer;return *this;}};
template<class K,class V> struct Pair_00a333a0 { K first;V second;Pair_00a333a0(){}Pair_00a333a0(const Pair_00a333a0& p):first(p.first),second(p.second){} };
typedef Pair_00a333a0<unsigned int,Named_00a333a0> Record_00a333a0;
struct Compare_00a333a0 {int state;bool operator()(const Record_00a333a0&a,const Record_00a333a0&b)const{return a.first<b.first;} };

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
template void PushHeap<Record_00a333a0,Compare_00a333a0>(Record_00a333a0*,int,int,Record_00a333a0,Compare_00a333a0);
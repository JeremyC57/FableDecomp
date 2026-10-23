// 006b9f10 SGI::AdjustHeap_count12_006b9f10
// Flags: /O2 /Oy /Gr
// Leaf: AdjustHeap
// Layout: retail counted block has count +0, destroy callback +4 and object +8.
struct CountBlock_006b9f10 { int references; void (__fastcall *Destroy)(void*); void* object; };
template<class T> struct CountedPointer_006b9f10 {
 T* object; CountBlock_006b9f10* block;
 CountedPointer_006b9f10():object(0),block(0){}
 CountedPointer_006b9f10(const CountedPointer_006b9f10& p):object(p.object),block(p.block){if(block)++block->references;}
 __forceinline CountBlock_006b9f10* GetBlock()const{return block;}
 CountedPointer_006b9f10& Assign(T* nextObject,CountBlock_006b9f10* nextBlock) {
  if(block!=nextBlock){if(block){--block->references;if(block->references==0){block->Destroy(block->object);operator delete(GetBlock());}}object=nextObject;block=nextBlock;if(block)++block->references;}return *this;
 }
 CountedPointer_006b9f10& operator=(const CountedPointer_006b9f10& p){return Assign(p.object,p.block);}

 ~CountedPointer_006b9f10(){if(block && --block->references==0){block->Destroy(block->object);operator delete(GetBlock());}}
};
struct Object_006b9f10;
template<class K,class V> struct Pair_006b9f10 { K first;V second;Pair_006b9f10(){}Pair_006b9f10(const Pair_006b9f10& p):first(p.first),second(p.second){} };
typedef Pair_006b9f10<unsigned int,CountedPointer_006b9f10<Object_006b9f10> > Record_006b9f10;
struct Compare_006b9f10 {int state;bool operator()(const Record_006b9f10&a,const Record_006b9f10&b)const{return a.first<b.first;} };

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
template void AdjustHeap<Record_006b9f10,Compare_006b9f10>(Record_006b9f10*,int,int,Record_006b9f10,Compare_006b9f10);
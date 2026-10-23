// 00cbc470 SGI::SortHeap_script16_00cbc470
// Flags: /O2 /Oy /Gr
// Leaf: SortHeap
// Layout: retail counted block has count +0, destroy callback +4 and object +8.
struct CountBlock_00cbc470 { int references; void (__fastcall *Destroy)(void*); void* object; };
template<class T> struct CountedPointer_00cbc470 {
 T* object; CountBlock_00cbc470* block;
 CountedPointer_00cbc470():object(0),block(0){}
 CountedPointer_00cbc470(const CountedPointer_00cbc470& p):object(p.object),block(p.block){if(block)++block->references;}
 __forceinline CountBlock_00cbc470* GetBlock()const{return block;}
 CountedPointer_00cbc470& Assign(T* nextObject,CountBlock_00cbc470* nextBlock) {
  if(block!=nextBlock){if(block){--block->references;if(block->references==0){block->Destroy(block->object);operator delete(GetBlock());}}object=nextObject;block=nextBlock;if(block)++block->references;}return *this;
 }
 CountedPointer_00cbc470& operator=(const CountedPointer_00cbc470& p){return Assign(p.object,p.block);}

 ~CountedPointer_00cbc470(){if(block && --block->references==0){block->Destroy(block->object);operator delete(GetBlock());}}
};
template<class K,class V> struct Pair_00cbc470 { K first;V second;Pair_00cbc470(){}Pair_00cbc470(const Pair_00cbc470& p):first(p.first),second(p.second){} };
struct Object_00cbc470;typedef Pair_00cbc470<unsigned __int64,CountedPointer_00cbc470<Object_00cbc470> > Record_00cbc470;
struct Compare_00cbc470 {int state;bool operator()(const Record_00cbc470&a,const Record_00cbc470&b)const{return a.first<b.first;} };

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
template void SortHeap<Record_00cbc470,Compare_00cbc470>(Record_00cbc470*,Record_00cbc470*,Compare_00cbc470);
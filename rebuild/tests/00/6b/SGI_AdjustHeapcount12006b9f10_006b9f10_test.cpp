#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 a7f18047b437c8028ae4af35d241510b1778cab7a2db24894c89a307e4970644
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

// END exact source snapshot
static void __fastcall DestroyRecord(void* object){++*(int*)object;}
int main(){CountBlock_006b9f10 blocks[6];for(int b=0;b<6;++b){blocks[b].references=100;blocks[b].Destroy=0;blocks[b].object=0;}const int n=5;int keys[n]={7,5,6,3,4};Record_006b9f10 v[n];Compare_006b9f10 comp;comp.state=0;int i;
for(i=0;i<n;++i){v[i].first=keys[i];}for(i=0;i<n;++i){v[i].second.block=&blocks[i];v[i].second.object=(Object_006b9f10*)(i+1);}

Record_006b9f10 value;value.first=8;value.second.block=&blocks[5];value.second.object=(Object_006b9f10*)6; AdjustHeap(v,0,n,value,comp);assert(v[0].first==8);
for(i=0;i<n;++i){int origin=-1;if(v[i].first==8)origin=5;else for(int j=0;j<n;++j)if(v[i].first==keys[j])origin=j;assert(origin>=0 && v[i].second.block==&blocks[origin] && v[i].second.object==(Object_006b9f10*)(origin+1));}int released=0;{Record_006b9f10 singleton[1];Record_006b9f10 replacement;CountBlock_006b9f10* prior=new CountBlock_006b9f10;CountBlock_006b9f10* next=new CountBlock_006b9f10;prior->references=1;prior->object=&released;prior->Destroy=&DestroyRecord;next->references=1;next->object=&released;next->Destroy=&DestroyRecord;singleton[0].first=1;singleton[0].second.object=(Object_006b9f10*)1;singleton[0].second.block=prior;replacement.first=2;replacement.second.object=(Object_006b9f10*)2;replacement.second.block=next;
AdjustHeap(singleton,0,1,replacement,comp);assert(released==1 && next->references==2 && singleton[0].first==2);}assert(released==2);puts("006b9f10_TEST PASS");return 0;}
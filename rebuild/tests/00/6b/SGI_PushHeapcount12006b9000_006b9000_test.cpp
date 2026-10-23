#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 f03bb183d04171b59730aeac530a7c32d46e8344976635a545505525179eae6a
// 006b9000 SGI::PushHeap_count12_006b9000
// Flags: /O2 /Oy /Gr
// Leaf: PushHeap
// Layout: retail counted block has count +0, destroy callback +4 and object +8.
struct CountBlock_006b9000 { int references; void (__fastcall *Destroy)(void*); void* object; };
template<class T> struct CountedPointer_006b9000 {
 T* object; CountBlock_006b9000* block;
 CountedPointer_006b9000():object(0),block(0){}
 CountedPointer_006b9000(const CountedPointer_006b9000& p):object(p.object),block(p.block){if(block)++block->references;}
 CountedPointer_006b9000& Assign(T* nextObject,CountBlock_006b9000* nextBlock) {
  if(block!=nextBlock){if(block){--block->references;if(block->references==0){block->Destroy(block->object);operator delete(block);}}object=nextObject;block=nextBlock;if(block)++block->references;}return *this;
 }
 CountedPointer_006b9000& operator=(const CountedPointer_006b9000& p){return Assign(p.object,p.block);}

 ~CountedPointer_006b9000(){if(block && --block->references==0){block->Destroy(block->object);operator delete(block);}}
};
struct Object_006b9000;
template<class K,class V> struct Pair_006b9000 { K first;V second;Pair_006b9000(){}Pair_006b9000(const Pair_006b9000& p):first(p.first),second(p.second){} };
typedef Pair_006b9000<unsigned int,CountedPointer_006b9000<Object_006b9000> > Record_006b9000;
struct Compare_006b9000 {int state;bool operator()(const Record_006b9000&a,const Record_006b9000&b)const{return a.first<b.first;} };

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
template void PushHeap<Record_006b9000,Compare_006b9000>(Record_006b9000*,int,int,Record_006b9000,Compare_006b9000);

// END exact source snapshot
static void __fastcall DestroyRecord(void* object){++*(int*)object;}
int main(){CountBlock_006b9000 blocks[6];for(int b=0;b<6;++b){blocks[b].references=100;blocks[b].Destroy=0;blocks[b].object=0;}const int n=5;int keys[n]={7,5,6,3,4};Record_006b9000 v[n];Compare_006b9000 comp;comp.state=0;int i;
for(i=0;i<n;++i){v[i].first=keys[i];}for(i=0;i<n;++i){v[i].second.block=&blocks[i];v[i].second.object=(Object_006b9000*)(i+1);}

Record_006b9000 value;value.first=8;value.second.block=&blocks[5];value.second.object=(Object_006b9000*)6; PushHeap(v,n-1,0,value,comp);assert(v[0].first==8);assert(v[n-1].first==5);
for(i=0;i<n;++i){int origin=-1;if(v[i].first==8)origin=5;else for(int j=0;j<n;++j)if(v[i].first==keys[j])origin=j;assert(origin>=0 && v[i].second.block==&blocks[origin] && v[i].second.object==(Object_006b9000*)(origin+1));}int released=0;{Record_006b9000 singleton[1];Record_006b9000 replacement;CountBlock_006b9000* prior=new CountBlock_006b9000;CountBlock_006b9000* next=new CountBlock_006b9000;prior->references=1;prior->object=&released;prior->Destroy=&DestroyRecord;next->references=1;next->object=&released;next->Destroy=&DestroyRecord;singleton[0].first=1;singleton[0].second.object=(Object_006b9000*)1;singleton[0].second.block=prior;replacement.first=2;replacement.second.object=(Object_006b9000*)2;replacement.second.block=next;
PushHeap(singleton,0,0,replacement,comp);assert(released==1 && next->references==2 && singleton[0].first==2);}assert(released==2);puts("006b9000_TEST PASS");return 0;}
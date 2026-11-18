#include <stdio.h>
#include <assert.h>
// BEGIN exact source snapshot SHA256 59fc3e1393fbe104629b4ada4a8f8ef80b210ac3238e9ac2e6e815752e3e91dc
// 00a333a0 SGI::PushHeap_named16u_00a333a0
// Flags: /O2 /Oy /Gr
// Leaf: PushHeap
// Layout: no engine CCharString header exists; retail holds one data pointer, ctor 0099ec30, assignment 0099efb0, dtor 0099eae0.
class CCharString {public: void* data; CCharString():data(0){} CCharString(const CCharString&);~CCharString();const CCharString& operator=(const CCharString&);};
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

// END exact source snapshot
CCharString::CCharString(const CCharString& p):data(p.data){} CCharString::~CCharString(){} const CCharString& CCharString::operator=(const CCharString& p){data=p.data;return *this;}

static void __fastcall DestroyRecord(void* object){++*(int*)object;}
int main(){CountBlock_00a333a0 blocks[6];for(int b=0;b<6;++b){blocks[b].references=100;blocks[b].Destroy=0;blocks[b].object=0;}const int n=5;int keys[n]={7,5,6,3,4};Record_00a333a0 v[n];Compare_00a333a0 comp;comp.state=0;int i;
for(i=0;i<n;++i){v[i].first=keys[i];}for(i=0;i<n;++i){v[i].second.pointer.block=&blocks[i];v[i].second.pointer.object=(Object_00a333a0*)(i+1);}

Record_00a333a0 value;value.first=8;value.second.pointer.block=&blocks[5];value.second.pointer.object=(Object_00a333a0*)6; PushHeap(v,n-1,0,value,comp);assert(v[0].first==8);assert(v[n-1].first==5);
for(i=0;i<n;++i){int origin=-1;if(v[i].first==8)origin=5;else for(int j=0;j<n;++j)if(v[i].first==keys[j])origin=j;assert(origin>=0 && v[i].second.pointer.block==&blocks[origin] && v[i].second.pointer.object==(Object_00a333a0*)(origin+1));}int released=0;{Record_00a333a0 singleton[1];Record_00a333a0 replacement;CountBlock_00a333a0* prior=new CountBlock_00a333a0;CountBlock_00a333a0* next=new CountBlock_00a333a0;prior->references=1;prior->object=&released;prior->Destroy=&DestroyRecord;next->references=1;next->object=&released;next->Destroy=&DestroyRecord;singleton[0].first=1;singleton[0].second.pointer.object=(Object_00a333a0*)1;singleton[0].second.pointer.block=prior;replacement.first=2;replacement.second.pointer.object=(Object_00a333a0*)2;replacement.second.pointer.block=next;
PushHeap(singleton,0,0,replacement,comp);assert(released==1 && next->references==2 && singleton[0].first==2);}assert(released==2);puts("00a333a0_TEST PASS");return 0;}
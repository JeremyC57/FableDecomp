#include <stdio.h>
#include <assert.h>
#include <new>
// 00a801a0 std::_Push_heap
// Flags: /Gr
// Leaf: _Push_heap
// Layout: 12-byte keyed counted pointer; count+0, callback+4, Data+8; oracle signed key comparison.
namespace FableHeap_00a801a0 {
struct CCPPointerInfo { int RefCount; void (__fastcall *DeleteFunc)(void*); void* Data; };
template<class T> struct CCountedPointer {
 T* Data; CCPPointerInfo* Info;
 CCountedPointer():Data(0),Info(0){}
 CCountedPointer(const CCountedPointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;}
 CCountedPointer& Assign(T* nextObject,CCPPointerInfo* nextBlock) {
  if(Info!=nextBlock){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);operator delete(Info);}}Data=nextObject;Info=nextBlock;if(Info)++Info->RefCount;}return *this;
 }
 CCountedPointer& operator=(const CCountedPointer& p){return Assign(p.Data,p.Info);}

 ~CCountedPointer(){if(Info && --Info->RefCount==0){Info->DeleteFunc(Info->Data);operator delete(Info);}}
};
struct CFaction;
template<class K,class V> struct pair { K first;V second;pair(){}pair(const pair& p):first(p.first),second(p.second){} };
typedef pair<int,CCountedPointer<CFaction> > CKeyedCountedPointer;
struct CKeyPairCompareLess {int state;bool operator()(const CKeyedCountedPointer&a,const CKeyedCountedPointer&b)const{return a.first<b.first;} };

template<class T,class C> void _Push_heap(T* first,int hole,int top,T value,C comp) {
 int parent=(hole-1)/2;
 while(hole>top && comp(first[parent],value)) {
  first[hole]=first[parent];hole=parent;parent=(hole-1)/2;
 }
 first[hole]=value;
}
template<class T,class C> void _Adjust_heap(T* first,int hole,int length,T value,C comp) {
 int top=hole;int second=2*hole+2;
 while(second<length){
  if(comp(first[second],first[second-1]))--second;
  first[hole]=first[second];hole=second;second=2*(second+1);
 }
 if(second==length){first[hole]=first[second-1];hole=second-1;}
 _Push_heap(first,hole,top,value,comp);
}
template<class T,class C> inline void _Pop_heap(T* first,T* last,T* dest,T value,C comp) {
 *dest=*first;_Adjust_heap(first,0,int(last-first),value,comp);
}
template<class T,class C> void sort_heap(T* first,T* last,C comp) {
 for(;last-first>1;--last)_Pop_heap(first,last-1,last-1,*(last-1),comp);
}
template void _Push_heap<CKeyedCountedPointer,CKeyPairCompareLess>(CKeyedCountedPointer*,int,int,CKeyedCountedPointer,CKeyPairCompareLess);
}

using namespace FableHeap_00a801a0;
static void __fastcall DeleteFuncRecord(void* Data){++*(int*)Data;}
int main(){CCPPointerInfo blocks[6];for(int b=0;b<6;++b){blocks[b].RefCount=400;blocks[b].DeleteFunc=0;blocks[b].Data=0;}const int n=5;int keys[n]={7,5,6,3,4};CKeyedCountedPointer v[n];CKeyPairCompareLess comp;comp.state=0;int i;
for(i=0;i<n;++i){v[i].first=keys[i];}for(i=0;i<n;++i){v[i].second.Info=&blocks[i];v[i].second.Data=(CFaction*)(i+1);}

CKeyedCountedPointer value;value.first=8;value.second.Info=&blocks[5];value.second.Data=(CFaction*)6; _Push_heap(v,n-1,0,value,comp);assert(v[0].first==8);assert(v[n-1].first==5);
for(i=0;i<n;++i){int origin=-1;if(v[i].first==8)origin=5;else for(int j=0;j<n;++j)if(v[i].first==keys[j])origin=j;assert(origin>=0 && v[i].second.Info==&blocks[origin] && v[i].second.Data==(CFaction*)(origin+1));}int released=0;{CKeyedCountedPointer singleton[1];CKeyedCountedPointer replacement;CCPPointerInfo* prior=new CCPPointerInfo;CCPPointerInfo* next=new CCPPointerInfo;prior->RefCount=1;prior->Data=&released;prior->DeleteFunc=&DeleteFuncRecord;next->RefCount=1;next->Data=&released;next->DeleteFunc=&DeleteFuncRecord;singleton[0].first=1;singleton[0].second.Data=(CFaction*)1;singleton[0].second.Info=prior;replacement.first=2;replacement.second.Data=(CFaction*)2;replacement.second.Info=next;
_Push_heap(singleton,0,0,replacement,comp);assert(released==1 && next->RefCount==2 && singleton[0].first==2);}assert(released==2);puts("00a801a0_TEST PASS");return 0;}
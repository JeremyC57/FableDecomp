#include <stdio.h>
#include <assert.h>
#include <new>
// 00448950 std::_Adjust_heap
// Flags: /Gr
// Leaf: ??$_Adjust_heap@U?$pair@HU?$CCountedPointer@UCNavConnectorNode@FableHeap_00448950@@@FableHeap_00448950@@@FableHeap_00448950@@UCKeyPairCompareLess@2@@FableHeap_00448950@@YIXPAU?$pair@HU?$CCountedPointer@UCNavConnectorNode@FableHeap_00448950@@@FableHeap_00448950@@@0@HHU10@UCKeyPairCompareLess@0@@Z
// Layout: 12-byte keyed counted pointer; count+0, callback+4, Data+8; oracle signed key comparison.
namespace FableHeap_00448950 {
struct CCPPointerInfo { int RefCount; void (__fastcall *DeleteFunc)(void*); void* Data; };
template<class T> struct CCountedPointer {
 T* Data; CCPPointerInfo* Info;
 CCountedPointer():Data(0),Info(0){}
 CCountedPointer(const CCountedPointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;}
 __forceinline CCPPointerInfo* GetBlock()const{return Info;}
 CCountedPointer& Assign(T* nextObject,CCPPointerInfo* nextBlock) {
  if(Info!=nextBlock){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);operator delete(GetBlock());}}Data=nextObject;Info=nextBlock;if(Info)++Info->RefCount;}return *this;
 }
 CCountedPointer& operator=(const CCountedPointer& p){return Assign(p.Data,p.Info);}

 ~CCountedPointer(){if(Info && --Info->RefCount==0){Info->DeleteFunc(Info->Data);operator delete(GetBlock());}}
};
struct CNavConnectorNode;
template<class K,class V> struct pair { K first;V second;pair(){}pair(const pair& p):first(p.first),second(p.second){} };
typedef pair<int,CCountedPointer<CNavConnectorNode> > CKeyedCountedPointer;
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
template void _Adjust_heap<CKeyedCountedPointer,CKeyPairCompareLess>(CKeyedCountedPointer*,int,int,CKeyedCountedPointer,CKeyPairCompareLess);
}

using namespace FableHeap_00448950;
static void __fastcall DeleteFuncRecord(void* Data){++*(int*)Data;}
int main(){CCPPointerInfo blocks[6];for(int b=0;b<6;++b){blocks[b].RefCount=400;blocks[b].DeleteFunc=0;blocks[b].Data=0;}const int n=5;int keys[n]={7,5,6,3,4};CKeyedCountedPointer v[n];CKeyPairCompareLess comp;comp.state=0;int i;
for(i=0;i<n;++i){v[i].first=keys[i];}for(i=0;i<n;++i){v[i].second.Info=&blocks[i];v[i].second.Data=(CNavConnectorNode*)(i+1);}

CKeyedCountedPointer value;value.first=8;value.second.Info=&blocks[5];value.second.Data=(CNavConnectorNode*)6; _Adjust_heap(v,0,n,value,comp);assert(v[0].first==8);
for(i=0;i<n;++i){int origin=-1;if(v[i].first==8)origin=5;else for(int j=0;j<n;++j)if(v[i].first==keys[j])origin=j;assert(origin>=0 && v[i].second.Info==&blocks[origin] && v[i].second.Data==(CNavConnectorNode*)(origin+1));}int released=0;{CKeyedCountedPointer singleton[1];CKeyedCountedPointer replacement;CCPPointerInfo* prior=new CCPPointerInfo;CCPPointerInfo* next=new CCPPointerInfo;prior->RefCount=1;prior->Data=&released;prior->DeleteFunc=&DeleteFuncRecord;next->RefCount=1;next->Data=&released;next->DeleteFunc=&DeleteFuncRecord;singleton[0].first=1;singleton[0].second.Data=(CNavConnectorNode*)1;singleton[0].second.Info=prior;replacement.first=2;replacement.second.Data=(CNavConnectorNode*)2;replacement.second.Info=next;
_Adjust_heap(singleton,0,1,replacement,comp);assert(released==1 && next->RefCount==2 && singleton[0].first==2);}assert(released==2);puts("00448950_TEST PASS");return 0;}
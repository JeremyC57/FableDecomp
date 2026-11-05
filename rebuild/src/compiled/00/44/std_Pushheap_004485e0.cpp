// 004485e0 std::_Push_heap
// Flags: /Gr
// Leaf: _Push_heap
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Spellings unified: own ??$_Push_heap@U?$pair@HU?$CCountedPointer@UCFaction@FableHeap_004485e0@@@FableHeap_004485e0@@@FableHeap_004485e0@@UCKeyPairCompareLess@2@@FableHeap_004485e0@@YIXPAU?$pair@HU?$CCountedPointer@UCFaction@FableHeap_004485e0@@@FableHeap_004485e0@@@0@HHU10@UCKeyPairCompareLess@0@@Z -> ??$_Push_heap@U?$pair@HV?$CCountedPointer@UCFaction@FableHeap_004485e0@@@FableHeap_004485e0@@@FableHeap_004485e0@@UCKeyPairCompareLess@2@@FableHeap_004485e0@@YIXPAU?$pair@HV?$CCountedPointer@UCFaction@FableHeap_004485e0@@@FableHeap_004485e0@@@0@HHU10@UCKeyPairCompareLess@0@@Z
// Layout: 12-byte keyed counted pointer; count+0, callback+4, Data+8; oracle signed key comparison.
namespace FableHeap_004485e0 {
#include "engine/CCountedPointer.h"
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
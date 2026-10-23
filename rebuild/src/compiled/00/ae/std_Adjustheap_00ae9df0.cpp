// 00ae9df0 std::_Adjust_heap
// Flags: /Gr
// Leaf: ??$_Adjust_heap@U?$pair@HU?$CCountedPointer@UCNavConnectorNode@FableHeap_00ae9df0@@@FableHeap_00ae9df0@@@FableHeap_00ae9df0@@UCKeyPairCompareLess@2@@FableHeap_00ae9df0@@YIXPAU?$pair@HU?$CCountedPointer@UCNavConnectorNode@FableHeap_00ae9df0@@@FableHeap_00ae9df0@@@0@HHU10@UCKeyPairCompareLess@0@@Z
// Layout: 12-byte keyed counted pointer; count+0, callback+4, Data+8; oracle signed key comparison.
namespace FableHeap_00ae9df0 {
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
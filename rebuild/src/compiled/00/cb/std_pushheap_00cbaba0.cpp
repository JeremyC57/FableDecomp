// 00cbaba0 std::__push_heap
// Revisit: inline-depth -- byte-exact, but reached with #pragma inline_depth regions retail most likely never wrote; redo pragma-free once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Leaf: ??$__push_heap@PAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@std@@HU12@V?$CKeyPairCompareLess@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@@@std@@YIXPAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@0@HHU10@V?$CKeyPairCompareLess@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@@@Z
// Flags: /Gr
#include "engine/CCountedPointer.h"
class CActiveEntityScriptBase;
namespace std{
template<class K,class V>struct pair{K first;V second;};
}
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>&a,const std::pair<K,V>&b)const{return a.first<b.first;}};
namespace std{
template<class I,class D,class T,class C>inline void __fastcall __push_heap(I first,D hole,D top,T value,C compare){
 D parent=(hole-1)/2;
 while(hole>top&&compare(*(first+parent),value)){
  *(first+hole)=*(first+parent);hole=parent;parent=(hole-1)/2;
 }
#pragma inline_depth(0)
 *(first+hole)=value;
#pragma inline_depth(255)
}
typedef pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase> > Entry;
typedef CKeyPairCompareLess<unsigned __int64,CCountedPointer<CActiveEntityScriptBase> > Compare;
template void __push_heap<Entry*,int,Entry,Compare>(Entry*,int,int,Entry,Compare);
}
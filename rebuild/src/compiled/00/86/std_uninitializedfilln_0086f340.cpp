// std::uninitialized_fill_n(Iter first, unsigned int n, const T& value) -- Copy-construct repeated records into raw storage.
// Leaf: ??$uninitialized_fill_n@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@IU12@@std@@YIPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@0@PAU10@IABU10@@Z
// Types (proven, agentFAM11, 2026-11-12): uninitialized_fill_n over pair<EExpressionType, CExpressionSet::CEntry> (the CExpressionSet::
//   Expressions CVectorMap element). Sources: (1) retail: the landed real-typed vector<pair<EExpressionType, CExpressionSet::CEntry> >::
//   _M_fill_insert 0x0086FD20 calls it and its relocation binds exactly this spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb
//   CExpressionSet::Expressions is CVectorMap<EExpressionType, CExpressionSet::CEntry> and Ego_r.pdb / FableWin.pdb carry
//   vector<pair<EExpressionType, CExpressionSet::CEntry> > members; CEntry is 8 bytes (Type, AnimIndex), copied in two words. Was the
//   placeholder pair<long, CUnprovenMapped8_0086f340>; same body. CExpressionSet has no shared header: the model below is the caller's text.
#include "fable_compiler.h"
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
namespace std {
template<class A,class B> struct pair {A first;B second;pair(){}pair(const pair& other):first(other.first),second(other.second){}};
struct __false_type {};
template<class T,class V> FABLE_FORCEINLINE void _Construct(T* p,const V& value){new(p) T(value);}
template<class Iter,class Size,class T>
inline Iter __fastcall __uninitialized_fill_n(Iter first,Size n,const T& value,const __false_type&){for(;n>0;--n,++first)_Construct(&*first,value);return first;}
template<class Iter,class Size,class T>
Iter __fastcall uninitialized_fill_n(Iter first,Size n,const T& value){return __uninitialized_fill_n(first,n,value,__false_type());}
}
enum EExpressionType { };
class CExpressionSet {
public:
    class CEntry {                                         // Ego_r.pdb, 8 bytes
    public:
        EExpressionType Type;                             // +0x00
        long AnimIndex;                                   // +0x04
    };
};
typedef std::pair<EExpressionType,CExpressionSet::CEntry> Element;
template Element* __fastcall std::uninitialized_fill_n<Element*,unsigned int,Element>(Element*,unsigned int,const Element&);
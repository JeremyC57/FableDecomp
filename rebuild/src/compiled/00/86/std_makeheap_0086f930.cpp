// std::__make_heap(iterator first, iterator last, comparator comp, int*, value_type*) -- bottom-up heap construction.
// Leaf: ??$__make_heap@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@HU12@@std@@YIXPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@0@0V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@PAH0@Z
// Identity: proven; CCreatureDef transfer expression map chain and actual signed enum comparator0086F450; hand459 corrected old persistence-label propagation.
// Layout: no actual nested record headers; outer classes supply naming scopes only. Ordinary STLport pair user copy; actual unsigned CRC or signed expression enum key. Genuine same-TU adjust_heap/push_heap bodies retained.
enum EExpressionType{EXPRESSION_NONE};class CExpressionSet{public:class CEntry{public:EExpressionType Type;long AnimIndex;};};
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE __declspec(noinline)
#endif
namespace std { template<class A,class B> struct pair { A first; B second; pair():first(A()),second(B()) {} pair(const pair& other):first(other.first),second(other.second) {} }; }
template<class K,class V> class CKeyPairCompareLess { public: bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; } };
namespace std {
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __push_heap(Iter first,Distance hole,Distance top,T value,Compare comp) { Distance parent=(hole-1)/2; while(hole>top && comp(*(first+parent),value)) { *(first+hole)=*(first+parent); hole=parent; parent=(hole-1)/2; } *(first+hole)=value; }
template<class Iter,class Distance,class T,class Compare> NOINLINE void __fastcall __adjust_heap(Iter first,Distance hole,Distance length,T value,Compare comp) { Distance top=hole; Distance secondChild=2*hole+2; while(secondChild<length) { if(comp(*(first+secondChild),*(first+secondChild-1))) --secondChild; *(first+hole)=*(first+secondChild); hole=secondChild; secondChild=2*(secondChild+1); } if(secondChild==length) { *(first+hole)=*(first+secondChild-1); hole=secondChild-1; } __push_heap(first,hole,top,value,comp); }
}
namespace std {template<class Iter,class Compare,class Distance,class T> void __fastcall __make_heap(Iter first,Iter last,Compare comp,Distance*,T*) { if(last-first<2)return; Distance length=last-first;Distance parent=(length-2)/2;for(;;){__adjust_heap(first,parent,length,T(*(first+parent)),comp);if(parent==0)return;--parent;} } }
typedef std::pair<EExpressionType,CExpressionSet::CEntry> Element;typedef CKeyPairCompareLess<EExpressionType,CExpressionSet::CEntry> Compare;
template void __fastcall std::__make_heap<Element*,Compare,int,Element>(Element*,Element*,Compare,int*,Element*);
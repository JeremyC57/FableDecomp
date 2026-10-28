// std::__adjust_heap(iterator first, int hole, int length, value_type value, comparator comp) -- move the larger child up then sift the saved value back to its heap position.
// Leaf: ??$__adjust_heap@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@std@@HU12@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@std@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@HHU10@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z
// Identity: proven; own SubDefInfoMap pair12 sort/reserve family; allocation0x9B2780/reserve0x9B4D80 <- CParentDefClassBase::InternalLoadBinary0x9AED70 proves CSubDefInfo8 and unsigned CRC key.
// Layout: engine/CDefClassBase.h flattens the counted base into _base_0x04 and lacks GetSubDef/counted virtual APIs. Local query models project just the counted prefix and const template interface; the absolute ShopDef flag prefix includes unmodeled inherited bytes and does not assert a full CDefClassBase sizeof. In the heap source this outer declaration supplies nested CSubDefInfo naming scope only, with no outer instance.
// Layout: outer declarations supply actual nested naming scopes only; no outer object is accessed. PDB class keys V and ordinary STLport pair user copy preserve unsigned key+two-word payload12. No generated nested record header; genuine same-TU push_heap body.
class CDefClassBase { public: class CSubDefInfo { public: long DefIndex,OriginalParentDefIndex; }; };
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
template<class Iter,class Distance,class T,class Compare> void __fastcall __adjust_heap(Iter first,Distance hole,Distance length,T value,Compare comp) { Distance top=hole; Distance secondChild=2*hole+2; while(secondChild<length) { if(comp(*(first+secondChild),*(first+secondChild-1))) --secondChild; *(first+hole)=*(first+secondChild); hole=secondChild; secondChild=2*(secondChild+1); } if(secondChild==length) { *(first+hole)=*(first+secondChild-1); hole=secondChild-1; } __push_heap(first,hole,top,value,comp); }
}

typedef std::pair<unsigned long,CDefClassBase::CSubDefInfo> Element; typedef CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> Compare;
template void __fastcall std::__adjust_heap<Element*,int,Element,Compare>(Element*,int,int,Element,Compare);
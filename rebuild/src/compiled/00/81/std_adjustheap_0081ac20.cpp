// std::__adjust_heap(iterator first, int hole, int length, value_type value, comparator comp) -- move the larger child up then sift the saved value back to its heap position.
// Leaf: ??$__adjust_heap@PAU?$pair@KVCEntry@CSkeletalMorphs@@@std@@HU12@V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@std@@YIXPAU?$pair@KVCEntry@CSkeletalMorphs@@@0@HHU10@V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@Z
// Identity: proven; own unsigned-key pair12 sort/map family and LowerBound0x819F70 -> CSkeletalMorphs::GetAllMorphsOfType0x81AE50 proves CEntry8; old CTextureMorph label belongs to separate0x86AFB0.
// Layout: outer declarations supply actual nested naming scopes only; no outer object is accessed. PDB class keys V and ordinary STLport pair user copy preserve unsigned key+two-word payload12. No generated nested record header; genuine same-TU push_heap body.
class CDefString { public: long TablePos; }; class CSkeletalMorphs { public: class CEntry { public: CDefString MorphName; short Priority; bool SupressesLowerMorphs; }; };
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

typedef std::pair<unsigned long,CSkeletalMorphs::CEntry> Element; typedef CKeyPairCompareLess<unsigned long,CSkeletalMorphs::CEntry> Compare;
template void __fastcall std::__adjust_heap<Element*,int,Element,Compare>(Element*,int,int,Element,Compare);
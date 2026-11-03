#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
// CVectorMap<K,V,CompareType> (Codex pass 11 consolidation, one model): Ego_r TPI CVectorMap derives CArray<pair<K,V> >
// (a public STLport vector); retail LowerBound reads start +0 / finish +4, Compare is at +12 and Dirty at +13 (sizeof 0x10,
// the "CVectorMap 0x10" container size of the generated headers). LowerBound is out of line in retail (every caller calls it).
#include "engine/CArray.h"
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    iterator LowerBound(const K&);
    const_iterator LowerBound(const K&) const;
#ifdef FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
    // Retail opt-in (2026-11-03, chunk 720, the size-optimised CTCInventoryWeapons TU): retail expands Find inside the
    // forced-inline CThing::GetTC there (`call LowerBound; mov esi,[esi+4]; cmp eax,esi; je; cmp [eax],ebx; jle`); under
    // optimize("s") a plain inline Find is called out of line instead (0x005C552C, 0x005C36C2).
    __forceinline
#endif
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
#ifdef FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
    __forceinline
#endif
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};
// LowerBound: out of line in retail (every caller calls it). A same-TU noinline body (EXTRA_RULES chunk 422): VC7.1 then knows
// the callee does not capture the caller's key temporary, so Find compares the folded constant key (00626E40 `cmp [eax],8`).
template<class K,class V,class CompareType>
__declspec(noinline) typename CVectorMap<K,V,CompareType>::iterator CVectorMap<K,V,CompareType>::LowerBound(const K& key)
{
    iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
template<class K,class V,class CompareType>
__declspec(noinline) typename CVectorMap<K,V,CompareType>::const_iterator CVectorMap<K,V,CompareType>::LowerBound(const K& key) const
{
    const_iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        const_iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
#endif

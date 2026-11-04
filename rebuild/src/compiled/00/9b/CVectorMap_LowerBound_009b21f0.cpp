// CVectorMap::LowerBound(const unsigned long& key) const -- Find the first entry whose key is at least key.
// Leaf: ?LowerBound@?$CVectorMap@KJV?$CKeyPairCompareLess@KJ@@@@QBEPBU?$pair@KJ@std@@ABK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: CCRCSymbolMap::SLONGMap and const FindSymbol00a38420; PDB unsigned-long key/long mapped type. Older CNameCRCToIndexMap wrapper is descriptive.
#ifndef _MSC_VER
#define __cdecl
#endif
#include "engine/CArray.h"
template<class K,class V> class CKeyPairCompareLess {
public:bool operator()(const std::pair<K,V>& entry,const K& key)const{return entry.first<key;}
};
// Layout: retail CVectorMap has12-byte Data at+0, empty Compare+12 and Dirty+13; key/value element is8bytes. Own callers and loop stride confirm these offsets.
template<class K,class V,class C> class CVectorMap {
public:
    typedef std::pair<K,V> value_type;typedef const value_type* const_iterator;
    CArray<value_type> Data;C Compare;bool Dirty;
    const_iterator LowerBound(const K& key)const;
};
template<class K,class V,class C>
typename CVectorMap<K,V,C>::const_iterator CVectorMap<K,V,C>::LowerBound(const K& key)const {
    const_iterator first=Data.begin();
    int count=Data.end()-Data.begin();
    while(count>0) {
        int half=count/2;
        const_iterator middle=first+half;
        if(Compare(*middle,key)){first=middle+1;count-=half+1;}
        else count=half;
    }
    return first;
}
typedef CVectorMap<unsigned long,long,CKeyPairCompareLess<unsigned long,long> > Map;
template class CVectorMap<unsigned long,long,CKeyPairCompareLess<unsigned long,long> >;
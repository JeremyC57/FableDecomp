// CVectorMap::LowerBound(const unsigned long& key) const -- Find the first entry whose key is at least key.
// Leaf: ?LowerBound@?$CVectorMap@KKV?$CKeyPairCompareLess@KK@@@@QBEPBU?$pair@KK@std@@ABK@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own caller009cc410 is CBankFile::FindIndexBySymbolCRC(unsigned long) const, not CDataBank::LookupValue. Actual CBankFile constructor009cd480 and PostInitEntries009cfb70 place SymbolCRCIndices at+0xd0; PDB maps unsigned-long to unsigned-long. Both PDB sets confirm const FindIndexBySymbolCRC.
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
typedef CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> > Map;
template class CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> >;
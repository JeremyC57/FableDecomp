#include <stdio.h>
// CVectorMap::LowerBound(const unsigned long& key) const -- Find the first entry whose key is at least key.
// Leaf: ?LowerBound@?$CVectorMap@KKV?$CKeyPairCompareLess@KK@@@@QBEPBU?$pair@KK@std@@ABK@Z
// Identity: Own caller009cc410 is CBankFile::FindIndexBySymbolCRC(unsigned long) const, not CDataBank::LookupValue. Actual CBankFile constructor009cd480 and PostInitEntries009cfb70 place SymbolCRCIndices at+0xd0; PDB maps unsigned-long to unsigned-long. Both PDB sets confirm const FindIndexBySymbolCRC.
#ifndef _MSC_VER
#define __cdecl
#endif
namespace std {
template<class A,class B> struct pair {A first;B second;};
template<class T> class allocator {};
template<class V,class T,class A> class _STLP_alloc_proxy : public A {public:V _M_data;};
template<class T,class A> class vector {
public:
    typedef T* iterator;typedef const T* const_iterator;
    T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
    const_iterator begin()const{return _M_start;}
    const_iterator end()const{return _M_finish;}
};
}
template<class T> class CArray : public std::vector<T,std::allocator<T> > {};
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

int main() {
    Map map;Map::value_type entries[6];unsigned long keys[6]={0,4,4,9,0x80000000UL,0xffffffffUL};int i;
    for(i=0;i<6;++i)entries[i].first=keys[i];
    map.Data._M_start=entries;map.Data._M_finish=entries+6;map.Data._M_end_of_storage._M_data=entries+6;
    const Map& view=map;unsigned long key=4;int ok=view.LowerBound(key)==entries+1;
    key=0;ok=ok && view.LowerBound(key)==entries;
    key=5;ok=ok && view.LowerBound(key)==entries+3;
    key=0x80000000UL;ok=ok && view.LowerBound(key)==entries+4;
    key=0xfffffffeUL;ok=ok && view.LowerBound(key)==entries+5;
    key=0xffffffffUL;ok=ok && view.LowerBound(key)==entries+5;
    map.Data._M_finish=entries+4;ok=ok && view.LowerBound(key)==entries+4;
    map.Data._M_finish=entries;ok=ok && view.LowerBound(key)==entries;
    for(i=0;i<6;++i)ok=ok && entries[i].first==keys[i];
    puts(ok ? "009d20d0_TEST PASS" : "009d20d0_TEST FAIL");return ok ? 0 : 1;
}
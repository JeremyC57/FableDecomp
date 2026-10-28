// CVectorMap::LowerBound(const unsigned long& key) const -- Find the first entry whose key is at least key.
// Leaf: ?LowerBound@?$CVectorMap@KVCRegisteredModeInfo@CTCCreatureModeManager@@V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@@QBEPBU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@std@@ABK@Z
// Identity: Own Count0073da50 and AllocateMode0073a8e0; PDB CRegisteredModeInfo::AllocFunc, const registry lookup.
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
class CThing;class CModeBase;
// Layout: CTCCreatureModeManager supplies only a nested name scope. CRegisteredModeInfo is the original4-byte AllocFunc pointer record; no outer object is accessed.
class CTCCreatureModeManager {public:class CRegisteredModeInfo {public:CModeBase* (__cdecl* AllocFunc)(CThing*);};};
typedef CVectorMap<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo,CKeyPairCompareLess<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo> > Map;
template class CVectorMap<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo,CKeyPairCompareLess<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo> >;
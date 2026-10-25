// CSoundManagerLH::FindByName(const CCharString& name) -- look the name up in the name map at +0x64; the mapped object, or null when absent
// Leaf: ?FindByName@CSoundManagerLH@@
// Retail: Fable.exe @ 0x00A3ED30, 82 bytes (+ int3 padding), thiscall `ret 4`. Descriptive name (the audio layer is not
//   in the PDBs).
// Callers: CSoundManagerLH::vfunc_38 0x00A3ED95 (landed, calls it with the bank key), 0x00A3F152, 0x00A3F278, 0x00AAC1E0.
// Callees: CVectorMap<CCharString, ...>::LowerBound 0x00A404D0, CBasicString<char>::operator< 0x00429950 (behind the
//   inline CCharString::operator<).
// Label: catalog "VectorMap_LookupByKey" is descriptive.
// Shape: the inline CVectorMap::Find (LowerBound, then end() read ONCE into a local `e` after the call, returned when the bound is end() or key < bound->first, with
//   CCharString's inline `<`: same block, null left, null right, then the data compare); the hit test re-reads end();
//   `return 0` first, the mapped pointer (+4) last.

template <class _CharT> class CBasicString {
public:
    _CharT* PData;
    bool operator<(const CBasicString<_CharT>& other) const;    // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PStringData;                        // the shared CCharStringData (CBasicString first)
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _T1, class _T2>
bool __cdecl operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y);   // 0x00A40250 (pair<CCharString, CCharString>)
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    _Tp* end() const { return _M_finish; }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {   // retail 16 bytes
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                          // +0x00
    C                        Comp;                          // +0x0C (empty comparator)
    bool                     Dirty;                         // +0x0D
    const value_type* LowerBound(const K& key) const;       // out of line (0x00A404D0 / 0x00A40AF0)
    const value_type* End() const { return Data.end(); }
    const value_type* Find(const K& key) const
    {
        const value_type* it = LowerBound(key);
        const value_type* e = Data.end();
        if (it == e || Comp(key, *it))
            return e;
        return it;
    }
};

class CSoundBankLH;                                         // descriptive: the named sound objects of the two maps

// The audio layer is absent from the PDBs: the two CVectorMaps are placed by these rows and by CommitChanges
// 0x00A3F340 (slot 41): a (name, name) -> object map at +0x54 (12-byte entries) and a name -> object map at +0x64
// (8-byte entries), each a retail 16-byte CVectorMap.
class CSoundManagerLH {                                     // vtable 0x0129D2C4
public:
    virtual void v00();
    unsigned char _04[0x50];                                // +0x04..+0x53
    CVectorMap<std::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;   // +0x54
    CVectorMap<CCharString, CSoundBankLH*>                          NameMap;       // +0x64
    CSoundBankLH* FindByName(const CCharString& name);
    CSoundBankLH* FindByNamePair(const CCharString& first, const CCharString& second);
};

CSoundBankLH* CSoundManagerLH::FindByName(const CCharString& name)
{
    const CVectorMap<CCharString, CSoundBankLH*>::value_type* it = NameMap.Find(name);
    if (it == NameMap.End())
        return 0;
    return it->second;
}
// CSoundManagerLH::FindByNamePair(const CCharString& first, const CCharString& second) -- look the (first, second) name pair up in the pair map at +0x54; the mapped object, or null when absent
// Leaf: ?FindByNamePair@CSoundManagerLH@@
// Retail: Fable.exe @ 0x00A3EE50, 126 bytes (+ int3 padding), thiscall `ret 8`. Descriptive name.
// Callers: CSoundManagerLH::vfunc_40 0x00A3EEDA (landed) and 0x00A3F261.
// Callees: CCharString copy ctor 0x0099EC30 (x2, the key pair), CVectorMap<pair<CCharString, CCharString>, ...>::LowerBound
//   0x00A40AF0, STLport operator<(const pair&, const pair&) 0x00A40250 (__cdecl, `add esp,8`), ~CCharString 0x0099EAE0.
// Label: catalog "Map_Insert_Sorted_CCharStringPair" is descriptive and wrong (nothing is inserted).
// Shape: the key pair is a temporary of the full expression (built in the 8-byte frame, destroyed second-then-first
//   before the end() test); the inline CVectorMap::Find (end() read once into a local after LowerBound) over CKeyPairCompareLess, whose `key < entry.first` is the
//   out-of-line pair operator<; the mapped pointer sits at +8 of the 12-byte entry.

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

namespace _STL {
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
    bool operator()(const K& key, const _STL::pair<K, V>& entry) const { return key < entry.first; }
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {   // retail 16 bytes
public:
    typedef _STL::pair<K, V> value_type;
    _STL::vector<value_type> Data;                          // +0x00
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
    CVectorMap<_STL::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;   // +0x54
    CVectorMap<CCharString, CSoundBankLH*>                          NameMap;       // +0x64
    CSoundBankLH* FindByName(const CCharString& name);
    CSoundBankLH* FindByNamePair(const CCharString& first, const CCharString& second);
};

CSoundBankLH* CSoundManagerLH::FindByNamePair(const CCharString& first, const CCharString& second)
{
    const CVectorMap<_STL::pair<CCharString, CCharString>, CSoundBankLH*>::value_type* it =
        NamePairMap.Find(_STL::pair<CCharString, CCharString>(first, second));
    if (it == NamePairMap.End())
        return 0;
    return it->second;
}
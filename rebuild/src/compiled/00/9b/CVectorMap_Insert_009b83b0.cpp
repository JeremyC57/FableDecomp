// CVectorMap::Insert(const CCharString& key, const unsigned long& value) -- CVectorMap<CCharString,unsigned long>::Insert: append the (key, value) pair to the backing vector and mark the map dirty (unsorted)
// Flags: /Gr
// Leaf: ?Insert@?$CVectorMap@VCCharString@@KV?$CKeyPairCompareLess@VCCharString@@K@@@@QAEXABVCCharString@@ABK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B83B0, 135 bytes (+ int3 padding), thiscall `ret 8`.
// Identity: FableWin.pdb ?Insert@?$CVectorMap@VCCharString@@KV?$CKeyPairCompareLess@VCCharString@@K@@@@QAEXABVCCharString@@ABK@Z;
//   Ego_r.pdb CVectorMap<CCharString,unsigned long>: the pair vector base (retail 12 bytes), Compare +0xC, Dirty +0xD.
//   The landed CDefinitionManager::CheckDependencies 0x009B05F0 inlines the same body.  Catalogued as
//   `StdVector_PushBack_Pair_CCharString_Entry`.
// Callees: CCharString(const CCharString&) @ 0x0099EC30 (x2: the pair temporary and its placement copy),
//   vector::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x009B71B0 (the tag in the key's dead argument slot),
//   ~CCharString @ 0x0099EAE0 (the temporary).
// Shape: `push_back(pair<K, V>(key, value)); Dirty = true;` with STLport push_back __forceinline (plain inline stays a call);
//   the Dirty store lands after the temporary's destructor on both paths.
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

namespace std {
struct __false_type { __false_type() {} };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef __false_type _IsPODType;
    __forceinline void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);   // 0x009B71B0
protected:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    void Insert(const K& key, const V& value);
    C    Compare;                                       // +0x0C
    bool Dirty;                                         // +0x0D
};

template <class K, class V, class C> void CVectorMap<K, V, C>::Insert(const K& key, const V& value)
{
    this->push_back(std::pair<K, V>(key, value));
    Dirty = true;
}

template void CVectorMap<CCharString, unsigned long>::Insert(const CCharString&, const unsigned long&);
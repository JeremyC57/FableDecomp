// CVectorMap::Insert(const unsigned long& key, const CDefClassInfo& value) -- CVectorMap<unsigned long,CDefinitionManager::CDefClassInfo>::Insert: append the (index, class info) pair to the backing vector and mark the map dirty (unsorted)
// Flags: /Gr
// Leaf: ?Insert@?$CVectorMap@KVCDefClassInfo@CDefinitionManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B9290, 101 bytes (+ int3 padding), thiscall `ret 8`.
// Identity: the CVectorMap<CCharString,unsigned long>::Insert shape (row 0x009B83B0, FableWin.pdb
//   ?Insert@?$CVectorMap@...@@QAEXABV..@ABK@Z) over the pair<unsigned long, CDefinitionManager::CDefClassInfo> vector of
//   0x009B7320: Ego_r.pdb CDefinitionManager::DefClassInfo (+132) is this CVectorMap, and FableWin.pdb has
//   ?Insert@?$CVectorMap@KVCDefClassInfo@CDefinitionManager@@...@@QAEXABKABVCDefClassInfo@CDefinitionManager@@@Z; Dirty at
//   +0xD.  No direct callers in the xref scan.  Catalogued as `StdVector_Insert_Complex_Pair`.
// Callees: CCharString(const CCharString&) @ 0x0099EC30, vector<unsigned long>(const vector&) @ 0x00535470,
//   vector<pair<...>>::push_back @ 0x009B9170 (out of line for this 24-byte element), free @ 0x00BFEA14 /
//   ~CCharString @ 0x0099EAE0 (the temporary).
// Shape: `push_back(pair<K, V>(key, value)); Dirty = true;` -- the temporary built in the frame, destroyed before the store.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                           // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
private:
    void* PStringData;
};

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = (_Tp*)__copy_trivial(__x._M_start, __x._M_finish, this->_M_start);
    }
    ~vector() {}
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // vector<unsigned long>: 0x0045BC09
    void push_back(const _Tp& __x);                                    // vector<pair<unsigned long, CDefClassInfo>>: 0x009B9170
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
}

template <class T> class CArray : public std::vector<T> {      // retail CArray: the 12-byte STLport vector
};

class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                               // Ego_r.pdb (24 bytes; retail 20: CArray 16 -> 12)
    public:
        CCharString            Name;                    // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();          // +0x04
        CArray<unsigned long>  GlobalIndices;           // +0x08
    };
};

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
typedef std::pair<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoPair;

template <class K, class V, class C> void CVectorMap<K, V, C>::Insert(const K& key, const V& value)
{
    this->push_back(std::pair<K, V>(key, value));
    Dirty = true;
}

template void CVectorMap<unsigned long, CDefinitionManager::CDefClassInfo>::Insert(const unsigned long&,
                                                                                   const CDefinitionManager::CDefClassInfo&);
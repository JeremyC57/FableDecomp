// CVectorMap::Insert(const unsigned __int64& key, const bool& value) -- append a (unique id, open flag) entry to the switchable node's unsorted id map and mark it dirty
// Grade: DIFFER(111v109) -- register allocation: retail keeps the temporary's key in ecx/edx and the bool in bl (push ebx) and copies from them, then `mov eax,[esi+4] ; add eax,10h ; mov [esi+4],eax`; the lane keeps only ecx, re-reads the high key word and the bool from the frame and uses `add dword ptr [esi+4],10h`. Tried: plain vector, make_pair, _Vector_base + _STLP_alloc_proxy, implicit pair copy (16-byte copy)
// Leaf: ?Insert@?$CVectorMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83770, 109 bytes (+ int3 padding), thiscall `ret 8`.
// Identity: Ego_r.pdb `?Insert@?$CVectorMap@_K_NV?$CKeyPairCompareLess@_K_N@@@@QAEXAB_KAB_N@Z` (CVectorMap<unsigned __int64,
//   bool, CKeyPairCompareLess<unsigned __int64, bool> >::Insert); an unreferenced COMDAT copy -- the landed
//   CNavSwitchableLeafNode constructors inline it (UniqueIDsOpen.Add). The catalog label "Vector_PushBack_9ByteElement"
//   is descriptive.
// Callee: vector<pair<unsigned __int64, bool> >::_M_insert_overflow(pos, x, tag, fill_len, atend) @ 0x00A82380.
// Layout: CVectorMap = STLport vector<pair<K, V> > (12 bytes) + empty Compare +0x0C + Dirty +0x0D; the pair is 16 bytes
//   (8-aligned key, bool at +8).
// Shape: `push_back(pair<K, V>(key, value)); Dirty = true;` -- the pair temporary in a 16-byte frame (key in ecx/edx, the
//   bool in bl, all three stored), STLport push_back's placement copy forwarded from the registers behind the null test
//   and `++_M_finish`, or _M_insert_overflow with the empty tag homed in the dead `key` slot; Dirty set on both exits.
inline void* operator new(unsigned int, void* where) { return where; }

namespace _STL {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A82380
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public _STL::vector<_STL::pair<K, V> > {
public:
    C    Compare;                                       // +0x0c
    bool Dirty;                                         // +0x0d
    void Insert(const K& key, const V& value)
    {
        push_back(_STL::pair<K, V>(key, value));
        Dirty = true;
    }
};

template void CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<unsigned __int64, bool> >::Insert(const unsigned __int64& key, const bool& value);

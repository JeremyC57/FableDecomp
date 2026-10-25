// CVectorMap::Insert(const unsigned __int64& key, CANavQuadTreeNode* const& value) -- append a (unique id, quad-tree node) entry to the navigation tree's unsorted id map and mark it dirty
// Leaf: ?Insert@?$CVectorMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A837E0, 114 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb `?Insert@?$CVectorMap@_KPAVCANavQuadTreeNode@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@@QAEXAB_K
//   ABQAVCANavQuadTreeNode@@@Z`; an unreferenced COMDAT copy (CNavQuadTree::LoadFromFile 0x00A7AF60 inlines it and calls
//   the same overflow at 0x00A7B61D). Twin of the bool map's Insert 0x00A83770. The catalog label
//   "Vector_PushBack_12ByteElement" is descriptive.
// Callee: vector<pair<unsigned __int64, CANavQuadTreeNode*> >::_M_insert_overflow(pos, x, tag, fill_len, atend) @ 0x00A82480.
// Layout: CVectorMap = STLport vector<pair<K, V> > (12 bytes) + empty Compare +0x0C + Dirty +0x0D; the pair is 16 bytes
//   (8-aligned key, node pointer at +8).
// Shape: `push_back(pair<K, V>(key, value)); Dirty = true;` -- the pair temporary in a 16-byte frame (key in ecx/edx, the
//   pointer in edi, all three stored), STLport push_back's placement copy forwarded from the registers behind the null
//   test and `++_M_finish`, or _M_insert_overflow with the empty tag homed in the dead `key` slot; Dirty set on both exits.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // stand-in body only
extern "C" void __cdecl free(void* block);              // stand-in body only

namespace std {
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A82480 (same-TU body below)
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU stand-in for the growth path 0x00A82480 (retail compiled it in this TU): with only a declaration VC7.1 treats the
// pair temporary as address-exposed and re-reads its words from the frame; with a body that never lets `x` escape it keeps
// the key and the pointer in registers (push ebx/edi) as retail.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = this->_M_finish - this->_M_start;
    unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = this->_M_start; __p != __position; ++__p, ++__new_finish)
        new (__new_finish) _Tp(*__p);
    for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish)
        new (__new_finish) _Tp(__x);
    if (!__atend)
        for (_Tp* __q = __position; __q != this->_M_finish; ++__q, ++__new_finish)
            new (__new_finish) _Tp(*__q);
    if (this->_M_start)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                       // +0x0c
    bool Dirty;                                         // +0x0d
    void Insert(const K& key, const V& value)
    {
        push_back(std::pair<K, V>(key, value));
        Dirty = true;
    }
};

class CANavQuadTreeNode;

template void CVectorMap<unsigned __int64, CANavQuadTreeNode*, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> >::Insert(
    const unsigned __int64& key, CANavQuadTreeNode* const& value);
// std::__insertion_sort(pair<unsigned long, CDefinitionManager::CDefClassInfo>* first, pair<unsigned long, CDefinitionManager::CDefClassInfo>* last, CKeyPairCompareLess comp) -- insertion sort of the definition manager's class-info map: linear-insert a copy of every entry after the first into the sorted prefix
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8F70, 191 bytes (+ int3 padding), `ret 4` (/Gr algorithm TU: first in
//   ecx, last in edx, the empty comparator by value on the stack).
// Identity: STLport 4.x `__insertion_sort(first, last, comp)` of the pair<unsigned long, CDefinitionManager::CDefClassInfo>
//   CVectorMap sort (FableWin.pdb names _Adjust_heap / _Push_heap of the same instantiation, 0x009B8220 / 0x009B77B0);
//   its only callee is that sort's __linear_insert 0x009B8C00 (landed; "single caller 0x009B900E" = this body). The
//   catalog label "make_heap_pair_fixup" is descriptive.
// Callees: CCharString(const CCharString&) @ 0x0099EC30, malloc @ 0x00BFEA0E and memmove @ 0x00BFEAE6 (the
//   CArray<unsigned long> copy, inline), __linear_insert(first, i, val, comp) @ 0x009B8C00 (the callee destroys val).
// Shape: `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, _Tp(*i), comp);`
//   -- the 24-byte value is copy-constructed straight into its argument slot: the pair's user copy constructor, the
//   implicit CDefClassInfo copy (Name through its out-of-line copy constructor, the alloc function pointer, then the
//   STLport vector copy: `_Vector_base(n, a)` with the three zero stores, malloc only for n != 0, __copy_trivial's
//   memmove); first and last are spilled to the two frame words.
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
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
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
typedef std::pair<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoPair;

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B8C00

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
template void __fastcall __insertion_sort<CDefClassInfoPair*, CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> >(
    CDefClassInfoPair*, CDefClassInfoPair*, CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>);
}
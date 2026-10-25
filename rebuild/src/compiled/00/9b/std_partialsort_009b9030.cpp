// std::__partial_sort(pair* first, pair* middle, pair* last, pair*, CKeyPairCompareLess comp) -- (pair<unsigned long, CDefinitionManager::CDefClassInfo>) STLport partial_sort core: heapify [first, middle), pop every later element smaller than the root into the heap, then sort_heap the head
// Flags: /Gr
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9030, 244 bytes, `ret 0Ch` (/Gr TU: first in ecx, middle in edx; last, the
//   unused value-type pointer and the empty comparator on the stack). Callers 0x009B925C and 0x009B9479 (partial_sort /
//   the introsort loop's heap-sort fallback of the definition manager's class-info vector map). The catalog label
//   "PartialSort_Heap_Pair24byte" is descriptive.
// Callees: __make_heap(first, last, comp, T*, int*) @ 0x009B8820 (landed), __pop_heap(first, last, result, val, comp,
//   int*) @ 0x009B8770 (landed; the 24-byte value is built in its argument area), sort_heap(first, last, comp)
//   @ 0x009B8CA0 (landed), CCharString(const CCharString&) @ 0x0099EC30, malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6 (the
//   value's inlined CDefClassInfo / vector<unsigned long> copy).
// Layout: the 24-byte pair<unsigned long, CDefClassInfo> of the landed siblings (Ego_r.pdb CDefClassInfo names).
// Shape: STLport 4.6 _algo.c `__partial_sort` verbatim; make_heap is the inline wrapper over the out-of-line
//   __make_heap (`push 0 ; push 0 ; push comp`). The loop is `for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, (int*)0)`; VC7.1 turns it into a counted loop and spills `first` into
//   the dead value-type argument slot.
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                           // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x009B8820

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                _Tp __val, _Compare __comp, _Distance*);                                                           // 0x009B8770

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);                       // 0x009B8CA0

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CDefClassInfoPair*)0, (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                    _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __partial_sort(CDefClassInfoPair*, CDefClassInfoPair*, CDefClassInfoPair*, CDefClassInfoPair*,
                             CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>);
}
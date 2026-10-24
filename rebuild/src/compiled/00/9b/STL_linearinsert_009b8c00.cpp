// _STL::__linear_insert(pair<unsigned long,CDefClassInfo>* first, pair<unsigned long,CDefClassInfo>* last, pair<unsigned long,CDefClassInfo> val, CKeyPairCompareLess<unsigned long,CDefClassInfo> comp) -- STLport insertion-sort step: if val sorts before the first element shift [first, last) up one slot and store val first, otherwise sink it from last
// Flags: /Gr
// Leaf: ??$__linear_insert@PAU?$pair@KVCDefClassInfo@CDefinitionManager@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8C00, 160 bytes, `ret 1Ch` (/Gr TU: first in ecx, last in edx; the
//   24-byte value and the empty comparator on the stack, the value destroyed by the callee).
// Identity: the pair<unsigned long, CDefinitionManager::CDefClassInfo> vector of 0x009B7320 (FableWin.pdb names
//   _Adjust_heap<pair<unsigned long,CDefinitionManager::CDefClassInfo>*,...,CKeyPairCompareLess<...>> for the same sort);
//   single caller 0x009B900E (its __insertion_sort).  Catalogued as `Vector_InsertOrReallocPair_DefClassInfo`.
// Callees: __copy_backward(first, last, last + 1, random_access_iterator_tag, int*) @ 0x009B7870 (landed; the tag in the
//   comparator's dead slot), CCharString::operator= @ 0x0099EFB0, vector<unsigned long>::operator= @ 0x0045BC09,
//   CCharString(const CCharString&) @ 0x0099EC30, vector<unsigned long>(const vector&) @ 0x00535470 (not inlined here),
//   __unguarded_linear_insert(last, val, comp) @ 0x009B76F0, free @ 0x00BFEA14 / ~CCharString @ 0x0099EAE0 (val).
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` with copy_backward's pointer dispatch inlined down to __copy_backward.
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

namespace _STL {
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

template <class T> class CArray : public _STL::vector<T> {      // retail CArray: the 12-byte STLport vector
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
typedef _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo> CDefClassInfoPair;

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*);   // 0x009B7870

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B76F0

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template void __linear_insert(CDefClassInfoPair*, CDefClassInfoPair*, CDefClassInfoPair,
                              CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>);
}
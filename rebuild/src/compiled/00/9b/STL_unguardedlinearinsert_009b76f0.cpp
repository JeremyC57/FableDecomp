// _STL::__unguarded_linear_insert(pair<unsigned long,CDefClassInfo>* last, pair<unsigned long,CDefClassInfo> val, CKeyPairCompareLess<unsigned long,CDefClassInfo> comp) -- STLport insertion-sort inner step: shift larger-keyed (index, class info) pairs up one slot and drop val into the gap
// Flags: /Gr
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@KVCDefClassInfo@CDefinitionManager@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B76F0, 139 bytes, `ret 1Ch` (/Gr TU: last in ecx; the 24-byte value
//   and the empty comparator on the stack, the value destroyed by the callee).
// Identity: the 24-byte pair<unsigned long, CDefClassInfo> of 0x009B7320 sorted by its unsigned key
//   (CKeyPairCompareLess<unsigned long,CDefClassInfo>: `cmp ; jae`); callers 0x009B814C / 0x009B8C7C (the insertion
//   sorts of that vector).  Catalogued as `Insertion_Sort_Shift_Complex`.
// Callees: CCharString::operator= @ 0x0099EFB0, vector<unsigned long>::operator= @ 0x0045BC09, free @ 0x00BFEA14 (the
//   value's vector storage), ~CCharString @ 0x0099EAE0.
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`.
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template void __unguarded_linear_insert(CDefClassInfoPair*, CDefClassInfoPair,
                                        CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo>);
}
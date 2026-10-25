// std::__uninitialized_copy(const pair<unsigned long,CDefClassInfo>* first, const pair<unsigned long,CDefClassInfo>* last, pair<unsigned long,CDefClassInfo>* result, const __false_type&) -- STLport copy-construct a range of (index, class info) pairs into raw storage; returns the end of the copies
// Flags: /Gr
// Leaf: ??$__uninitialized_copy@PBU?$pair@KVCDefClassInfo@CDefinitionManager@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B7320, 177 bytes (+ int3 padding), `ret 8` (/Gr TU: first in ecx, last
//   in edx; result and the __false_type tag reference on the stack).
// Identity: element = 24 bytes {dword key, CCharString +4, dword +8, a 12-byte trivially-copied vector of dwords +0xC} =
//   pair<unsigned long, CDefinitionManager::CDefClassInfo> (Ego_r.pdb CDefClassInfo: Name CCharString, AllocFunc, GlobalIndices
//   CArray<unsigned long>); the sibling rows 0x009B73E0 (uninitialized_fill_n) and 0x009B8C00 (vector insert) and the
//   landed sort 0x009B9490 work on the same vector of class-info pairs.  Callers 0x009B8E3E / 0x009B8EAF (the vector's
//   range insert 0x009B8DD0).  Catalogued as `Vector_CopyRange_Deque`.
// Callees: CCharString(const CCharString&) @ 0x0099EC30, malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- placement new keeps
//   its null test; the pair / CDefClassInfo / CArray copy constructors inline down to the vector copy constructor
//   (_Vector_base(n): zero the three words, allocate n, then __copy_trivial's memmove).
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

namespace std {
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CDefClassInfoPair* __uninitialized_copy(const CDefClassInfoPair*,
    const CDefClassInfoPair*, CDefClassInfoPair*, const __false_type&);
}
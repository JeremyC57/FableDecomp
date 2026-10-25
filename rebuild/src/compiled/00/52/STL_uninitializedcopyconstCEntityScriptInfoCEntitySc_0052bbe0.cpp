// std::__uninitialized_copy<const CEntityScriptInfo*, CEntityScriptInfo*>(first, last, result, const __false_type&) -- STLport uninitialized copy of NThingManager::CEntityRuntimePersistence::CEntityScriptInfo records: copy-construct each source element into raw storage from result on, return the end of the constructed range
// Leaf: ??$__uninitialized_copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052BBE0, 145 bytes, `ret 8` (/Gr `__fastcall`: first in ecx, last in edx; result and
//   the `__false_type` reference on the stack).
// Callees: CCharString copy ctor 0x0099EC30, malloc 0x00BFEA0E and memmove 0x00BFEAE6 (the inlined STLport
//   vector<unsigned char> copy constructor: `_Vector_base(n, alloc)` + the trivial `__copy_trivial` memmove).
// Callers: 0x0052C1AA, 0x0052C217, 0x0052C39A, 0x0052C3F4 (vector<CEntityScriptInfo>::operator= / insert paths).
// Label correction: catalogued as "CCharStringVector_CopyAssign"; it is STLport `__uninitialized_copy` for a non-POD element.
// Layout: CEntityScriptInfo (Ego_r.pdb, 24 bytes in retail): Script +0x00, SaveData +0x04 (CArray<unsigned char> = STLport
//   vector: start +4, finish +8, end of storage +0xc), UncompressedSize +0x10, Valid +0x14.
// Shape: STLport 4.x `_ForwardIter __cur = __result; for ( ; __first != __last; ++__first, ++__cur) _Construct(&*__cur,
//   *__first); return __cur;` (the try/unwind pair compiles away). `_Construct` is the null-tested placement new; the
//   implicit CEntityScriptInfo copy constructor inlines the vector copy: `_M_start = _M_finish = _M_end_of_storage = 0`
//   with `mov eax,0` (keeps the size compare's flags), the malloc of the byte count unless zero, start/finish/end set, then
//   the memmove of the source bytes. VC strength-reduces the destination into a second pointer at element + 8
//   (`[esi-4]`, `[esi]`, `[esi+4]`). An explicit instantiation emits only this function.
extern "C" void* __cdecl malloc(unsigned int size);                       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PData;                                                          // CBasicString<char>* (4 bytes)
    CCharString(const CCharString& other);                                // 0x0099EC30
    ~CCharString();                                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                     // 0x0099EFB0
};

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _Tp> class allocator {                                    // STLport allocator over the malloc allocator: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {                     // the end-of-storage word over the empty allocator
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                                        // +0x0
    _Tp* _M_finish;                                                       // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;               // +0x8
};

template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __true_type());
    }
    ~vector() { }
    vector& operator=(const vector& __x);                                 // 0x005295B0 (vector<unsigned char>)
};
}

template <class T> class CArray : public std::vector<T> {               // retail CArray<T>: an STLport vector (12 bytes)
};

namespace NThingManager {
struct CEntityScriptInfo {                                                // NThingManager::CEntityRuntimePersistence::CEntityScriptInfo, 24 bytes
    CCharString          Script;                                          // +0x00
    CArray<unsigned char> SaveData;                                       // +0x04 (PDB 16 bytes, retail 12)
    long                 UncompressedSize;                                // +0x10
    bool                 Valid;                                           // +0x14
};
}
using NThingManager::CEntityScriptInfo;

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
// STLport 4.x __uninitialized_copy for a non-trivial element; the algorithm TU is compiled /Gr, so it is __fastcall.
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                    const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CEntityScriptInfo* __fastcall __uninitialized_copy<const CEntityScriptInfo*, CEntityScriptInfo*>(
    const CEntityScriptInfo*, const CEntityScriptInfo*, CEntityScriptInfo*, const __false_type&);
}
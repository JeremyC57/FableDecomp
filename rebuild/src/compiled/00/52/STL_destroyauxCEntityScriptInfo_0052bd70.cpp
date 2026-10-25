// std::__destroy_aux<CEntityScriptInfo*>(first, last, const __false_type&) -- STLport range destroy of NThingManager::CEntityRuntimePersistence::CEntityScriptInfo records: run each element's destructor from first to last (free the SaveData buffer, destroy the Script string)
// Leaf: ??$__destroy_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052BD70, 51 bytes, `ret 4` (/Gr `__fastcall`: first in ecx, last in edx, the
//   `__false_type` reference on the stack).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate in ~_Vector_base), ~CCharString 0x0099EAE0.
// Callers: 17 sites in the CThingManager TU, among them the landed NThingManager::CEntityRuntimePersistence::Clear
//   0x005236F0 (as `Destroy_0052bd70`) and vector<CEntityScriptInfo>::operator= 0x0052C0DA.
// Label correction: catalogued as "_Copy_backward<CPixelFormat*,CPixelFormat*>" (propagated); it is STLport `__destroy_aux`
//   (the body `_Destroy(first, last)` reaches for a non-trivial element).
// Layout: CEntityScriptInfo (Ego_r.pdb, 24 bytes in retail): Script +0x00, SaveData +0x04 (CArray<unsigned char> = STLport
//   vector: start +4, finish +8, end of storage +0xc), UncompressedSize +0x10, Valid +0x14.
// Shape: STLport 4.x `for ( ; __first != __last; ++__first) _Destroy(&*__first);` with the implicit ~CEntityScriptInfo
//   inlined: ~vector<unsigned char> (trivial element destroy, then ~_Vector_base's `if (_M_start != 0) deallocate`) and the
//   out-of-line ~CCharString. An explicit instantiation emits only this function.
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
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
// STLport 4.x __destroy_aux; the algorithm TU is compiled /Gr, so the template is __fastcall.
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template void __fastcall __destroy_aux<CEntityScriptInfo*>(CEntityScriptInfo*, CEntityScriptInfo*, const __false_type&);
}
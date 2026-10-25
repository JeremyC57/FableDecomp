// std::__copy<const CEntityScriptInfo*, CEntityScriptInfo*, int>(first, last, result, const random_access_iterator_tag&, int*) -- STLport random-access copy of NThingManager::CEntityRuntimePersistence::CEntityScriptInfo records: assign (last - first) elements forward, return the end of the destination
// Leaf: ??$__copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052B5E0, 91 bytes, `ret 0Ch` (/Gr `__fastcall`: first in ecx, last in edx; result,
//   the tag reference and the null `_Distance*` on the stack).
// Callees: CCharString::operator= 0x0099EFB0, vector<unsigned char>::operator= 0x005295B0 (CArray<unsigned char>'s
//   implicit assignment forwards to it; manifest "std::vector_assign_buffer").
// Callers: 0x0052BB2C (the `copy()` front end 0x0052BB20), 0x0052C0CB / 0x0052C101 (vector<CEntityScriptInfo>::operator=).
// Label correction: catalogued as "_Move<CRegister*,CRegister*>" (propagated); it is STLport `__copy`, the const-source instantiation (vector::operator= copies `x` over the live prefix through it).
//   0x0052B5E0 and 0x0052BD10 are byte-identical bodies (two instantiations, no ICF in retail).
// Layout: CEntityScriptInfo (Ego_r.pdb, 24 bytes in retail): Script +0x00 (CCharString), SaveData +0x04 (CArray<unsigned char>
//   = an STLport vector, PDB 16 / retail 12 bytes), UncompressedSize +0x10, Valid +0x14.
// Shape: STLport 4.x `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
//   return __result;` -- the 24-byte stride is the 0x2AAAAAAB / `sar edx,2` divide, the count runs down in ebx; the implicit
//   CEntityScriptInfo::operator= calls the two member assignments and copies the long and the bool. An explicit instantiation
//   emits only this function.
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
// STLport 4.x random-access __copy; the algorithm TU is compiled /Gr, so the template is __fastcall.
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                     _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CEntityScriptInfo* __fastcall __copy<const CEntityScriptInfo*, CEntityScriptInfo*, int>(const CEntityScriptInfo*, const CEntityScriptInfo*, CEntityScriptInfo*,
                                                                    const random_access_iterator_tag&, int*);
}
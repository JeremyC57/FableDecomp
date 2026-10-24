// _STL::vector::reserve(unsigned int n) -- grow a vector<CBlendedEnvironmentTheme> to capacity n: allocate, copy-construct, destroy and free the old block
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- same-TU auto_inline(off) noinline bodies for __uninitialized_copy and __destroy_aux (tags unnamed) with the element's out-of-line destructor declared (an empty destroy loop is dropped) home both tags on n's argument slot
// Leaf: ?reserve@?$vector@VCBlendedEnvironmentTheme@NEnvironmentInternal@@V?$allocator@VCBlendedEnvironmentTheme@NEnvironmentInternal@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889CB0, length 240 bytes (+ int3 padding), `ret 4`.
// Identity: callers 0x00888562, 0x008887C2 and 0x00888846 in the CEnvironmentThemeSetDay code of the environment TU
//   reserve local vectors of 0x1F4-byte NEnvironmentInternal::CBlendedEnvironmentTheme (the element of the landed
//   __destroy_aux 0x006B67A0 and of the uninitialized copy 0x00889700). The catalog label "Vector_Reserve" is
//   descriptive.
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the STLport malloc allocator), the out-of-line
//   __uninitialized_copy @ 0x00889700 and __destroy_aux @ 0x006B67A0 (both /Gr `__fastcall`, the empty tag by reference).
// Shape: STLport 4.x `vector::reserve`: capacity() and size() as signed pointer differences divided by 500 (`imul
//   0x10624dd3 ; sar 5`), _M_allocate_and_copy + _M_clear inlined when a block exists (the new block parked in the
//   `push ecx` frame word across the copy, the empty tags placed on n's argument slot so n is re-read afterwards),
//   a bare allocate otherwise, then _M_set.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {                        // 500 bytes (retail 12-byte CArrays)
public:
    unsigned char _body[0x1F4];
    ~CBlendedEnvironmentTheme();                        // non-trivial (its CArray members); out of line
};
}

namespace _STL {
struct __false_type { __false_type() { } };

typedef NEnvironmentInternal::CBlendedEnvironmentTheme CTheme;
// Same-TU stand-ins for the out-of-line /Gr callees (GOTCHAS empty-tag rule): real bodies, never inlined, the tag
// parameter unnamed -- VC7.1 then homes both __false_type temporaries on n's dead argument slot as retail.
#pragma auto_inline(off)
__declspec(noinline) CTheme* __fastcall __uninitialized_copy(CTheme* first, CTheme* last, CTheme* result, const __false_type&)   // 0x00889700
{
    for (; first != last; ++first, ++result)
        new (result) CTheme(*first);
    return result;
}
__declspec(noinline) void __fastcall __destroy_aux(CTheme* first, CTheme* last, const __false_type&)                            // 0x006B67A0
{
    for (; first != last; ++first)
        first->~CTheme();
}
#pragma auto_inline(on)

template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp>
class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                                      // +0x00
    _Tp* _M_finish;                                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;             // +0x08

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage._M_data - _M_start); }

    _Tp* _M_allocate_and_copy(unsigned int __n, _Tp* __first, _Tp* __last)
    {
        _Tp* __result = _M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void reserve(unsigned int __n);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(unsigned int __n)
{
    if (capacity() < __n) {
        const unsigned int __old_size = size();
        _Tp* __tmp;
        if (_M_start) {
            __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
            _M_clear();
        } else {
            __tmp = _M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
template class vector<CTheme>;
}
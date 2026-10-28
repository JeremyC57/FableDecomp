// std::vector::_M_allocate_and_copy(size_type n, const _D3DVERTEXELEMENT9* first, const _D3DVERTEXELEMENT9* last) -- vector<_D3DVERTEXELEMENT9> (CArray of the vertex-shader declaration): malloc n elements and copy-construct [first, last) into them, returning the new block
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5DFF0, 78 bytes, `ret 0Ch` (thiscall, `this` unused).
// Caller: vector<_D3DVERTEXELEMENT9>::operator= @ 0x00A5E2E0 (via CArray<_D3DVERTEXELEMENT9>::operator= 0x00A5E5A0, which
//   CShaderResource::SetVertexShaderInput @ 0x00A5D8B0 calls). Callees: malloc @ 0x00BFEA0E.
// Label: the manifest's `AllocAndCopyRange_u64` is descriptive; STLport 4.6 `vector::_M_allocate_and_copy` (member
//   template over const_pointer). Byte-identical to 0x00A5DEE0, the same body for the TU's other 8-byte-element vector.
// Shape: allocator::allocate (`n != 0 ? malloc(n * 8) : 0`), then __uninitialized_copy with __false_type (STLport's
//   default for a user struct): a placement-new copy per element under its null test, `cur` kept as result - first.
struct _D3DVERTEXELEMENT9 {                     // 8 bytes
    unsigned short Stream;
    unsigned short Offset;
    unsigned char  Type;
    unsigned char  Method;
    unsigned char  Usage;
    unsigned char  UsageIndex;
};
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
inline void* operator new(size_t, void* where) { return where; }

namespace std {

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, size_t) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef size_t size_type;
    protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    public:
};

template _D3DVERTEXELEMENT9* vector<_D3DVERTEXELEMENT9>::_M_allocate_and_copy(size_t, const _D3DVERTEXELEMENT9*,
                                                                              const _D3DVERTEXELEMENT9*);

}   // namespace std
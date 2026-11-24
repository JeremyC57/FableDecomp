// vector<CPlane>::reserve(unsigned int n) -- grow the plane list's capacity to n, copying the existing planes into a fresh block
// Leaf: ?reserve@?$vector@VCPlane@@V?$allocator@VCPlane@@@std@@@std@@QAEXI@Z
// Name fix (NF382): was vector<C2DLineF>::reserve (its own body is 0x00A74940, caller 0x00A74EA0). FableWin
//   CPhysicsMeshObject::GetNavigationLevelVolumes (lib_physics_mesh_object_2.obj), this address's only caller 0x00A12EE0, calls
//   vector<CPlane>::reserve; the copy callee 0x00A11210 is __uninitialized_copy<CPlane*, CPlane*> (catalog, and bound so by
//   vector<CPlane>::_M_insert_overflow 0x00A13720). STLport 4.6 _M_allocate_and_copy is a member template, so reserve passes
//   the plain pointers and the callee is that same <CPlane*, CPlane*> instance. Same bytes.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A13650, 171 bytes, `ret 4`. Only caller 0x00A12F41 in
//   CPhysicsMeshObject::GetNavigationLevelVolumes (a zeroed local vector<CPlane> reserved to the plane count). The
//   catalog label "CArray_Reserve" is descriptive; the element is the 16-byte CPlane (Normal/Distance, Ego_r.pdb, engine/CPlane.h).
// Callees: malloc 0x00BFEA0E (allocator::allocate, both arms), the out-of-line STLport `__uninitialized_copy(first,
//   last, result, const __false_type&)` 0x00A11210 (__fastcall, `ret 8`, catalog __uninitialized_copy<CPlane*,CPlane*>), free
//   0x00BFEA14 (inline _M_clear: trivial destroy + deallocate).
// Shape: STLport 4.6 `reserve` verbatim with `_M_allocate_and_copy` inlined. The copy's body must be visible in the
//   TU (template under auto_inline(off) + noinline): only then does VC7.1 spill the cached finish to the `push ecx`
//   word and home the new block (and the empty tag) on the live `n` argument slot as retail does.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

// CPlane / C3DVector keep their implicit (trivial) copy: retail 0x00A11210 copies the four dwords with integer moves.
#define FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23
#include "engine/CPlane.h"

namespace std {
struct __false_type { __false_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void reserve(size_type __n);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
}

template void std::vector<CPlane>::reserve(unsigned int);
// std::vector::reserve(size_type n) -- STLport vector<C3DVector>::reserve: when the capacity is below n, reallocate n elements (copying and freeing the old block when there is one) and keep the size
// Leaf: ?reserve@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00920FF0, 147 bytes, __fastcall `ret 4`.
// Identity: STLport 4.6 vector<_Tp>::reserve (Ego_r.pdb ?reserve@?$vector@VC3DVector@@...@std@@QAEXI@Z family). The catalog
//   label "vector_reserve" is right in spirit.
// Callees: _M_allocate_and_copy 0x00920F90 (this TU's out-of-line member), free 0x00BFEA14 (_M_clear's deallocate),
//   malloc 0x00BFEA0E (the empty-vector allocate).
// Element: C3DVector (12 bytes, memberwise copy through ebx). The body is COMDAT-folded: every STLport vector of a
//   12-byte trivially-copied element shares it (callers CAIStateGroup_CreatureSeekEnemyUnderground::Update 0x00920CAE,
//   CPhysicsMesh GetMeshDataFromFileData 0x00A6CD0F, CArray<pair<..>>::DeallocateUnusedMemory 0x00A7261A,
//   COptimisedPrimitive 0x00AC8CCB, the particle CPSCAttractor/CPSCLight copies 0x00B180C8 / 0x00B1A826).
// Layout (STLport 4.6 vector, 12 bytes): _M_start +0, _M_finish +4, _M_end_of_storage (_STLP_alloc_proxy) +8.
// Shape: STLport verbatim -- capacity()/size() as pointer differences divided by 12 (the 0x2AAAAAAB imul), the
//   `if (_M_start)` split between _M_allocate_and_copy + _M_clear (a null-tested free) and a bare allocate
//   (`n != 0 ? malloc(n*12) : 0`), then _M_set(tmp, tmp + old_size, tmp + n).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);          // 0x00BFEA14

inline void* operator new(size_t, void* p) { return p; }

#pragma pack(push, 1)
class C3DVector {                               // retail: byte-packed, memberwise user copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

namespace std {

struct __false_type {                           // STLport empty tag; a user ctor keeps the temporary store-free
    __false_type() { }
};

template <class _Tp> class allocator {          // STLport allocator over the CRT malloc/free
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, size_t) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(__first);
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {   // STLport vector, 12 bytes
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void reserve(size_type __n);
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
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

template class std::vector<C3DVector>;
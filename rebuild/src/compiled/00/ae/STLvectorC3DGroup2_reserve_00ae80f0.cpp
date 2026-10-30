// std::vector<C3DGroup2>::reserve(unsigned int n) -- grow the skinning-group array (CArray<C3DGroup2>) to capacity n: allocate, copy the groups across and release the old block, or just allocate when the array never had storage
// Leaf: ?reserve@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE80F0, 163 bytes, __fastcall `ret 4`.
// Callers: CVectorPairOwner::ReserveBoth 0x00AE3330 (landed; the program's SimGroups / NonSimGroups) and the program loader
//   0x00AE3948.
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy<C3DGroup2*> 0x00AE7480 (landed; the non-const instance -- reserve copies from _M_start, a _Tp*; the const instance is 0x00AE7530), _M_clear 0x00AE7FA0.
// Layout: retail CArray<T> is an STLport vector {_M_start, _M_finish, _M_end_of_storage}; C3DGroup2 is {long BoneIndex,
//   CArray<C3DVertexBlend2> VertexBlends} = 16 bytes.
// Label: catalog "Container_Reserve" is descriptive.
// Shape: STLport 4.6 _vector.c verbatim (`if (capacity() < n) { old_size = size(); if (_M_start) { tmp =
//   _M_allocate_and_copy(n, _M_start, _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old_size,
//   tmp + n); }`); _M_allocate_and_copy inline. The tag-taking __uninitialized_copy is a same-TU noinline body that never
//   touches the tag, so the `__false_type()` temporary and the spilled tmp share the dead n argument slot.
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00AE7480
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear();                                                                           // 0x00AE7FA0
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    pointer _M_allocate_and_copy(size_type __n, _Tp* __first, _Tp* __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    public:
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

class C3DVertexBlend2 { public: long Vertex; float Weight; };   // 8 bytes

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: an STLport vector (12 bytes)
public:
    CArray(const CArray& other);                         // 0x00AE5D80 for CArray<C3DVertexBlend2>
};

class C3DGroup2 { public:                                       // 16 bytes in retail
    long                     BoneIndex;                  // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;               // +0x4
};

template void std::vector<C3DGroup2>::reserve(unsigned int);
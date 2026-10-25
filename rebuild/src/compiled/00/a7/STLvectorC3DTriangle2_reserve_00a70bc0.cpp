// std::vector<C3DTriangle2>::reserve(unsigned int n) -- grow the index-triangle vector's storage to hold at least n triangles, copying the existing ones and freeing the old block
// Leaf: ?reserve@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70BC0, 142 bytes, thiscall `ret 4`.
// Callees: vector<C3DTriangle2>::_M_allocate_and_copy<const C3DTriangle2*> 0x00A6E470 (same chunk, out of line),
//   free 0x00BFEA14 / malloc 0x00BFEA0E (MSVCR71 import thunks behind the STLport allocator).
// Identity: the manifest's "StdVector_Reserve_Tri" is descriptive. The 6-byte stride (the 0x2AAAAAAB divide) and the
//   callee are the C3DTriangle2 (unsigned short VertexIndex[3]) vector of the CPhysicsMesh TU.
// Shape: STLport 4.6 `if (capacity() < n) { const size_type old_size = size(); pointer tmp; if (_M_start) { tmp =
//   _M_allocate_and_copy(n, _M_start, _M_finish); _M_clear(); } else tmp = _M_end_of_storage.allocate(n);
//   _M_set(tmp, tmp + old_size, tmp + n); }` -- _M_clear's destroy loop over the trivially destructible triangles
//   vanishes, leaving the null-tested free; allocate is `n != 0 ? malloc(n * 6) : 0`.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class C3DTriangle2 {                                    // 6 bytes (Ego_r.pdb)
public:
    unsigned short VertexIndex[3];                      // +0x00
};

namespace std {

class __false_type { public: __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __value_type(__first));
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00A6E470
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
    void reserve(size_type __n);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, (const _Tp*)this->_M_start, (const _Tp*)this->_M_finish);
            _M_clear();
        }
        else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template void vector<C3DTriangle2>::reserve(unsigned int);

}
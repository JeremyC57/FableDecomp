// _STL::vector::reserve(unsigned int n) -- STLport reserve for a vector of 12-byte CNavigationPosition entries: when the capacity is below n, copy the entries into a fresh n-entry block and free the old one, or just allocate when the vector is empty; the size is kept
// Leaf: ?reserve@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80980, 147 bytes (+ int3 padding to 160), thiscall `ret 4`.
// Caller: CNavQuadTree::LoadFromFile (0x00A7B0FA: `ActionPoints.reserve(count)`, the vector<CNavigationPosition> at +0x50).
// Callees: vector<CNavigationPosition>::_M_allocate_and_copy<CNavigationPosition*> 0x00A7E9D0 (this chunk), free
//   0x00BFEA14 (the inlined _M_clear: trivial destroy + deallocate), malloc 0x00BFEA0E (allocator::allocate).
// Label: catalog "Vector_Reserve_SpecificSize_12Byte" is descriptive.
// Shape: STLport 4.x `reserve` verbatim (the landed 0x006E4F30 shape): capacity() and size() as pointer differences
//   divided by 12 (the 0x2AAAAAAB reciprocal), the unsigned `capacity() < n` test, `_M_allocate_and_copy` out of line,
//   `_M_clear()` inline, `_M_set(tmp, tmp + old_size, tmp + n)`.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class C2DVector {
public:
    float X;
    float Y;
};

class CNavigationPosition {                                 // 12 bytes (Ego_r.pdb)
public:
    C2DVector Position;                                     // +0x00
    long      Layer;                                        // +0x08
};

namespace _STL {
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
};

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00A7E9D0
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
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
}

template void _STL::vector<CNavigationPosition>::reserve(unsigned int);
// std::vector::reserve(size_type n) -- (vector<CVertexGroupBlockOptimiser::CBlockInfo>) STLport reserve of the 12-byte block-candidate records: when capacity() < n, allocate n slots, copy the records over and free the old block (or just allocate when empty), keep the size
// Leaf: ?reserve@?$vector@VCBlockInfo@CVertexGroupBlockOptimiser@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADB960, 209 bytes (+15 int3 padding), thiscall, `ret 4`.
// Identity: the manifest's "std::vector::reserve_12byte" is descriptive. The only caller 0x00ADA022 is
//   CVertexGroupBlockOptimiser::AddBlocks 0x00AD9F10 reserving a local vector; the element is Ego_r.pdb
//   CVertexGroupBlockOptimiser::CBlockInfo (BlockIndex, Cost, Benefit: 12 bytes, the record the landed __linear_insert
//   0x00ADBC50 sorts); __uninitialized_copy 0x00ADA7E0 is its placement copy (callers here, 0x00ADA91C and the fill
//   insert 0x00ADBD50).
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x00ADA7E0 (`__fastcall`,
//   the tag by reference), free 0x00BFEA14.
// Shape: STLport 4.6 vector.c reserve, as the CScriptThing sibling 0x008ACD30, but with the element's trivial
//   destructor `_M_clear()` folds to the null-tested free of the re-read start (inline). The copy gets the address of
//   the LIVE `n` argument slot as its tag: a same-TU noinline __uninitialized_copy stand-in that never touches the tag.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {                                      // Ego_r.pdb, 12 bytes
    public:
        unsigned long BlockIndex;                           // +0x00
        long          Cost;                                 // +0x04
        long          Benefit;                              // +0x08
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

// Same-TU stand-in for 0x00ADA7E0 (its own row), kept out of line.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }

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
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
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
            __tmp = _M_allocate_and_copy(__n, (const_pointer)this->_M_start, (const_pointer)this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template void vector<CVertexGroupBlockOptimiser::CBlockInfo>::reserve(unsigned int);

} // namespace std
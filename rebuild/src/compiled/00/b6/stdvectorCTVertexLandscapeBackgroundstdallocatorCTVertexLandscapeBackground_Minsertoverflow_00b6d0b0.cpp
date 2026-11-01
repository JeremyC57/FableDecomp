// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CTVertexLandscapeBackground>) STLport 4.6 growth insert: allocate size + max(size, fill_len), copy the head, place the one element or fill n, copy the tail unless at the end, _M_clear the old block, _M_set
// Leaf: ?_M_insert_overflow@?$vector@VCTVertexLandscapeBackground@@V?$allocator@VCTVertexLandscapeBackground@@@std@@@std@@IAEXPAVCTVertexLandscapeBackground@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6D0B0, 404 bytes; `ret 14h`. Callees: malloc @ 0x00BFEA0E (allocator::allocate), the vector's
//          _M_clear @ 0x00B6D250 (out of line: `if (first) free(first)`).
// Identity (naming wave 20, chunk 688): the chunk 688 vector<CTVertexLandscapeBackground>::_M_fill_insert 0x00B6CE00 (proven through the PDB
//   resize 0x00B6CD10) calls this address with (pos, x, __false_type, n, false) = STLport 4.6 _M_insert_overflow; the element is Ego_r
//   CTVertexLandscapeBackground (24 bytes), the landed 0x00BE4380 push_back calls it with (finish, x, tag, 1, true). Was the descriptive
//   CRetailVector_SRecord24::InsertGrow (the catalog "CActiveFile::ExpandAndFillVertices" is propagated).
// Layout: the retail 12-byte STLport vector {_M_start, _M_finish, _M_end_of_storage}; the element's IMPLICIT copy constructor (an explicit
//         memberwise one folds the `mov edx,ecx ; mov esi,eax` pointer copies of each placement copy and shrinks the frame).
// Shape: STLport 4.6 _M_insert_overflow(__false_type): `len = size + max(size, n)` through the reference-returning max, allocate(len) is null
//        for zero, each copy is a placement-new `_Construct` (null-tested), the single-element case is separate from __uninitialized_fill_n,
//        then the out-of-line _M_clear and the inline _M_set. The helpers are __forceinline (plain inline leaves them out of line here).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CTVertexLandscapeBackground {                // Ego_r.pdb, 24 bytes (CTVertexLandscapeBackgroundBase layout; the chunk 688 _M_fill_insert 0x00B6CE00 model)
public:
    unsigned short X;                              // +0x00
    unsigned short Y;                              // +0x02
    float Z;                                       // +0x04
    float NormalX;                                 // +0x08
    float NormalY;                                 // +0x0C
    float NormalZ;                                 // +0x10
    unsigned char UVPad1;                          // +0x14
    unsigned char V1;                              // +0x15
    unsigned char U1;                              // +0x16
    unsigned char UVPad2;                          // +0x17
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                            // 0x00B6D250 (out of line)
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CTVertexLandscapeBackground>::_M_insert_overflow(CTVertexLandscapeBackground*, const CTVertexLandscapeBackground&, const __false_type&, unsigned int, bool);
}

// std::vector<CArray<unsigned short> >::resize(size_type new_size) -- STLport one-argument resize: resize(new_size, CArray<unsigned short>()) through the out-of-line two-argument resize; the empty default array needs no destruction
// Leaf: ?resize@?$vector@V?$CArray@G@@V?$allocator@V?$CArray@G@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3FB60, 36 bytes, thiscall, `ret 4`.
// Identity: byte twin of the landed 0x00A4B1D0 vector<CLipSyncFrameData>::resize(size_type); the only relocation is the two-argument
//   resize 0x00F3FAF0, catalogued std::vector<CArray<unsigned short>>::resize (its 12-byte element stride: `imul 0x2AAAAAAB ; sar 1`, its
//   erase 0x00F3F7C0 / _M_fill_insert 0x00F3F960 of the same vector, in the CTriangleStripifier2 TU), whose only caller is this row
//   (0x00F3FB79). Both PDB name sets hold ?resize@?$vector@V?$CArray@G@@V?$allocator@V?$CArray@G@@@std@@@std@@QAEXI@Z. No caller of
//   0x00F3FB60 itself (unreferenced COMDAT). The catalog label `_Umove<EWeaponClass*>` is wrong.
// Callees: vector<CArray<unsigned short> >::resize(size_type, const value_type&) 0x00F3FAF0 (thiscall, `ret 8`).
// Shape: the twin's (STLport 4.6 `void resize(size_type __new_size) { resize(__new_size, _Tp()); }`): the default value is three zero
//   words in the frame, whose inline destructor folds away because the two-argument resize has a same-TU body that never writes
//   through the value reference.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

namespace std {
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) _Destroy(__first); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);
    static unsigned long _M_fill_value_words;               // stand-in sink (see below)
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x00F3FAF0 for CArray<unsigned short>
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}
// 0x00F3FAF0 resize(n, x) (out of line in retail): a same-TU noinline stand-in (not graded) that, like the retail body,
// reads the value but never writes through the reference (the real one fill-inserts copies of it).
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::resize(unsigned int __new_size, const _Tp& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        _M_fill_value_words = *(const unsigned long*)&__x;
}
#pragma auto_inline(on)
template <class T> class CArray : public std::vector<T> {   // Fable's CArray<T>: an STLport vector<T>, 12 bytes in retail
};
typedef CArray<unsigned short> TElem;
template void std::vector<TElem>::resize(unsigned int);
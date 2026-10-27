// std::vector<CLipSyncFrameData>::resize(size_type new_size) -- STLport one-argument resize: resize(new_size, CLipSyncFrameData()) through the out-of-line two-argument resize; the zero default value needs no destruction
// Leaf: ?resize@?$vector@VCLipSyncFrameData@@V?$allocator@VCLipSyncFrameData@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4B1D0, 36 bytes, thiscall, `ret 4`.
// Identity (proven): FableWin.pdb ?resize@?$vector@VCLipSyncFrameData@@V?$allocator@VCLipSyncFrameData@@@std@@@std@@QAEXI@Z, named by the landed resize(n, x) 0x00A4B160 whose only caller this is (CLipSyncSampleData::FrameData).
// Callees: vector<CLipSyncFrameData>::resize(size_type, const value_type&) 0x00A4B160 (thiscall, `ret 8`).
// Shape (family K030, representative 0x0086DE40): STLport 4.6 `void resize(size_type __new_size) { resize(__new_size,
//   _Tp()); }` -- the default value is three zero words in the frame (a value-initialised pair / an empty CArray), whose
//   inline destructor folds to nothing because its storage pointers are known null -- VC7.1 knows that only because the two-argument resize has a same-TU body that never writes through the value reference (the retail TU holds the real one; declaration-only, the temporary's destroy loop and free stay).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
};
class CLipSyncPhoneme {                                  // Ego_r.pdb, 12 bytes
public:
    CCharString Symbol;
    unsigned long ID;
    float Weight;
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;                                          // +0x04
    pair() : first(_T1()), second(_T2()) { }
};
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
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x00A4B160 for this element
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}
// 0x%(R2)s resize(n, x) (out of line in retail): a same-TU noinline stand-in (not graded) that, like the retail body,
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
class CLipSyncFrameData {                                // 12 bytes in retail (16 in Ego_r.pdb)
public:
    CArray<CLipSyncPhoneme> Phonemes;                    // +0x00
};
typedef CLipSyncFrameData TElem;
template void std::vector<TElem>::resize(unsigned int);
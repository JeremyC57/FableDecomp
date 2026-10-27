// std::vector<CUnprovenCharStringEntry_004d1800>::_M_clear() -- STLport 4.6 `_M_clear()`: destroy every element, then free the block
// Leaf: ?_M_clear@?$vector@VCUnprovenCharStringEntry_004d1800@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D1800, 50 bytes, thiscall, plain `ret`.
// Identity (descriptive): no direct caller or reference; the 8-byte element with a CCharString at +4 is all the bytes say.
// Callees: the element destructor's one call -- a CCharString at +4, ~CCharString 0x0099EAE0; free 0x00BFEA14.
// Shape (family K104, representative 0x0047BD70): the landed vector<pair<CCharString, float> >::_M_clear 0x006FBE60 (family J007)
//   with the destroyed member at another offset: `_Destroy(_M_start, _M_finish)` (inline __destroy_aux loop) then deallocate.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class CCharStringData;
class CCharString {                                            // 4 bytes: PStringData
public:
    ~CCharString();                                            // 0x0099EAE0
    CCharStringData* PStringData;
};
class CUnprovenCharStringEntry_004d1800 {               // 8 bytes: a dword, then a CCharString (unproven element)
public:
    unsigned long First;                                 // +0x00
    CCharString Second;                                  // +0x04
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
template void std::vector<CUnprovenCharStringEntry_004d1800, std::allocator<CUnprovenCharStringEntry_004d1800 > >::_M_clear();
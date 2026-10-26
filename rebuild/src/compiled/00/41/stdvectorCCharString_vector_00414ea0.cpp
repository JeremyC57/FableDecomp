// std::vector<CCharString>::~vector() -- STLport destructor: destroy every string, then free the block
// Leaf: ??1?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00414EA0, 49 bytes, thiscall, plain `ret`.
// Identity (sweep3; FINDINGS chunk 480 "Coverage names to re-check: 0x414EA0 (~vector<CCharString>)"; the catalog name was
//   already right, the source was a "CCombatWheel_ResetRings" stand-in): 4-byte stride, each element destroyed by
//   CCharString::~CCharString 0x0099EAE0, then the block freed (free 0x00BFEA14) when non-null = STLport 4.6 ~vector with the
//   inline ~_Vector_base; Ego_r/FableWin ??1?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAE@XZ; the landed
//   caller 0x00431E40 already binds this address as ??1?$vector@VCCharString@@@std@@QAE@XZ (the one-parameter spelling).
// Callees: CCharString::~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
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
};
}
class CCharStringData;
class CCharString {                                            // 4 bytes: PStringData
public:
    ~CCharString();                                            // 0x0099EAE0
    CCharStringData* PStringData;
};

template std::vector<CCharString, std::allocator<CCharString> >::~vector();

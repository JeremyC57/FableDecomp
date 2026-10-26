// std::vector::_M_clear() -- (vector<CUnprovenElement_00a40d00>) STLport 4.6 protected `_M_clear()` (the _M_insert_overflow reallocation's old-block release): destroy every element (its implicit destructor runs two out-of-line member destructors, the later member first), then free the block
// Leaf: ?_M_clear@?$vector@VCUnprovenElement_00a40d00@@V?$allocator@VCUnprovenElement_00a40d00@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A40D00, 57 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (descriptive): std::vector<CUnprovenElement_00a40d00>::_M_clear(); evidence: DESCRIPTIVE element type; the function is proven _M_clear (its only caller is the _M_insert_overflow 0x00A41390 of the push_back 0x00A41840 used by CSoundManagerLH slot 39 0x00A3F250 on this+0x54 and the landed 0x00A41A70): CSoundManagerLH (vtable 0x0129D2C4) is in neither PDB, and its map stores {two CCharStrings built by 0x0076DD40, a dword from a virtual call} whose class is not named anywhere.
// Label: the manifest's "ResetRings" is propagated (family J113: five destroy-and-free bodies -- STLport ~vector (with the
//   inline ~_Vector_base) and the protected _M_clear() that _M_insert_overflow calls have the same code; retail has no
//   identical-code folding, so each address is one (function, element) pair).
// Callees: CCharString::~CCharString 0x0099EAE0 (on +4, then +0), free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: agent952's H071 model (0x004D1460): `_Destroy(_M_start, _M_finish)` (the element's implicit destructor in an inline
//   __destroy_aux loop, stride 0xC) then the block freed when non-null.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
class CCharStringData;
class CCharString {                                            // 4 bytes: PStringData
public:
    ~CCharString();                                            // 0x0099EAE0
    CCharStringData* PStringData;
};
class CUnprovenElement_00a40d00 {                              // DESCRIPTIVE placeholder: 12 bytes, a two-string key + one dword
public:
    CCharString _str00;                                        // +0x0
    CCharString _str04;                                        // +0x4
    unsigned long _dw08;                                       // +0x8
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

template void std::vector<CUnprovenElement_00a40d00, std::allocator<CUnprovenElement_00a40d00> >::_M_clear();
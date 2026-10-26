// std::vector::_M_clear() -- (vector<CSubtitledMessage >) STLport 4.6 `_M_clear()`: destroy every element, then free the block (the reallocating branch of vector::operator= / reserve)
// Leaf: ?_M_clear@?$vector@VCSubtitledMessage@@V?$allocator@VCSubtitledMessage@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2720, 50 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (proven): vector<CSubtitledMessage >::_M_clear; evidence: its only caller is the vector operator= 0x0071A160, which the CActionTalkBase / CActionTalkToThing constructors (0x00717950, 0x00717AA0, ...) apply to this+0x30 -- Ego_r.pdb's CActionTalkBase::Messages (+0x30), CArray<CSubtitledMessage>; the element destructor 0x004D1DB0 tears down exactly CSubtitledMessage's members (Text +0x0C, Narrator +0x10, the vector maps at +0x14 / +0x24 / +0x34), and the 0x44 stride is the retail CSubtitledMessage size (CameraTags +0x34 plus a 16-byte CVectorMap)
// Label: the manifest's "ResetRings" is propagated (family H072: six destroy-and-free bodies whose element destructor runs on the element itself -- STLport ~vector (with the inline
//   ~_Vector_base) and the protected _M_clear() that vector::operator= calls when it reallocates have the same code; retail has
//   no identical-code folding, so each address is one (function, element) pair).
// Callees: CSubtitledMessage::~CSubtitledMessage 0x004D1DB0 (out of line), free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: STLport 4.6 -- `_Destroy(_M_start, _M_finish)` (the element's implicit destructor in an inline __destroy_aux loop,
//   stride 68) then the block freed when non-null.
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
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
}
class CSubtitledMessage {                                      // 0x44 bytes in retail
public:
    unsigned char Data[0x44];
    ~CSubtitledMessage();                                      // 0x004D1DB0
};

template void std::vector<CSubtitledMessage, std::allocator<CSubtitledMessage > >::_M_clear();
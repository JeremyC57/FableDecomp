// std::vector::_M_clear() -- (vector<CInputEvent >) STLport 4.6 `_M_clear()`: destroy every element, then free the block (the reallocating branch of vector::operator= / reserve)
// Leaf: ?_M_clear@?$vector@VCInputEvent@@V?$allocator@VCInputEvent@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6850, 50 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (proven): vector<CInputEvent >::_M_clear; evidence: its only caller is the vector operator= 0x009F6890, which CInputManager::SetEventsFromEventPackage 0x009F54D0 applies to this+0x10 from the package -- Ego_r.pdb's CInputManager::EventStore (+0x10 after the 12-byte ProcessedEvents), vector<CInputEvent>, assigned from CInputEventPackage::Events
// Label: the manifest's "ResetRings" is propagated (family H072: six destroy-and-free bodies whose element destructor runs on the element itself -- STLport ~vector (with the inline
//   ~_Vector_base) and the protected _M_clear() that vector::operator= calls when it reallocates have the same code; retail has
//   no identical-code folding, so each address is one (function, element) pair).
// Callees: CInputEvent::~CInputEvent 0x00A03B30 (an empty out-of-line `ret`; Ego_r.pdb emits ??1CInputEvent@@QAE@XZ, and its 72 call sites are input code destroying CInputEvent locals), free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: STLport 4.6 -- `_Destroy(_M_start, _M_finish)` (the element's implicit destructor in an inline __destroy_aux loop,
//   stride 52) then the block freed when non-null.
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
class CInputEvent {                                            // 0x34 bytes (Ego_r.pdb)
public:
    unsigned char Data[0x34];
    ~CInputEvent();                                            // 0x00A03B30 (empty, out of line)
};

template void std::vector<CInputEvent, std::allocator<CInputEvent > >::_M_clear();
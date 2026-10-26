// std::vector::~vector() -- (vector<CInputEvent >) out-of-line destructor: destroy every element, then free the vector's own block
// Leaf: ??1?$vector@VCInputEvent@@V?$allocator@VCInputEvent@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6A70, 49 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (probable): vector<CInputEvent >::~vector; evidence: its only caller is the landed CInputManager::LoadAllEventPackages 0x009F5D70 (0x009F5DDB), destroying the local CInputEventPackage at [ebp-0x50] whose first member is Events (Ego_r.pdb CInputEventPackage: vector<CInputEvent> Events +0, then trivially destroyed TimeStamp / Checksum / MousePos / JoystickPos), stride 0x34 = CInputEvent. The out-of-line layer could be the implicit ~CInputEventPackage (same body; the landed caller declares it so, unbound in the catalog); named as vector<CInputEvent>::~vector by the H071 convention -- probable for that layer (vector<CInputEvent>::_M_clear is the separate 0x009F6850).
// Label: the manifest's "ResetRings" is propagated (family J007: six destroy-and-free bodies whose element destructor runs on the element itself -- STLport ~vector (with the inline
//   ~_Vector_base) and the protected _M_clear() that vector::operator= calls when it reallocates have the same code; retail has
//   no identical-code folding, so each address is one (function, element) pair).
// Callees: CInputEvent::~CInputEvent 0x00A03B30 (an empty out-of-line `ret`; Ego_r.pdb emits ??1CInputEvent@@QAE@XZ), free 0x00BFEA14 (allocator::deallocate behind its null test).
// Body: STLport 4.6 -- `_Destroy(_M_start, _M_finish)` (the element's implicit destructor in an inline __destroy_aux loop,
//   stride 52) then the block freed when non-null.
// Model: agent952 H071/H072 (the 0x004D2720 shape).
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

template std::vector<CInputEvent, std::allocator<CInputEvent > >::~vector();
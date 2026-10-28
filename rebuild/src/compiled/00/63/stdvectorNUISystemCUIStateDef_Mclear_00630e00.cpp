// std::vector<NUISystem::CUIStateDef>::_M_clear() -- destroy every element (virtual destructor), then free the block
// Leaf: ?_M_clear@?$vector@VCUIStateDef@NUISystem@@V?$allocator@VCUIStateDef@NUISystem@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00630E00, 50 bytes, thiscall, plain `ret`.
// Identity (proven): its only caller is vector<NUISystem::CUIStateDef>::operator= 0x006315E0 (the grow arm: allocate-and-copy, then this), which CUIDef's constructor 0x006303C0 and operator= 0x00631720 apply to CUIDef::States (Ego_r.pdb vector<NUISystem::CUIStateDef>, 0x84-byte element, 0x7C in retail: the 124-byte stride). The manifest's "~vector<CConversation>" is propagated.
// Callees: the element's virtual destructor (vtable slot 0, flags 0), free 0x00BFEA14.
// Shape (family M172, representative 0x00630E00): the landed _M_clear 0x006F3450 (family K104) with a polymorphic element:
//   `_Destroy(_M_start, _M_finish)` (inline __destroy_aux loop, `p->~T()` through the vtable) then the null-tested free. Speed TU.
extern "C" void __cdecl free(void* block);               // 0x00BFEA14
namespace NUISystem {
class CUIStateDef {                                     // Ego_r.pdb 0x84 bytes, retail 0x7C (12-byte vectors)
public:
    virtual ~CUIStateDef();
    unsigned long Members[30];
};
}
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
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
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
    public:
};
}
template void std::vector<NUISystem::CUIStateDef>::_M_clear();
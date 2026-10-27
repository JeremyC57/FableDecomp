// std::vector::empty() -- true when the scheduler's process stack holds no entries (start == finish)
// Leaf: ?empty@?$vector@PAVCASuspendableProcess@@V?$allocator@PAVCASuspendableProcess@@@std@@@std@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A44930, 11 bytes. std::vector<CASuspendableProcess*>::empty() const, the
//   out-of-line COMDAT the unoptimised CSuspendableProcessMicrothread::RunSuspendableProcess (0x00A44880) calls on
//   the process stack g_ProcessStack (0x013D2828). The instantiation keeps the header's optimisation state, so the
//   body is optimised: `start - finish ; neg ; sbb ; inc` is the 0/1 of `start == finish`.
// Layout: STLport 4.6 vector, 12 bytes (_M_start +0, _M_finish +4, _M_end_of_storage +8).
class CASuspendableProcess;

namespace std {

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {   // the end-of-storage pointer
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    bool empty() const { return this->_M_start == this->_M_finish; }
};

}   // namespace std

template bool std::vector<CASuspendableProcess*, std::allocator<CASuspendableProcess*> >::empty() const;
// std::vector<ENavigatorType>::_M_clear() -- STLport: destroy [start, finish) (trivial for the enum) and release the block
// Leaf: ?_M_clear@?$vector@W4ENavigatorType@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006715F0, 14 bytes, thiscall `ret`: `if (_M_start) free(_M_start)` (0x00BFEA14).
// Identity (FINDINGS chunk 383; was a `__fastcall OnReadFinished(S*)` stand-in calling an "orf_notify"): its only caller
//   is the landed std::vector<ENavigatorType>::operator= 0x006714B0 (call at 0x006714EB in the reallocate branch, which
//   already declares 0x006715F0 as this _M_clear), itself the copy of CThingCreatureDef::NavigatorTypes from
//   CThingCreatureDef::operator= 0x00671160 (Ego_r.pdb CThingCreatureDef::NavigatorTypes is vector<ENavigatorType>).
// Shape: STLport 4.x `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start,
//   _M_end_of_storage._M_data - _M_start); }`; the enum's _Destroy is empty and deallocate is the null-tested free. Model
//   (allocator / _STLP_alloc_proxy / _Vector_base) as in the landed 0x006714B0, so both TUs bind one symbol.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
enum ENavigatorType { NAVIGATOR_TYPE_0 };          // 4-byte enum

namespace std {
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_clear();
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
}

template void vector<ENavigatorType>::_M_clear();
}

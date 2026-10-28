// std::vector<CEngineLocalDetailObjectDef>::_M_clear() -- STLport vector storage release for the detail-object def vector: destroy the elements (out-of-line range destroy), then deallocate the block (free when non-null)
// Leaf: ?_M_clear@?$vector@VCEngineLocalDetailObjectDef@@V?$allocator@VCEngineLocalDetailObjectDef@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00477BFC, 28 bytes, thiscall, plain `ret` (size-optimised def TU: unaligned, pop ecx).
// Identity: the only caller is std::vector<CEngineLocalDetailObjectDef>::operator= 0x00477AB7 (STLport's `_M_clear()` slot
//   between _M_allocate_and_copy 0x00477C18 and the `_M_start = tmp` store; that row binds
//   ?_M_clear@?$vector@..CEngineLocalDetailObjectDef..@std@@QAEXXZ here); the callee 0x004774B9 is the renamed
//   std::_Destroy<CEngineLocalDetailObjectDef*> (__fastcall, ecx/edx) and 0x00BFEA14 the MSVCR71 free import thunk
//   (STLport allocator::deallocate).  The landed names "CActiveFile::OnReadFinished" / "StdList_Dtor" were propagated.
// Shape: STLport 4.6 `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start, ...); }`;
//   _M_finish loaded into edx before _M_start into ecx; deallocate is `if (p) free(p)`.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CEngineLocalDetailObjectDef {                         // retail 0x88 bytes, polymorphic (vtable 0x0123634C)
public:
    virtual ~CEngineLocalDetailObjectDef();
    unsigned char _body[0x84];
};

namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);   // 0x004774B9

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    protected:
    void _M_clear();
    public:
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
}

template void vector<CEngineLocalDetailObjectDef>::_M_clear();

} // namespace std

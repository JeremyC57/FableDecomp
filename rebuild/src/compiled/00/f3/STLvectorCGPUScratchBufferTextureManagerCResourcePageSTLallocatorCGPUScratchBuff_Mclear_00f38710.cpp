// std::vector<CResourcePage>::_M_clear() -- scratch-texture memory pages: destroy every page (freeing its ResourceList block), then free the vector's own block
// Leaf: ?_M_clear@?$vector@UCResourcePage@CGPUScratchBufferTextureManager@@V?$allocator@UCResourcePage@CGPUScratchBufferTextureManager@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F38710, 58 bytes, thiscall, plain `ret`: start/finish loaded once, for each 0x1C-byte
//   page `if ([esi+4]) free([esi+4])`, then `if (start) free(start)` (free 0x00BFEA14).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 453 "0xF384xx-0xF388xx Vector_* = vector<CResourcePage>"; hand453 README label
//   table "0x00F38710 | vector<CResourcePage>::_M_clear"): its only caller is the landed
//   std::vector<CGPUScratchBufferTextureManager::CResourcePage>::_M_insert_overflow 0x00F38770 (call at 0x00F3883F), whose
//   model declares this out-of-line _M_clear and binds this Leaf -> 00f38710 in reloc_bindings; CResourcePage is that row's
//   Ego_r.pdb layout (ScratchPool +0, ResourceList vector<long> +4, NextFreeBufferAddress +0x10, FreeSpaceSize +0x14,
//   EnableAutoRestore +0x18; 0x1C bytes with the 12-byte STLport vector). The landed CRetailVector_00f38710::Destroy name
//   was descriptive.
// Shape: STLport 4.6 `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(...); }` with the page's implicit
//   destructor running ~vector<long> (trivial element destroy, then the base's `if (p) free(p)`).
// Access: public as in the existing binding (STLport 4.6 declares _M_clear in a protected section; the bytes are the same).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
namespace std {
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
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }                                      // long: trivial element destroy
    void _M_clear();
};
}   // namespace std
class CGPUScratchBufferTextureManager {
public:
    struct CResourcePage {                             // 0x1c bytes in retail (Ego_r.pdb 32: CArray<long> is 16 there)
        long                ScratchPool;               // +0x00
        std::vector<long>  ResourceList;              // +0x04
        unsigned char*      NextFreeBufferAddress;     // +0x10
        long                FreeSpaceSize;             // +0x14
        bool                EnableAutoRestore;         // +0x18
    };
};
typedef CGPUScratchBufferTextureManager::CResourcePage CResourcePage;
namespace std {
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_clear()
{
    _Destroy(this->_M_start, this->_M_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
}
template void vector<CResourcePage>::_M_clear();
}   // namespace std

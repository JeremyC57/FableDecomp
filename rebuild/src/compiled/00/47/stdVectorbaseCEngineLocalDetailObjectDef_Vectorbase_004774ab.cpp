// std::_Vector_base<CEngineLocalDetailObjectDef>::~_Vector_base() -- STLport vector storage destructor: free the block when
//   there is one
// Leaf: ??1?$_Vector_base@VCEngineLocalDetailObjectDef@@V?$allocator@VCEngineLocalDetailObjectDef@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004774AB, 14 bytes (`mov eax,[ecx] ; test ; je ; push eax ; call free ; pop ecx ; ret`).
// Identity: its only caller is CEngineLocalDetailLayerDef::~CEngineLocalDetailLayerDef 0x0047747C, on the member at +0x48
//   right after std::_Destroy 0x004774B9 over [start, finish) -- the inlined STLport ~vector (`_Destroy(_M_start,
//   _M_finish)`) followed by its base's out-of-line destructor; Ego_r.pdb CEngineLocalDetailLayerDef::Objects is the
//   vector<CEngineLocalDetailObjectDef> (PDB +80 = retail +0x48 after the 12-byte STLport SpacingFromLayer), and the
//   landed _Vector_base<CEngineLocalDetailObjectDef> constructor sits at 0x00477530. The catalog's FreeMember0 (and the
//   manifest's propagated CActiveFile::OnReadFinished) are descriptions.
// Callees: free 0x00BFEA14 (the MSVCR71 import thunk) through the allocator's deallocate.
// Shape: STLport 4.x `~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start,
//   _M_end_of_storage._M_data - _M_start); }`; the size is dead. Pragma: optimize("s") (`pop ecx` after free); the size
//   TU inlines the allocator's deallocate only as `__forceinline`.
#pragma optimize("s", on)

extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CEngineLocalDetailObjectDef {
public:
    unsigned char _body[0x88];
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base();
};

template <class _Tp, class _Alloc>
_Vector_base<_Tp, _Alloc>::~_Vector_base()
{
    if (_M_start != 0)
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
}

template class _Vector_base<CEngineLocalDetailObjectDef, allocator<CEngineLocalDetailObjectDef> >;

}

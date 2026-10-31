// std::copy_backward(CHandleBasedMemoryPool_012b4098::CHandleInfo* _First, CHandleBasedMemoryPool_012b4098::CHandleInfo* _Last, CHandleBasedMemoryPool_012b4098::CHandleInfo* _Dest) -- VC7.1 Dinkumware copy_backward over the pool's 4-byte handle-table entry (a class, so the non-scalar `_Copy_backward` is inlined): `while (_First != _Last) *--_Dest = *--_Last; return _Dest;`
// Leaf: ??$copy_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29250, 35 bytes (+ int3 padding), __fastcall (_First in ecx, _Last in edx,
//   _Dest on the stack, `ret 4`).
// Caller: vector<CHandleBasedMemoryPool_012b4098::CHandleInfo>::_Insert_n @ 0x00C298D0 (call at 0x00C29AC5), the handle table's
//   insert path (push_back @ 0x00C29C30 <- InternalAllocateHandle @ 0x00C28A80). No callees.
// Identity: the libpk pool TU uses VC7.1's own Dinkumware STL (16-byte vectors: allocator word + first/last/end; `_Xlen`
//   "vector<T> too long" @ 0x00C28DC0 / 0x00C28E50). The element is Ego_r.pdb's CHandleBasedMemoryPool_012b4098::CHandleInfo
//   (4 bytes: a union of the block header pointer and the free-handle link), a class type, so _Ptr_cat picks the
//   non-scalar tag and the copy is an element loop, not the memmove of the scalar twin @ 0x00C29290. The tag dispatch is
//   inlined (`ret 4` = three arguments).
// Shape: the empty-range test before the esi save, the element copied through esi, `cmp edx,ecx` scheduled before the
//   store.
// Label correction: the catalog's `_Copy_backward<CPixelFormat*,CPixelFormat*>` is an Ego_r helper propagated across the
//   byte-identical body (CPixelFormat is an engine type, not a pool type).
class CHandleBasedMemoryPool_012b4098 {
public:
    class CHeader;
    class CHandleInfo { public:                    // Ego_r.pdb: 4 bytes
        union { CHeader* Header; long NextFreeHandle; };
    };
};

namespace std {
struct _Nonscalar_ptr_iterator_tag { };

template<class _BidIt1, class _BidIt2> inline
_BidIt2 _Copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}

template<class _BidIt1, class _BidIt2>
_BidIt2 __fastcall copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest)
{
    return (_Copy_backward(_First, _Last, _Dest, _Nonscalar_ptr_iterator_tag()));
}

template CHandleBasedMemoryPool_012b4098::CHandleInfo* __fastcall
copy_backward<CHandleBasedMemoryPool_012b4098::CHandleInfo*, CHandleBasedMemoryPool_012b4098::CHandleInfo*>(
    CHandleBasedMemoryPool_012b4098::CHandleInfo*, CHandleBasedMemoryPool_012b4098::CHandleInfo*, CHandleBasedMemoryPool_012b4098::CHandleInfo*);
}
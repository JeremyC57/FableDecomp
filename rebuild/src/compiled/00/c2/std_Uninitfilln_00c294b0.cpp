// std::_Uninit_fill_n(CHandleBasedMemoryPool::CHandleInfo* _First, unsigned int _Count, const CHandleBasedMemoryPool::CHandleInfo& _Val, allocator& _Al, _Nonscalar_ptr_iterator_tag) -- VC7.1 Dinkumware uninitialised fill of the pool's handle-table entries: construct _Count copies of _Val into raw storage through the allocator (placement copy, null-tested)
// Leaf: ??$_Uninit_fill_n@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C294B0, 34 bytes (+ int3 padding), __fastcall (_First in ecx, _Count in edx,
//   _Val / _Al / the empty tag on the stack, `ret 0Ch`). No callees.
// Callers: vector<CHandleBasedMemoryPool::CHandleInfo>::_Ufill @ 0x00C29840 (call 0x00C29859) and the
//   `_Uninitialized_fill_n(_First, _Count, _Val, _Al)` front end @ 0x00C297A0 (call 0x00C297AF; it pushes its allocator
//   argument twice, the second copy filling the uninitialised tag slot), which the handle vector's _Insert_n @ 0x00C298D0
//   and push_back @ 0x00C29C30 call. The CHeader* vector of the same TU fills inline through the scalar path (_Ufill
//   @ 0x00C29580, no null test).
// Identity: Ego_r.pdb CHandleBasedMemoryPool::CHandleInfo is a 4-byte class (a union of the block header pointer and the
//   free-handle link), so _Ptr_cat gives the non-scalar tag and each element is built with allocator::construct, the
//   placement new whose null test is the `test ecx,ecx ; je` in the loop. The try/catch (destroy the built prefix and
//   rethrow) is the Dinkumware body; nothing in it can throw, so VC7.1 drops the EH frame.
// Shape: unsigned count (`test edx,edx ; jbe`), the value pointer loaded once into eax, one dword copy through esi.
// Label correction: the catalog's `_Fill_n<unsigned long*,unsigned int,unsigned long>` is an Ego_r helper propagated across the
//   byte-identical body; the landed push_back 0x00C29C30 declares it with the scalar tag over unsigned long (relocation-masked,
//   harmless) but the null test makes it the non-scalar instantiation over the handle entry.
inline void* __cdecl operator new(unsigned int, void* _Where) throw() { return (_Where); }

class CHandleBasedMemoryPool {
public:
    struct CHeader;
    struct CHandleInfo {                    // Ego_r.pdb: 4 bytes
        union { CHeader* Header; long NextFreeHandle; };
    };
};

namespace std {
struct _Nonscalar_ptr_iterator_tag { };

template<class _Ty> inline void _Construct(_Ty* _Ptr, const _Ty& _Val) { new ((void*)_Ptr) _Ty(_Val); }
template<class _Ty> inline void _Destroy(_Ty* _Ptr) { _Ptr->~_Ty(); }

template<class _Ty> class allocator {
public:
    void construct(_Ty* _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
    void destroy(_Ty* _Ptr) { _Destroy(_Ptr); }
};

template<class _FwdIt, class _Diff, class _Tval, class _Alloc>
void __fastcall _Uninit_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    _FwdIt _Next = _First;
    try {
        for (; 0 < _Count; --_Count, ++_First)
            _Al.construct(_First, _Val);
    } catch (...) {
        for (; _Next != _First; ++_Next)
            _Al.destroy(_Next);
        throw;
    }
}

template void __fastcall _Uninit_fill_n<CHandleBasedMemoryPool::CHandleInfo*, unsigned int, CHandleBasedMemoryPool::CHandleInfo,
    allocator<CHandleBasedMemoryPool::CHandleInfo> >(CHandleBasedMemoryPool::CHandleInfo*, unsigned int,
    const CHandleBasedMemoryPool::CHandleInfo&, allocator<CHandleBasedMemoryPool::CHandleInfo>&, _Nonscalar_ptr_iterator_tag);
}
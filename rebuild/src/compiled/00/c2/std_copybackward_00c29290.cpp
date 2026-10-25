// std::copy_backward(CHandleBasedMemoryPool::CHeader** _First, CHandleBasedMemoryPool::CHeader** _Last, CHandleBasedMemoryPool::CHeader** _Dest) -- VC7.1 Dinkumware copy_backward over the defragment stack's block-header pointers: the scalar-pointer `_Copy_backward` inlined, one memmove of the whole range to end at _Dest
// Leaf: ??$copy_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29290, 33 bytes, __fastcall (_First in ecx, _Last in edx, _Dest on the
//   stack, `ret 4`). Callee: MSVCR71 memmove (import 0x01440144).
// Caller: vector<CHandleBasedMemoryPool::CHeader*>::_Insert_n @ 0x00C295B0 (call at 0x00C29771), the defragment block stack's
//   insert path (push_back @ 0x00C29B30 <- PartialDefragment @ 0x00C28870).
// Identity: byte-identical to chunk 433's 0x00C13D40 / 0x00C13D70 (the same VC7.1 Dinkumware copy_backward over another
//   4-byte scalar); _Ptr_cat(T**, T**) is the scalar tag, so the pointer element takes the memmove path. The element type
//   follows from the caller (Ego_r CArray<CHandleBasedMemoryPool::CHeader*>, the pool's partial-defragment stack).
// Shape: `sub edx,ecx ; sar edx,2` is the element count, `lea eax,[edx*4]` its byte size, the destination `_Dest - _Off`
//   formed after the first two pushes; the memmove result is returned directly.
// Label correction: the catalog's `_Copy_backward<EWeaponClass*,EWeaponClass*>` is an Ego_r helper propagated across the
//   byte-identical body (EWeaponClass is a game enum, not a pool type).
class CHandleBasedMemoryPool {
public:
    struct CHeader;                         // a pool block header (32 bytes)
};

extern "C" __declspec(dllimport) void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x01440144

namespace std {
struct _Scalar_ptr_iterator_tag { };

template<class _BidIt1, class _BidIt2> inline
_BidIt2 _Copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Scalar_ptr_iterator_tag)
{
    int _Off = _Last - _First;
    return ((_BidIt2)memmove(&*_Dest - _Off, &*_First, _Off * sizeof(*_First)));
}

template<class _BidIt1, class _BidIt2>
_BidIt2 __fastcall copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest)
{
    return (_Copy_backward(_First, _Last, _Dest, _Scalar_ptr_iterator_tag()));
}

template CHandleBasedMemoryPool::CHeader** __fastcall
copy_backward<CHandleBasedMemoryPool::CHeader**, CHandleBasedMemoryPool::CHeader**>(
    CHandleBasedMemoryPool::CHeader**, CHandleBasedMemoryPool::CHeader**, CHandleBasedMemoryPool::CHeader**);
}
// std::__unguarded_linear_insert(CVertexGroupBlockOptimiser::CBlockInfo* last, CBlockInfo val, less<CBlockInfo> comp) -- STLport unguarded insertion step over block candidates: shift every larger-cost record up one slot and drop the value into the hole
// Flags: /Gr
// Leaf: ??$__unguarded_linear_insert@PAVCBlockInfo@CVertexGroupBlockOptimiser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADAA30, 0x43 bytes, __fastcall (last in ecx), `ret 10h` (the 12-byte value
// and the empty 4-byte comparator on the stack).
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8), ordered
// by CBlockInfo::operator< on Cost (`cmp edx,[eax+4] ; jge` = signed less); the same instantiation family as the
// landed __push_heap 0x00ADAAA0, __adjust_heap 0x00ADB000 and __insertion_sort 0x00ADBE70 (its out-of-line caller).
// Catalog "Insertion_ShiftAndInsert" is descriptive. No callees.
// Shape: STLport 4.x `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; }
// *last = val;` -- val.Cost lives in edx, the moves are three dword copies.
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

} // namespace std

class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {                       // Ego_r.pdb, 12 bytes
    public:
        unsigned long BlockIndex;            // +0x0
        long          Cost;                  // +0x4
        long          Benefit;               // +0x8
        bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
    };
};

namespace std {
template void __unguarded_linear_insert<CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo,
                                        less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo, less<CVertexGroupBlockOptimiser::CBlockInfo>);
}
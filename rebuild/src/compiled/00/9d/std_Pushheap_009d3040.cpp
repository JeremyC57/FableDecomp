// std::_Push_heap(unsigned long* first, int hole, int top, unsigned long val, CBankFile::CompareDataOffset pred) -- percolate a bank entry index up a max-heap ordered by each entry's data offset (packed or from the runtime record)
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x009D3040, 192 bytes incl. int3 padding (/O2). Called from the landed _Adjust_heap
// @ 0x009D3750 (same instantiation). Callee: CPackedUIntArray::operator[] 0x00A629F0 (landed).
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over plain entry indices; the
// comparator (Ego_r.pdb CBankFile::CompareDataOffset { CBankFile* BankFile; }) is inlined: each side's
// offset is the bank's packed offset (1-based: `index - 1`) when PackedDataOffset.Size is non-zero,
// else RuntimeData[index].DataOffset; the first offset is parked in the comparator's own (dead)
// argument slot and the compare is UNSIGNED (`jae`). Each exit path stores `val` on its own (three
// `ret 0Ch` tails). Layout: engine/CBankFile.h with FABLE_RETAIL_BANK_FILE (retail 12-byte CArrays):
// RuntimeData +0x20 (12-byte CRuntimeData, DataOffset first), PackedDataOffset +0x38 (CPackedUIntArray:
// PackedInts +0, Size +4); the accessors are those of the landed CompareDataOffset::operator() 0x009CC640.
// /Gr: first=ecx, hole=edx; `ret 0Ch` = top + val + the one-pointer comparator.
#define FABLE_RETAIL_BANK_FILE
#include "engine/CBankFile.h"
namespace N009d3040 {
struct CRuntimeData {                                     // CBankFile::CRuntimeData, Ego_r.pdb 12 bytes
    unsigned long DataOffset;                             // +0x00
    unsigned long DataSize;                               // +0x04
    unsigned char DataType;                               // +0x08
    bool          Valid;                                  // +0x09
};
struct CPackedUIntArray {                                 // Ego_r.pdb 16 bytes
    unsigned long* PackedInts;                            // +0x00
    unsigned long  Size;                                  // +0x04
    unsigned char  Bits;                                  // +0x08
    unsigned long  Bias;                                  // +0x0C
    unsigned long operator[](unsigned long index);        // 0x00A629F0
};
struct CRuntimeDataArray {                                // CArray<CBankFile::CRuntimeData>: first / last / end
    CRuntimeData* First;
    CRuntimeData* Last;
    CRuntimeData* End;
};
inline CPackedUIntArray& PackedDataOffsetOf(CBankFile* bank) { return *(CPackedUIntArray*)bank->PackedDataOffset; }
inline CRuntimeDataArray& RuntimeDataOf(CBankFile* bank) { return *(CRuntimeDataArray*)bank->RuntimeData; }
inline unsigned long GetEntryDataOffset(CBankFile* bank, unsigned long index)
{
    if (PackedDataOffsetOf(bank).Size != 0)
        return PackedDataOffsetOf(bank)[index - 1];
    return RuntimeDataOf(bank).First[index].DataOffset;
}
struct CompareDataOffset {                                // CBankFile::CompareDataOffset
    CBankFile* BankFile;                                  // +0x00
    bool operator()(unsigned long a, unsigned long b) const
    {
        return GetEntryDataOffset(BankFile, a) < GetEntryDataOffset(BankFile, b);
    }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
template void _Push_heap<unsigned long*, int, unsigned long, CompareDataOffset>(unsigned long*, int, int, unsigned long, CompareDataOffset);
}
// _unattributed::SortRecordsAndClearDirty() -- sorted-container Sort(): std::sort the m_count 0x38-byte records with the container's comparator, then clear the dirty flag
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A35330, 30 bytes. Manifest label "CSetIllustratorAsDrawing::operator()"
// (with a CDrawBase* argument) is propagated and rejected: the body takes no stack argument (`ret`) and its
// only callee is the SGI introsort front end over 56-byte records. Twin at 0x00A35310 (20-byte records,
// sort @ 0x00A35150). The owning container (a CVectorMap-like sorted array: comparator byte +0, data +4,
// count +8, Dirty +0xC -- not the PDB CVectorMap layout, whose vector precedes Compare/Dirty) is not
// recoverable from the bytes, so the leaf states what the body does. No direct caller references it.
// Callees: sort(first, last, compare) @ 0x00A351B0 (/Gr `__fastcall`: first in ecx, last in edx, the empty
// comparator by value on the stack; manifest "Introsort_Quicksort_Large"; the 0x92492493 / `sar 5`
// reciprocal is the divide by 56).
//
// Shape: the end pointer is `m_pData + m_count` (`imul edx,edx,38h ; add edx,ecx` with the data pointer
// left in ecx as the first argument); the empty comparator passed by value is materialised as
// `xor eax,eax ; mov al,[esi] ; push eax`; the dirty byte is cleared after the call.
// Leaf: SortRecordsAndClearDirty

struct CRecord38 {                               // opaque 0x38-byte record
    unsigned char _body[0x38];
};

struct CRecordCompare {                          // empty comparator functor
};

void __fastcall SortRecords(CRecord38* first, CRecord38* last, CRecordCompare compare);   // 0x00A351B0

struct CSortedRecords {
    CRecordCompare Compare;                      // +0x00
    CRecord38*     m_pData;                      // +0x04
    long           m_count;                      // +0x08
    bool           Dirty;                        // +0x0C
    void SortRecordsAndClearDirty();
};

void CSortedRecords::SortRecordsAndClearDirty()
{
    SortRecords(m_pData, m_pData + m_count, Compare);
    Dirty = false;
}

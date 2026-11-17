// CArray<CCharString>::DeallocateMemory() -- release the storage: when there is any capacity, erase every element (the out-of-line
//   erase), then swap the block out into an empty array whose destructor destroys the detached range and frees it
// Leaf: ?DeallocateMemory@?$CArray@VCCharString@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3FB0, 95 bytes, thiscall, plain `ret`.
// Identity (NF48, round 7): Ego_r.pdb ?DeallocateMemory@?$CArray@VCCharString@@@@QAEXXZ (0x00B21960 there) and FableWin.pdb (0x02F9D780);
//   retail calls it out of line on CBankFile::Symbols (a CArray<CCharString>) from CBankFile::CloseEntries 0x009CD850 and
//   AllocateBuffers 0x009CEAE0. No caller is a destructor: the earlier label ~CArray<CCharString> was wrong.
// Callees: vector<CCharString>::erase(iterator, iterator) 0x0043336A, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Shape: `if (capacity() > 0) { clear(); CArray<T> empty; swap(empty); }` on the shared engine/CArray.h (the both-PDB declaration set,
//   DeallocateMemory declared without a body) and stlport/fable_containers.h (clear over the out-of-line erase, inline swap, the empty
//   array's inline destructor).
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CArray.h"

template<class T> void CArray<T>::DeallocateMemory()
{
    if (this->capacity() > 0) {
        this->clear();
        CArray<T> empty;
        this->swap(empty);
    }
}
template void CArray<CCharString>::DeallocateMemory();

// CArray<CBankFile::CRuntimeData>::DeallocateMemory() -- release the array's storage: when it holds any capacity, erase every element and
// swap the storage out into an empty array, whose destructor frees the block
// Leaf: ?DeallocateMemory@?$CArray@VCRuntimeData@CBankFile@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3D90, length 90 bytes.
// Callees: the random-access __copy<CRuntimeData*> 0x009D1C30 (__fastcall: first in ecx, last in edx, result + the iterator category tag
//   + the Distance* on the stack), free 0x00BFEA14.
// NF72 (round 7, q10869): the element type is CBankFile::CRuntimeData (12 bytes, the 0x2AAAAAAB stride): Ego_r.pdb 0x00B21710 and
//   FableWin.pdb 0x02F9E000 carry ?DeallocateMemory@?$CArray@VCRuntimeData@CBankFile@@@@QAEXXZ, and the retail callers
//   CBankFile::AllocateBuffers 0x009CEAE0 (NF68 port, on RuntimeData +0x20) and PackRuntimeData 0x009CD740 call it on CBankFile::RuntimeData.
//   Moved off the local SElement12 / std::vector / CArray models onto engine/CArray.h + stlport/fable_containers.h + engine/CBankFile.h,
//   with the text of the landed sibling CArray<CCharString>::DeallocateMemory 0x009D3FB0 (NF48):
//   `if (capacity() > 0) { clear(); CArray<T> empty; swap(empty); }`.
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#include "engine/CBankFile.h"

template<class T> void CArray<T>::DeallocateMemory()
{
    if (this->capacity() > 0) {
        this->clear();
        CArray<T> empty;
        this->swap(empty);
    }
}
template void CArray<CBankFile::CRuntimeData>::DeallocateMemory();

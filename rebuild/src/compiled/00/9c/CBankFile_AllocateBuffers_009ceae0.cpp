// 009ceae0 CBankFile::AllocateBuffers
// Flags: /O2 /Oy /W3
// Leaf: ?AllocateBuffers@CBankFile@@AAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CEAE0, 375 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb 0x00B1CED0 / FableWin.pdb 0x02F9BF90 ?AllocateBuffers@CBankFile@@AAEXK@Z (private); retail caller 0x009CFBDB.
// Port (NF68, round 7): the local X_009ceae0 models (CCharString, CRuntimeData, CArray, the SymbolTree map stand-in and the
//   CBankFileMethods adapter) are replaced by the shared headers: engine/CBankFile.h typed members (FILENAMEINDICES /
//   ADDENTRYMULTIPLEFILES, plus the NF68 opt-ins SYMBOLINDICES, CRUNTIMEDATA_CTOR and ALLOCATEBUFFERS), engine/CArray.h (the
//   both-PDB declaration set; DeallocateMemory defined below as the landed 0x009D3FB0 body), fable_string.h CCharString and
//   stlport/fable_containers.h / fable_map.h (resize, swap, map clear).
// Callees: CArray<CBankFile::CRuntimeData>::DeallocateMemory 0x009D3D90, vector<CRuntimeData>::resize(n, x) 0x009D4010,
//   CArray<CCharString>::DeallocateMemory 0x009D3FB0, vector<CCharString>::resize(n) 0x0049B760, vector<unsigned long>::resize
//   0x00464931, vector<CBankFileEntryUpdateData*>::resize 0x009D3DF0, the map<CCharString,unsigned long> tree _M_erase 0x00579435
//   (SymbolIndices and the FilenameIndices multimap share the tree type), memmove 0x00BFEAE6, free 0x00BFEA14.
// Shape: the unsigned long / pointer arrays' DeallocateMemory expands inline (`if (capacity() > 0) { clear(); CArray<T> empty;
//   swap(empty); }`: memmove-erase then the empty array's free); the CRuntimeData temporary is zeroed member-wise by its ctor.
#include "fable_compiler.h"
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILE_SYMBOLINDICES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#define FABLE_RETAIL_CBANKFILE_ALLOCATEBUFFERS
#include "engine/CBankFile.h"

template<class T> FABLE_FORCEINLINE void CArray<T>::DeallocateMemory()
{
    if (this->capacity() > 0) {
        this->clear();
        CArray<T> empty;
        this->swap(empty);
    }
}
template<> void CArray<CBankFile::CRuntimeData>::DeallocateMemory();   // 0x009D3D90
template<> void CArray<CCharString>::DeallocateMemory();              // 0x009D3FB0

void CBankFile::AllocateBuffers(unsigned long size)
{
    Size = size;
    RuntimeData.DeallocateMemory();
    RuntimeData.resize(size, CRuntimeData());
    Symbols.DeallocateMemory();
    if (OpenFlags & 8)
        Symbols.resize(size);
    Checksums.DeallocateMemory();
    if (OpenFlags & 16)
        Checksums.resize(size, 0);
    UpdateData.DeallocateMemory();
    if (OpenFlags & 32)
        UpdateData.resize(size, 0);
    SymbolIndices.clear();
    FilenameIndices.clear();
}

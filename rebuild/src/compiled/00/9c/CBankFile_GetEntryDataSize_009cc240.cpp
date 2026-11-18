// CBankFile::GetEntryDataSize(unsigned long index) const -- the entry's data size from the packed array or the runtime record
// Leaf: ?GetEntryDataSize@CBankFile@@QBEKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC240. Callee: CPackedUIntArray::operator[] @ 0x00A629F0 (tail jump, the argument slot
//   rewritten to index-1).
// Retail bytes:  mov eax,[ecx+4Ch] ; test eax,eax ; mov eax,[esp+4] ; je R ; dec eax ; mov [esp+4],eax ; add ecx,48h ; jmp operator[] ; R: lea edx,[eax+eax*2] ; mov eax,[ecx+20h] ; mov eax,[eax+edx*4+4] ; ret 4
// NF72 (round 7, q10882): Ego_r.pdb 0x00B19F40 ?GetEntryDataSize@CBankFile@@QBEKK@Z (const), declared by engine/CBankFile.h ENTRYDATASIZE; the
//   ported callers 0x00A42980 / 0x00A25BD0 (NF68) and 0x009C80F0 / 0x00AF1360 (NF72) bind it. Moved off the CBankFile_Retail stand-in and its
//   local CRuntimeData / CPackedUIntArray models onto engine/CBankFile.h (FILENAMEINDICES / CRUNTIMEDATA_CTOR and the overlay opt-in
//   PACKRUNTIMEDATA: PackedDataSize +0x48 typed CPackedUIntArray) and engine/CPackedUIntArray.h: the text of the header's inline
//   GetEntryDataType (the same shape on PackedDataType +0x58 / DataType) on PackedDataSize / DataSize.
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#define FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#include "engine/CBankFile.h"

unsigned long CBankFile::GetEntryDataSize(unsigned long index) const
{
    if (PackedDataSize.GetSize())
        return PackedDataSize[index - 1];
    return RuntimeData[index].DataSize;
}

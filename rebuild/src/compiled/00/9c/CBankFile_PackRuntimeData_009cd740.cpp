// CBankFile::PackRuntimeData() -- retain packed entry arrays only when they consume less memory than the runtime records
// Leaf: ?PackRuntimeData@CBankFile@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CD740, 270 bytes.
// Calls: CPackedUIntArray::Initialise 0x00A62D80, CPackedUIntArray::Sizeof 0x00A62A60, CPackedUIntArray::Clear 0x00A629D0,
//   CArray<CBankFile::CRuntimeData>::DeallocateMemory 0x009D3D90 (tail jump).
// NF72 (round 7, q10869): moved off its local CBankFile / CArray / CRuntimeData / CPackedUIntArray models onto engine/CBankFile.h
//   (FILENAMEINDICES / ADDENTRYMULTIPLEFILES / CRUNTIMEDATA_CTOR, and the overlay opt-in PACKRUNTIMEDATA: PackedDataOffset / PackedDataSize
//   typed CPackedUIntArray as in Ego_r.pdb, the private PackRuntimeData), engine/CPackedUIntArray.h (overlay opt-in PACK) and engine/CArray.h.
//   The calls now bind the PDB spellings: Ego_r.pdb 0x00B21710 ?DeallocateMemory@?$CArray@VCRuntimeData@CBankFile@@@@QAEXXZ (was the
//   made-up ClearAndFree), ?Sizeof@CPackedUIntArray@@QBEKXZ (Ego_r 0x00C033B0 / FableWin 0x033AC100; was the made-up GetMemoryUsage), and the
//   row itself is the private ?PackRuntimeData@CBankFile@@AAEXXZ (Ego_r 0x00B1C8D0). The byte sizes stay long (retail `jge`).
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_C
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CArray.h"
#define FABLE_RETAIL_CPACKEDUINTARRAY_PACK
#include "engine/CPackedUIntArray.h"
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#define FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA
#include "engine/CBankFile.h"

void CBankFile::PackRuntimeData()
{
    unsigned long count = RuntimeData.size();
    if (count > 1) {
        CRuntimeData* records = &RuntimeData[1];
        --count;
        PackedDataOffset.Initialise(&records->DataOffset, 32, sizeof(CRuntimeData), count);
        PackedDataSize.Initialise(&records->DataSize, 32, sizeof(CRuntimeData), count);
        PackedDataType.Initialise(&records->DataType, 8, sizeof(CRuntimeData), count);
        PackedValid.Initialise(&records->Valid, 8, sizeof(CRuntimeData), count);
        long unpacked_size = (RuntimeData.capacity() + 1) * sizeof(CRuntimeData);
        long packed_size = PackedDataOffset.Sizeof() + PackedDataSize.Sizeof() + PackedDataType.Sizeof() + PackedValid.Sizeof();
        if (packed_size < unpacked_size)
            RuntimeData.DeallocateMemory();
        else {
            PackedDataOffset.Clear();
            PackedDataSize.Clear();
            PackedDataType.Clear();
            PackedValid.Clear();
        }
    }
}

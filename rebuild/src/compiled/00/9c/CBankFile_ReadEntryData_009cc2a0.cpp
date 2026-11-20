// CBankFile::ReadEntryData(unsigned long, unsigned long, void*, unsigned long) -- seek the bank's disk file to the entry (+offset) and read `size` bytes
// Leaf: ?ReadEntryData@CBankFile@@QAEXKKPAXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC2A0, 95 bytes, thiscall, `ret 10h`. Ego_r.pdb 0x00B19FA0 / FableWin.pdb 0x02F92080
//   ?ReadEntryData@CBankFile@@QAEXKKPAXK@Z (public; Ego_d parameter order: entry index, byte offset, buffer, size).
// Callees: CPackedUIntArray::operator[] @ 0x00A629F0 (x2, through the inlined GetEntryDataSize / GetEntryDataOffset), CDiskFileWin32
//   virtual slots 5 (SetPosition) and 3 (Read) on BankFile_Data (+0x7C).
// The entry size is computed first and never used (its packed-array call survives as an opaque call while the unpacked record read is
// dropped: an assertion on offset+size compiled out of the retail build), then the entry offset (both getters inlined: retail's TU
// held their bodies, out-of-line copies 0x009CC240 / 0x009CC270), then the disk file is positioned at entryOffset + offset and read
// into the buffer (buffer, size, 0).
// NF139 (2026-11-20, q6308): moved off the CBankFile_Retail stand-in and its local CRuntimeData / CPackedUIntArray / CDiskFileCalls
//   models onto engine/CBankFile.h (the define set of the landed 0x009CC240 GetEntryDataSize row, with the NF139 ENTRYDATA_INLINE getters
//   in place of its ENTRYDATASIZE declaration, and READENTRYDATA),
//   engine/CPackedUIntArray.h and engine/CDiskFileWin32.h; the 5 landed callers already bind this spelling.
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
#define FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE
#define FABLE_RETAIL_CBANKFILE_READENTRYDATA
#include "engine/CBankFile.h"
#define FABLE_RETAIL_CDISKFILEWIN32
#include "engine/CDiskFileWin32.h"

void CBankFile::ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size)
{
    unsigned long dataSize = GetEntryDataSize(index);
    unsigned long dataOffset = GetEntryDataOffset(index);
    BankFile_Data->SetPosition(dataOffset + offset);
    BankFile_Data->Read(buffer, size, 0);
}

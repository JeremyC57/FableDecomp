// CCountedPointer<CChunkedFileChunk>::DeleteData(void* data) -- the counted pointer's per-type deleter: `delete` the chunk through its virtual destructor
// Leaf: ?DeleteData@?$CCountedPointer@VCChunkedFileChunk@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A99FF0, 11 bytes, static __fastcall (data in ecx), `ret`: null test, then the scalar deleting
//   destructor (vtable slot 0) with flag 1.
// Identity (proven): stored as the count block's deleter by every CCountedPointer<CChunkedFileChunk> constructor in the
//   C3DAnimFileLoader TU (AddRootSubChunkType 0x00A98BE0 / 0x00A98C80 / 0x00A98D20 / 0x00A98DC0 and Reset 0x00A9CF40 push it as the
//   CCPPointerInfo DeleteFunc); catalog CCountedPointer<class CChunkedFileChunk>::DeleteData. Ego_r.pdb keeps DeleteData out of line
//   (engine/CCountedPointer.h). Name fix (NF189, 2026-11-20, q4553): the row was the free stand-in DeleteData(AutoTinyVirtualObject*).
// Shape: engine/CCountedPointer.h `static void DeleteData(void* data) { delete static_cast<T*>(data); }` over the shared
//   engine/CChunkedFileChunk.h (virtual destructor); only this member is instantiated.
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#include "engine/CChunkedFileChunk.h"
#include "engine/CCountedPointer.h"

template void CCountedPointer<CChunkedFileChunk>::DeleteData(void* data);

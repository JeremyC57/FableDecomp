// CCountedPointer<C3DMeshFileMapChunk>::DeleteData(void* data) -- the count block's deleter: `delete static_cast<C3DMeshFileMapChunk*>(data)`
//   through the virtual scalar deleting destructor (slot 0, flags 1)
// Leaf: ?DeleteData@?$CCountedPointer@VC3DMeshFileMapChunk@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC1460, 11 bytes: test ecx,ecx ; je ; mov eax,[ecx] ; push 1 ; call [eax] ; ret
// Name fix (NF202, 2026-11-20, q12247): FableWin.pdb 0x0347E160 ?DeleteData@?$CCountedPointer@VC3DMeshFileMapChunk@@@@SIXPAX@Z + the body
//   (byte-identical to the engine/CCountedPointer.h DeleteData<C3DMeshFileMapChunk> COMDAT, NF192) + its store into the count block by the
//   landed CreateMap 0x00AC0C00 (which binds this spelling). The free ?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z stand-in (and its local
//   AutoTinyVirtualObject model) is replaced by the shared template, explicit instantiation as the landed 0x00A99FF0
//   (DeleteData<CChunkedFileChunk>); the class comes from engine/C3DMeshFileMapChunk.h (polymorphic retail branch, as 0x00AC0C00).
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#include "fable_string.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/C3DMeshFileMapChunk.h"
#include "engine/CCountedPointer.h"

template void CCountedPointer<C3DMeshFileMapChunk>::DeleteData(void* data);

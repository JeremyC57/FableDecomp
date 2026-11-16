// C3DMeshFileXTextureIndexChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- write the chunk's texture Index
// Leaf: ?WriteToFile@C3DMeshFileXTextureIndexChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA4AF0, 16 bytes, `ret 8`: `mov eax,[ecx+10h]` (Index), `mov ecx,[esp+4]` (the stream),
// `push eax`, `call CDataOutputStream::WriteULONG` (0x00993EA0).
// Identity (NF42, round 5): Ego_r.pdb 0x00C3F9F0 ?WriteToFile@C3DMeshFileXTextureIndexChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z
// (vtable 0x0129E1F8); was an old placeholder (`S::WriteToFile` calling `Arg::N`). Now on the shared headers
// engine/C3DMeshFileXTextureIndexChunk.h (FABLE_RETAIL_ branch, Index +0x10) and engine/CDataOutputStream.h.
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_C3DMESHFILEXTEXTUREINDEXCHUNK
#include "engine/C3DMeshFileXTextureIndexChunk.h"
#include "engine/CDataOutputStream.h"

void C3DMeshFileXTextureIndexChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const
{
    stream.WriteULONG(Index);
}

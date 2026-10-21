// CMemoryDataInputStream::OpenMemorySource(const void* source, unsigned long size) -- points the stream at an in-memory buffer as one chunk
// Leaf: OpenMemorySource@CMemoryDataInputStream_Methods
// Retail: Fable.exe @ 0x00994570, 31 bytes, `ret 8`; Ego_r.pdb ?OpenMemorySource@CMemoryDataInputStream@@QAEXPBXK@Z. Resets the
// stream position (+0x04) and chunk stream position (+0x10) to 0, sets the current source pointer (+0x0c) and StreamSource
// (+0x18) to the buffer, and the stream size (+0x08) and chunk bytes remaining (+0x14) to size. Ego_r.pdb CDataInputStream
// names these StreamPos, StreamSize, CurrentSourcePtr, SourceChunkStreamPos, SourceChunkBytesRemaining.
// Layout: engine/CMemoryDataInputStream.h (its retail-only long names for the CDataInputStream base members).
#include "engine/CMemoryDataInputStream.h"
struct CMemoryDataInputStream_Methods : CMemoryDataInputStream {
    void OpenMemorySource(const void* source, unsigned long size);
};
void CMemoryDataInputStream_Methods::OpenMemorySource(const void* source, unsigned long size)
{
    position = 0;
    chunkPosition = 0;
    chunkBase = (long)source;
    sourceLength = size;
    chunkLength = size;
    StreamSource = (void*)source;
}
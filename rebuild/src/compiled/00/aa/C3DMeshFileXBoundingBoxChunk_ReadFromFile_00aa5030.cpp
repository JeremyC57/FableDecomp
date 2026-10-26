// C3DMeshFileXBoundingBoxChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the bounding box centre and diagonal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA5030, length 544 bytes (virtual, chunk reader slot).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined stream Read's slow path).
// Shape: six inlined 4-byte stream Reads (64-bit position range check, chunk fast path, overflow reader)
// through by-value ReadFloat() helpers; each helper local lives in the dead stream-argument slot and is
// copied to the member as a dword. The last member store is duplicated into both exits of the sixth Read.
// Same shape as the landed sibling C3DMeshFileXBoundingSphereChunk::ReadFromFile @ 0x00AA4E40.
// Layout: Center +0x10/+0x14/+0x18, Diagonal +0x1C/+0x20/+0x24 behind the 0x10 CChunkedFileChunk base
// (PDB header engine/C3DMeshFileXBoundingBoxChunk.h); stream fields from engine/CDataInputStream.h.
// Leaf: ReadFromFile@C3DMeshFileXBoundingBoxChunkMethods
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileXBoundingBoxChunk.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CChunkedFileLoader;
struct CInputStreamAccess : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};

struct C3DMeshFileXBoundingBoxChunkMethods : C3DMeshFileXBoundingBoxChunk {
    void ReadFromFile(CDataInputStream& input, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileXBoundingBoxChunkMethods::ReadFromFile(CDataInputStream& input, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    CInputStreamAccess& in = (CInputStreamAccess&)input;
    Center_X = in.ReadFloat();
    Center_Y = in.ReadFloat();
    Center_Z = in.ReadFloat();
    Diagonal_X = in.ReadFloat();
    Diagonal_Y = in.ReadFloat();
    Diagonal_Z = in.ReadFloat();
}
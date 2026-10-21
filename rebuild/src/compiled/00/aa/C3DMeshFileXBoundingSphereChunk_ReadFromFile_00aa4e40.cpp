// C3DMeshFileXBoundingSphereChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the bounding sphere centre and radius
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4E40, length 368 bytes (virtual, chunk reader slot).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined stream Read's slow path).
// Shape: four inlined 4-byte stream Reads (64-bit position range check, chunk fast path, overflow reader)
// through by-value ReadFloat() helpers; each helper local lives in the dead stream-argument slot and is
// copied to the member as a dword. The last member store is duplicated into both exits of the fourth Read.
// Layout: Center +0x10/+0x14/+0x18, Radius +0x1C behind the 0x10 CChunkedFileChunk base (PDB header
// engine/C3DMeshFileXBoundingSphereChunk.h); stream fields from engine/CDataInputStream.h.
// Leaf: ReadFromFile@C3DMeshFileXBoundingSphereChunkMethods
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileXBoundingSphereChunk.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
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

struct C3DMeshFileXBoundingSphereChunkMethods : C3DMeshFileXBoundingSphereChunk {
    void ReadFromFile(CDataInputStream& input, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileXBoundingSphereChunkMethods::ReadFromFile(CDataInputStream& input, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    CInputStreamAccess& in = (CInputStreamAccess&)input;
    Center_X = in.ReadFloat();
    Center_Y = in.ReadFloat();
    Center_Z = in.ReadFloat();
    Radius = in.ReadFloat();
}
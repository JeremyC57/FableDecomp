// CChunkedFileRawDataChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- replaces the chunk's byte buffer with a fresh one of the chunk size and reads it from the stream
// Leaf: ReadFromFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA77A0, 138 bytes of code (virtual, `ret 0Ch`).
// Callees: operator delete[] @ 0x00BFEB1C, operator new[] @ 0x00BFEB22 (one shared `add esp,8`),
//          CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0.
// Shape: the old buffer is freed and the new one allocated before either member store; the stream read is the
// inlined CMemoryDataInputStream::Read of the landed TransferBinaryIn family (0x007AD7F0): a positive-size guard,
// the 64-bit [0, 0x7FFFFFFF] position check through an inline predicate, then either the inline memcpy
// (`rep movsd ; rep movsb`) with the chunk pointer / remaining count / position updates, or the overflow read.
// `this` uses engine/CChunkedFileRawDataChunk.h (Data +0x10, ByteSize +0x14); the method (virtual in retail, the
// flattened header carries no vtable declarations) is declared on a methods-only derivation of it.
#include "engine/CChunkedFileRawDataChunk.h"
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CChunkedFileLoader;

class CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {            // `cmp ebx,[edx+14h]`: the size on the left
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct CChunkedFileRawDataChunk_Methods : CChunkedFileRawDataChunk {
    void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);
};

void CChunkedFileRawDataChunk_Methods::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    delete[] (unsigned char*)Data;
    Data = new unsigned char[size];
    ByteSize = size;
    stream.Read(Data, (long)size);
}
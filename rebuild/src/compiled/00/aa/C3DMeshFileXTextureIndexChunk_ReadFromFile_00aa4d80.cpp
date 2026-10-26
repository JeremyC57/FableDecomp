// C3DMeshFileXTextureIndexChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- reads the 4-byte texture index through the inlined memory-stream Read
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4D80, length 133 bytes.
// Leaf: ReadFromFile
// Callees: 0x00993CA0 CDataInputStream::ReadWithSrcChunkOverflow (when the current chunk runs out).
// Shape: the landed CPersistTraits TransferBinaryIn model (0x007AD7F0): a local `index` (homed in the dead
// stream-argument slot) is filled by the inlined Read(buffer, size) -- the position widened to __int64 and
// range-checked through an inline bool predicate (its exits keep the duplicated `Index = index` tails, which
// store the uninitialised local exactly like retail), a direct dword copy when 4 bytes remain in the chunk,
// else the out-of-line overflow read -- and the value is stored to Index (+0x10).
// The PDB header (rebuild/include/engine/C3DMeshFileXTextureIndexChunk.h) agrees: Index at +0x10.
#include "engine/C3DMeshFileXTextureIndexChunk.h"
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
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct C3DMeshFileXTextureIndexChunk_Methods : C3DMeshFileXTextureIndexChunk {
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileXTextureIndexChunk_Methods::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long index;
    in.Read(&index, sizeof(index));
    Index = index;
}
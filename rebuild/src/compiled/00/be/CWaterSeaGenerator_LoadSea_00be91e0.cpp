// CWaterSeaGenerator::LoadSea(CMemoryDataInputStream* stream, CVertexBufferWin32* vertexBuffer, long* vertexCount, CIndexBuffer* indexBuffer, long* polyCount) -- load the background sea mesh: drop both buffers, read the vertex and index counts, then create each buffer and range-decompress its compressed block straight into the locked memory (12-byte vertices, 16-bit indices)
// Leaf: LoadSea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE91E0, length 655 bytes, `ret 0x14` (this unused).
// Callees: CVertexBufferWin32::Uninitialise 0x00A63200 / Initialise 0x00A63150 / Lock 0x00AC20A0 / Unlock 0x00AC20B0;
//   CIndexBuffer::Uninitialise 0x00AC1EE0 / Initialise 0x00AC1F70 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00; resource manager
//   @ 0x01436E98: GetVertexBufferUsage 0x00B3B030, GetIndexBufferUsage 0x00B3B060, the pool getter 0x00B3B0A0 (returns 1);
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, SetPositionAssumeWithinSourceChunk 0x00993C10,
//   CRangeCompressor::Decompress 0x00F39ED0. Caller: the landed CEngineWaterRenderer::LoadSea 0x00B6D420.
// Layout: the landed CEngineWaterRenderer::LoadCompressedBuffer 0x00B6D940 stream model (PDB CDataInputStream offsets,
//   virtual GetPosition slot 2), reused verbatim; the generator itself holds no state used here.
// Shape: that function twice over (vertex block, then index block) with one function-scope range compressor; the counts
//   are written through the out-pointers and re-read from them.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream { public:
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    const unsigned char* StreamSource;                  // +0x18
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetPosition();                // slot 2
    void SetPositionAssumeWithinSourceChunk(unsigned long position);  // 0x00993C10
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
    {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else {
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)dst, size);
        }
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, 4);
        return value;
    }
    __forceinline const unsigned char* GetDataAndSkip(long size)
    {
        unsigned long position = GetPosition();
        __int64 np = (__int64)(position + size);
        const unsigned char* data;
        if (IsValidPosition(np)) {
            data = StreamSource + position;
            SetPositionAssumeWithinSourceChunk(position + size);
        } else {
            data = 0;
        }
        return data;
    }
};

class CVertexBufferWin32 { public:
    void Initialise(long vertexType, long count, long stride, unsigned long usage, long pool);   // 0x00A63150
    void Uninitialise();                                                                        // 0x00A63200
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);         // 0x00AC20A0
    void Unlock();                                                                              // 0x00AC20B0
};

class CIndexBuffer { public:
    void Initialise(unsigned long count, unsigned long flags, long format, long pool, long primitiveType);   // 0x00AC1F70
    void Uninitialise();                                                                                     // 0x00AC1EE0
    unsigned char* Lock(unsigned long offset, unsigned long size, unsigned long flags);                      // 0x00AC1FB0
    void Unlock();                                                                                           // 0x00AC1F00
};

class CEngineResourceManager { public:
    unsigned long GetVertexBufferUsage();          // 0x00B3B030
    unsigned long GetIndexBufferUsage();           // 0x00B3B060
    long GetIndexBufferPool();                     // 0x00B3B0A0 (returns 1)
};
extern CEngineResourceManager* g_pEngineResourceManager_01436e98;   // 0x01436E98

struct CRangeCompressor {
    void Decompress(const unsigned char* source, long count, long stride, void* destination);   // 0x00F39ED0
};

class CWaterSeaGenerator { public:
    long m_state;
    void LoadSea(CMemoryDataInputStream* stream, CVertexBufferWin32* vertexBuffer, long* vertexCount,
                 CIndexBuffer* indexBuffer, long* polyCount);
};

void CWaterSeaGenerator::LoadSea(CMemoryDataInputStream* stream, CVertexBufferWin32* vertexBuffer, long* vertexCount,
                                 CIndexBuffer* indexBuffer, long* polyCount)
{
    CRangeCompressor compressor;
    vertexBuffer->Uninitialise();
    indexBuffer->Uninitialise();
    *vertexCount = stream->ReadLong();
    *polyCount = stream->ReadLong();
    if (*vertexCount) {
        vertexBuffer->Initialise(0, *vertexCount, 0xc, g_pEngineResourceManager_01436e98->GetVertexBufferUsage(), 1);
        unsigned char* data = vertexBuffer->Lock(0, 0, 0);
        long compressedSize = stream->ReadLong();
        if (compressedSize > 0)
            compressor.Decompress(stream->GetDataAndSkip(compressedSize), *vertexCount, 0xc, data);
        vertexBuffer->Unlock();
    }
    if (*polyCount) {
        indexBuffer->Initialise(*polyCount * 2, g_pEngineResourceManager_01436e98->GetIndexBufferUsage(), 0x65,
                                g_pEngineResourceManager_01436e98->GetIndexBufferPool(), 5);
        unsigned char* data = indexBuffer->Lock(0, 0, 0);
        long compressedSize = stream->ReadLong();
        if (compressedSize > 0)
            compressor.Decompress(stream->GetDataAndSkip(compressedSize), *polyCount, 2, data);
        indexBuffer->Unlock();
    }
}
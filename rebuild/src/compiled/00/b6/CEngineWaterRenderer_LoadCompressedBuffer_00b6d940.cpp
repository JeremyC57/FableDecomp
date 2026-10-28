// CEngineWaterRenderer::LoadCompressedBuffer(CMemoryDataInputStream&, CVertexBufferWin32&, long*) -- read a vertex count and stride, create the vertex buffer, then range-decompress the following compressed block straight into the locked buffer; report the vertex count
// Leaf: LoadCompressedBuffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D940, length 432 bytes, `ret 0xc` (this unused).
// Callees: CVertexBufferWin32::Uninitialise 0x00A63200, Initialise(long,long,long,unsigned long,long) 0x00A63150, the
//   out-of-line Lock(3 args)/Unlock thunks 0x00AC20A0/0x00AC20B0 (COMDAT-folded), the buffer-usage query 0x00B3B030 on the
//   resource manager @ 0x01436E98 (a member that ignores `this`), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   SetPositionAssumeWithinSourceChunk 0x00993C10, CRangeCompressor::Decompress(const uchar*, long, long, void*) 0x00F39ED0.
// Layout: PDB CDataInputStream (StreamPos +4, CurrentSourcePtr +0xc, SourceChunkBytesRemaining +0x14) and
//   CMemoryDataInputStream::StreamSource +0x18; the stream's position getter is virtual slot 2. Local model: the engine
//   headers are data-only and cannot carry the inlined Read or the virtual call (as in the landed 0x00B16F60).
// Shape: each value comes from a value-returning ReadLong (count spills to the dead stream-argument slot); the range
//   compressor is a function-scope local declared first (its own frame slot); the skip helper assigns 0 in an else.
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

class CEngineResourceManager { public:
    unsigned long GetVertexBufferUsage();          // 0x00B3B030 (folded with GetIndexBufferUsage)
};
extern CEngineResourceManager* g_pEngineResourceManager_01436e98;   // 0x01436E98

struct CRangeCompressor {
    void Decompress(const unsigned char* source, long count, long stride, void* destination);   // 0x00F39ED0
};

class CEngineWaterRenderer { public:
    void LoadCompressedBuffer(CMemoryDataInputStream& stream, CVertexBufferWin32& buffer, long* vertexCount);
};

void CEngineWaterRenderer::LoadCompressedBuffer(CMemoryDataInputStream& stream, CVertexBufferWin32& buffer, long* vertexCount)
{
    CRangeCompressor compressor;
    buffer.Uninitialise();
    long count = stream.ReadLong();
    long stride = stream.ReadLong();
    if (count && stride) {
        buffer.Initialise(0, count, stride, g_pEngineResourceManager_01436e98->GetVertexBufferUsage(), 1);
        unsigned char* data = buffer.Lock(0, 0, 0);
        long compressedSize = stream.ReadLong();
        if (compressedSize > 0) {
            compressor.Decompress(stream.GetDataAndSkip(compressedSize), count, stride, data);
        }
        buffer.Unlock();
    }
    if (vertexCount)
        *vertexCount = count;
}
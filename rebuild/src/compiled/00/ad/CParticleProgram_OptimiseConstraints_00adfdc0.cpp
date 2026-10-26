// CParticleProgram::OptimiseConstraints() -- merge runs of same-typed constraint records in the program's constraint stream: each record is (type, count, data size, data); consecutive records of one type other than 0 and 1 are combined into a single record whose count and size are the sums and whose data is the concatenation; the merged stream then replaces the original
// Leaf: OptimiseConstraints
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADFDC0, 647 bytes (+ int3 padding), plain `ret`.
// Callees: CMemoryDataInputStream(const CMemoryDataOutputStream&) 0x00995EE0 / ~ 0x00993B10 (label "~CChunkedFileChunk"),
//   CMemoryDataOutputStream() 0x00996AF0 / ~ 0x00996B30 (label "~CCompressedMemoryDataInputStream") / Reset 0x00996B60 /
//   Write 0x00996CC0 (the slot-4 virtual, called directly on the locals and through the vtable on the member) / GetSize
//   0x00994B40 / PeekData 0x00995E70, CDataOutputStream::WriteULong 0x00993EA0, CDataInputStream::ReadWithSrcChunkOverflow
//   0x00993CA0 (slow path of the inline Read), CDataInputStream::ReadToStream(CDataOutputStream&, long) 0x009964E0.
// Layout: Constraints (CMemoryDataOutputStream, 0x18 bytes) at +0x114 as in the landed CParticleProgram::WriteToFile
//   0x00AE2230 model; CDataInputStream is the landed 0x00AF4660 model (inline Read with the 64-bit position range check,
//   StreamPos +0x04, StreamSize +0x08, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
class CDataOutputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);                 // slot 4 (+0x10)
    void WriteULong(unsigned long value);                              // 0x00993EA0
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    unsigned char _body04[0x14];
    CMemoryDataOutputStream();                                         // 0x00996AF0
    virtual ~CMemoryDataOutputStream();                                        // 0x00996B30
    virtual void Write(const void* data, long length);                 // 0x00996CC0
    void Reset();                                                      // 0x00996B60
    long GetSize();                                                    // 0x00994B40
    void* PeekData();                                                  // 0x00995E70
};
class CDataInputStream { public:
    virtual void v0();
    unsigned long StreamPos;                                           // +0x04
    long StreamSize;                                                   // +0x08
    const unsigned char* CurrentSourcePtr;                             // +0x0c
    unsigned long SourceChunkStreamPos;                                // +0x10
    long SourceChunkBytesRemaining;                                    // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    void ReadToStream(CDataOutputStream& out, long size);              // 0x009964E0
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
        } else
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, 4); return value; }
};
class CMemoryDataInputStream : public CDataInputStream { public:
    unsigned long m_source;                                            // +0x18
    CMemoryDataInputStream(const CMemoryDataOutputStream& source);     // 0x00995EE0
    virtual ~CMemoryDataInputStream();                                         // 0x00993B10
};

class CParticleProgram {
public:
    unsigned char           _body00[0x114];
    CMemoryDataOutputStream Constraints;                               // +0x114
    void OptimiseConstraints();
};

void CParticleProgram::OptimiseConstraints()
{
    CMemoryDataOutputStream& constraints = Constraints;
    CMemoryDataInputStream in(constraints);
    CMemoryDataOutputStream merged;
    long lastType = -1;
    long totalCount = 0;
    long totalSize = 0;
    CMemoryDataOutputStream data;
    while (in.StreamPos != in.StreamSize) {
        long type = in.ReadULong();
        long count = in.ReadULong();
        long size = in.ReadULong();
        if (type == lastType && type != 0 && type != 1) {
            totalCount += count;
            totalSize += size;
        } else {
            if (totalCount != 0) {
                merged.WriteULong(lastType);
                merged.WriteULong(totalCount);
                merged.WriteULong(totalSize);
                merged.Write(data.PeekData(), data.GetSize());
            }
            lastType = type;
            totalCount = count;
            totalSize = size;
            data.Reset();
        }
        in.ReadToStream(data, size);
    }
    if (totalCount != 0) {
        merged.WriteULong(lastType);
        merged.WriteULong(totalCount);
        merged.WriteULong(totalSize);
        merged.Write(data.PeekData(), data.GetSize());
    }
    constraints.Reset();
    constraints.Write(merged.PeekData(), merged.GetSize());
}
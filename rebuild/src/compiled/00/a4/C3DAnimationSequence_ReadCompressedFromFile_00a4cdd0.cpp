// C3DAnimationSequence::ReadCompressedFromFile(CDataInputStream&, unsigned long, C3DAnimationSequenceData*, CSimpleMemoryPool*) -- read the header, attach the shared data and let it read itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4CDD0, length 198 bytes (`ret 10h`).
// Callees: CDataInputStream::GetPosition (slot 2; result unused), ReadWithSrcChunkOverflow @ 0x00993CA0,
// C3DAnimationSequenceData::ReadCompressedFromFile(CDataInputStream&, CSimpleMemoryPool*) @ 0x00A4C5E0.
// Shape: the start position is fetched but never used (the virtual call stays); two inlined 4-byte reads
// through by-value helpers whose locals share the dead stream-argument slot: the version (unused, its
// fast-path copy dropped) and the parent index (a long truncated to the short member); then Data is set and
// the data's own reader is called with the stream and the pool.
// Layout: PDB C3DAnimationSequence (8 bytes): Data +0, ParentIndex (short) +4, Shared (bool) +6.
// Leaf: ReadCompressedFromFile@C3DAnimationSequence
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CChunkedFileLoader;
// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14
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
    // The typed readers return by value: their locals live in dead argument slots, and an unused result
    // (a skipped version word) loses its fast-path copy entirely.
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

class CSimpleMemoryPool;
class C3DAnimationSequenceData { public:
    void ReadCompressedFromFile(CDataInputStream& in, CSimpleMemoryPool* pool);   // 0x00A4C5E0
};

class C3DAnimationSequence { public:
    C3DAnimationSequenceData* Data;        // +0
    short ParentIndex;                     // +4
    bool  Shared;                          // +6
    void ReadCompressedFromFile(CDataInputStream& in, unsigned long chunkSize, C3DAnimationSequenceData* data, CSimpleMemoryPool* pool);
};

void C3DAnimationSequence::ReadCompressedFromFile(CDataInputStream& in, unsigned long chunkSize, C3DAnimationSequenceData* data, CSimpleMemoryPool* pool)
{
    unsigned long start = in.GetPosition();
    in.ReadULong();                        // version (unused)
    ParentIndex = (short)in.ReadLong();
    Data = data;
    data->ReadCompressedFromFile(in, pool);
}
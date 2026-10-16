// CSimVoice::TransferBinaryIn(CMemoryDataInputStream*) -- read the voice count, resize Voices and read the (type, voice) pairs in one block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00680950, length 240 bytes (`ret 4`).
// Catalog label "C3DMeshFileXTriangleStripChunk::ReadFromFile" is a propagated byte twin: the only caller is the
// CSimVoice transfer helper 0x00464B9A (load mode, `push [ctx+0x24]` stream, ecx = the CSimVoice at
// CSimVoicesDef+0x3c, reached from CSimVoicesDef::Transfer 0x00464B8A); PDB CSimVoice has Voices
// (CVectorMap<EReactionSpeechType,long>) at +8.
// Callees: CMemoryDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, the pair vector's resize(n) @ 0x00681340
// (catalog "vector<std::pair<CCharString...>>::resize", one argument).
// Shape: an inlined 4-byte ReadULong() for the count (local in the dead argument slot), `resize(count)`, then the
// inlined block Read of the landed 0x00AA48D0 into GetData() (array address when empty). GetData and the byte
// count each recompute size() here (8-byte elements: no common `sar` to share).
// Leaf: TransferBinaryIn@CSimVoice
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
// CMemoryDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
struct CMemoryDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
    long m_remaining;                      // +0x14
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
    __forceinline void ReadBlock(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (size <= m_remaining) {
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
};

struct CVoiceEntry {                      // pair<EReactionSpeechType, long>
    long first; long second;
};
template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
    void resize(unsigned long n);                        // 0x00681340
};
template<class T> inline T* GetData(CArray<T>& a)
{
    if (a.size() > 0)
        return a.m_begin;
    return reinterpret_cast<T*>(&a);
}

struct CSimVoice {
    unsigned char _00[8];                  // +0x00
    CArray<CVoiceEntry> Voices;            // +0x08 CVectorMap<EReactionSpeechType,long> storage
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CSimVoice::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    unsigned long count = stream->ReadULong();
    Voices.resize(count);
    stream->ReadBlock(GetData(Voices), Voices.size() * sizeof(CVoiceEntry));
}

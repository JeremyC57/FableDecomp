// CExpressionSet::TransferBinaryIn(CMemoryDataInputStream& stream) -- read a CVectorMap of 12-byte pairs: count, resize, raw block, re-sort if dirty, end marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086F680, length 369 bytes (`ret 4`).
// The manifest label is propagated across the CVectorMap<K,V>::TransferBinaryIn template instances; the
// object is a CVectorMap: a three-pointer vector of 12-byte pairs (+0/+4/+8), the empty comparator (+0xC)
// and the Dirty bool (+0xD) (GOTCHAS wave 25 chunk 159).
// Callees: CMemoryDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x3), the pair vector's
//          resize(n, const T&) @ 0x00870040, std::sort(first, last, comp) @ 0x008701D0 (`__fastcall`,
//          the empty comparator pushed as a dword).
// Shape: inlined ReadULong() for the count (value homed in the dead stream-argument slot), `resize(count, T())`
// with the value-initialised pair as a 3-dword frame temporary, the inlined block read into GetData()
// (the vector's own address when empty), the re-sort of a dirty map, then a trailing marker read whose
// unused result drops the fast-path copy (as in the landed COpinionSet 0x007020D0 / CSimVoice 0x00680950).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

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
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
};

struct SExpressionValue {                  // 8-byte mapped value
    long m_words[2];
};
struct SExpressionPair {                   // 12-byte pair<key, value>
    long first;
    SExpressionValue second;
    SExpressionPair() : first(long()), second(SExpressionValue()) {}
};
struct SExpressionCompare {};              // empty comparator

void __fastcall SortExpressionPairs(SExpressionPair* first, SExpressionPair* last, SExpressionCompare comp);  // 0x008701D0

struct CExpressionPairVector {
    SExpressionPair* m_begin; SExpressionPair* m_end; SExpressionPair* m_capacity;
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
    void resize(unsigned long n, const SExpressionPair& value);         // 0x00870040
};
inline SExpressionPair* GetData(CExpressionPairVector& a)
{
    if (a.size() > 0)
        return a.m_begin;
    return reinterpret_cast<SExpressionPair*>(&a);
}

struct CExpressionSet {
    CExpressionPairVector Pairs;           // +0x00
    SExpressionCompare Compare;            // +0x0C
    bool Dirty;                            // +0x0D
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CExpressionSet::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    unsigned long count = stream.ReadULong();
    Pairs.resize(count, SExpressionPair());
    stream.ReadBlock(GetData(Pairs), Pairs.size() * sizeof(SExpressionPair));
    if (Dirty) {
        SortExpressionPairs(Pairs.m_begin, Pairs.m_end, Compare);
        Dirty = false;
    }
    stream.ReadULong();
}
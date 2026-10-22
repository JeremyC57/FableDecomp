// COpinionSet::TransferBinaryIn(CMemoryDataInputStream&) -- read the opinion count, size OpinionValues (zero fill) and read the floats in one block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007020D0, length 238 bytes (`ret 4`).
// Catalog label "C3DMeshFileXTriangleStripChunk::ReadFromFile" is a propagated byte twin: the only caller is the
// landed CReactionMatch_Sphere::TransferBinaryIn (0x007019A2 calls this for its Centre COpinionSet at +0x08).
// Callees: CMemoryDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, CArray<float>::resize(n, const float&)
// @ 0x00442770 (catalog "vector<unsigned int>::resize").
// Shape: an inlined 4-byte ReadULong() for the count (local in the dead argument slot), `resize(count, 0.0f)`
// with the zero homed in the `push ecx` frame word, then the inlined block Read of the landed 0x00AA48D0
// (non-positive size skipped, `size <= remaining` polarity, inline rep movsd/movsb copy) into GetData(),
// which hands back the array's own address when it is empty.
// Layout: retail CArray<T> is three pointers (begin/end/capacity); OpinionValues at +0.
// Leaf: TransferBinaryIn@COpinionSet
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
// CMemoryDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
struct CMemoryDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;
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

template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
    void resize(unsigned long n, const T& value);       // 0x00442770
};
template<class T> inline T* GetData(CArray<T>& a)
{
    if (a.size() > 0)
        return a.m_begin;
    return reinterpret_cast<T*>(&a);
}

struct COpinionSet {
    CArray<float> OpinionValues;           // +0x00
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void COpinionSet::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    unsigned long count = stream.ReadULong();
    OpinionValues.resize(count, 0.0f);
    stream.ReadBlock(GetData(OpinionValues), OpinionValues.size() * sizeof(float));
}
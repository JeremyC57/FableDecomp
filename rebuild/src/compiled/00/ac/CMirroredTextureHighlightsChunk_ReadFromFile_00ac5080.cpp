// CMirroredTextureHighlightsChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the index count, size TriangleVertexIndexList, then one unsigned long per index
// Leaf: ReadFromFile@CMirroredTextureHighlightsChunk
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC5080, 233 bytes (`ret 0Ch`, virtual).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0; CArray<unsigned long>::resize(n, const unsigned long&)
// 0x00464931 (manifest label "push_back" is wrong; landed family shape).
// Layout: engine/CMirroredTextureHighlightsChunk.h puts TriangleVertexIndexList (CArray<unsigned long>) at +0x10 behind the
// 0x10 CChunkedFileChunk base, but flattens the vptr into a member and gives the CArray the PDB's 0x10 bytes; retail CArray
// is begin/end/capacity (12 bytes), so this local model keeps the header's offset with the retail CArray.
// Shape: the twin of the landed C3DAnimFileBoneMaskChunk::ReadFromFile 0x00AB0790 with 4-byte elements: an inlined by-value
// ReadULong() for the count (ebx), `resize(count, 0)` with the zero temporary in the dead stream-argument slot, then a loop
// of inlined ReadULong() reads stored through `*(begin() + i)`; the array address is kept in the frame.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
struct CDataInputStream {
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
    // The typed readers return by value: their locals live in dead argument slots, and an unused result
    // (a skipped version word) loses its fast-path copy entirely.
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
};

template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    T* begin() { return m_begin; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n, const T& value);       // 0x00464931 for T = unsigned long
};

struct CMirroredTextureHighlightsChunk {
    virtual void v0();
    unsigned char _base_body[0x0c];        // CChunkedFileChunk: ChunkType[5] + pad + Parent
    CArray<unsigned long> TriangleVertexIndexList;      // +0x10
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void CMirroredTextureHighlightsChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long count = in.ReadULong();
    TriangleVertexIndexList.resize(count, 0);
    for (unsigned long i = 0; i < count; ++i)
        TriangleVertexIndexList[i] = in.ReadULong();
}
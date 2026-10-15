// CMirroredTextureHighlightsChunk::WriteToFile(CDataOutputStream& stream) -- write the vertex index count then each index of TriangleVertexIndexList (begin +0x10, end +0x14) as a 4-byte word
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC4F50, length 59 bytes.
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0 (labelled WriteCBYTE, propagated; thiscall with one dword, `ret 4`). Vtable slot 8 of CMirroredTextureHighlightsChunk. Shape: the count in esi (signed pointer difference), the stream in ebp, an unsigned counted loop re-reading the begin pointer each pass, `ret 8` (a second, unused argument).
struct CDataOutputStream {
    void WriteULong(unsigned long value);          // 0x00993EA0
};

struct CMirroredTextureHighlightsChunk {
    virtual void v00();
    long           m_4;
    long           m_8;
    long           m_c;
    unsigned long* m_pIndexBegin;                  // +0x10 TriangleVertexIndexList (CArray<unsigned long>): begin
    unsigned long* m_pIndexEnd;                    // +0x14 end
    unsigned long* m_pIndexCapacity;               // +0x18
    long           m_1c;                           // +0x1c
    virtual void WriteToFile(CDataOutputStream& stream, long flags);
};

void CMirroredTextureHighlightsChunk::WriteToFile(CDataOutputStream& stream, long flags)
{
    unsigned long count = m_pIndexEnd - m_pIndexBegin;
    stream.WriteULong(count);
    for (unsigned long i = 0; i < count; ++i)
        stream.WriteULong(m_pIndexBegin[i]);
}
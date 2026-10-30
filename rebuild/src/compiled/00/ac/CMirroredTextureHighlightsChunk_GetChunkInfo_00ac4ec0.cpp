// Flags: /GS
// CMirroredTextureHighlightsChunk::GetChunkInfo() -- a CCharString "Number of verticies: %u" built with sprintf over the TriangleVertexIndexList count (end +0x14 - begin +0x10)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC4EC0, length 96 bytes.
// Callees: sprintf @ 0x00BFEB10 (the format literal @ 0x0129EF9C), CCharString::CCharString(const char*, long) @ 0x0099EBF0 on the hidden result, __security_check_cookie @ 0x00BFE9F9 (cookie @ 0x0139C8A8). Vtable slot 2. Shape: a 128-byte text buffer under a /GS cookie (frame 0x84), the hidden result pointer read into esi after the sprintf, `ret 4`.
// Grade: DIFFER(ours v retail) -- the lane copies `this` to eax, loads begin before end and reads the hidden result pointer into esi BEFORE the sprintf; retail subtracts begin from end in place and loads the hidden pointer after the call (explicit CCharString(buffer, -1), the implicit conversion, an inline GetNoItems accessor and optimize("s") tried, all under /GS).
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10

class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0 (the default argument: the buffer converts implicitly)
};

struct CArray_ulong {                              // CArray<unsigned long>: begin, end, capacity end, a spare word
    unsigned long* m_pBegin;
    unsigned long* m_pEnd;
    unsigned long* m_pCapacityEnd;
    long           m_c;
    unsigned long GetNoItems() const { return (unsigned long)(m_pEnd - m_pBegin); }
};

class CMirroredTextureHighlightsChunk { public:
    virtual void v00();
    long           m_4;
    long           m_8;
    long           m_c;
    CArray_ulong   TriangleVertexIndexList;        // +0x10
    virtual CCharString GetChunkInfo();
};

CCharString CMirroredTextureHighlightsChunk::GetChunkInfo()
{
    char buffer[128];
    sprintf(buffer, "Number of verticies: %u", TriangleVertexIndexList.GetNoItems());
    return buffer;
}
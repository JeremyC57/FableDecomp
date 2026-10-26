// Behaviour test for CMirroredTextureHighlightsChunk::GetChunkInfo @ 0x00AC4EC0: sprintf "Number of verticies: %u"
// (count = number of unsigned longs in TriangleVertexIndexList, end +0x14 minus begin +0x10) into a 128-byte buffer,
// returned through the implicit CCharString(buffer, -1) conversion. Checks several entries, none, and a large count.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

int __cdecl sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10

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

// ---- instrumented stubs ----
static int g_textCalls = 0;
static long g_lenArg = 0;
CCharString::CCharString(const char* text, long length)
{
    ++g_textCalls; g_lenArg = length;
    size_t n = length < 0 ? std::strlen(text) : (size_t)length;
    char* p = (char*)std::malloc(n + 1); std::memcpy(p, text, n); p[n] = 0; m_pStringData = p;
}
void CMirroredTextureHighlightsChunk::v00() { }
static bool is(const CCharString& s, const char* text) { return std::strcmp((const char*)s.m_pStringData, text) == 0; }

int main()
{
    static unsigned long idx[300000];
    CMirroredTextureHighlightsChunk chunk; chunk.m_4 = chunk.m_8 = chunk.m_c = 0;
    CHECK((char*)&chunk.TriangleVertexIndexList - (char*)&chunk == 0x10);
    chunk.TriangleVertexIndexList.m_pBegin = idx; chunk.TriangleVertexIndexList.m_pEnd = idx + 12;
    chunk.TriangleVertexIndexList.m_pCapacityEnd = idx + 16; chunk.TriangleVertexIndexList.m_c = 0;

    g_textCalls = 0;
    { CCharString r = chunk.GetChunkInfo(); CHECK(g_textCalls == 1 && g_lenArg == -1 && is(r, "Number of verticies: 12")); }

    g_textCalls = 0; chunk.TriangleVertexIndexList.m_pEnd = idx;
    { CCharString r = chunk.GetChunkInfo(); CHECK(g_textCalls == 1 && is(r, "Number of verticies: 0")); }

    g_textCalls = 0; chunk.TriangleVertexIndexList.m_pBegin = idx + 1; chunk.TriangleVertexIndexList.m_pEnd = idx + 250001;
    { CCharString r = chunk.GetChunkInfo(); CHECK(g_textCalls == 1 && is(r, "Number of verticies: 250000")); }

    if (g_fail) { std::printf("00ac4ec0_TEST FAIL\n"); return 1; }
    std::printf("00ac4ec0_TEST PASS\n");
    return 0;
}
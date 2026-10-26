// Behaviour test for C3DMeshFileGroupChunk::GetChunkInfo @ 0x00ABAE50: sprintf "Bone Index: %li, Vertex Count: %lu"
// (count = number of 8-byte C3DVertexBlend2 entries in VertexList) into a 1 KB buffer and return
// CCharString(buffer, -1). Checks a positive index with several vertices, a negative index with none, and a large
// vertex count (the element-size divide).
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10

class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
};

class C3DVertexBlend2 { public: unsigned short m_index; float m_weight; };   // 8 bytes
template <class T> class CArray { public:                  // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return m_end - m_begin; }
};

class C3DMeshFileGroupChunk { public:
    virtual ~C3DMeshFileGroupChunk();
    unsigned char _base_body[0x28];                 // +0x04 .. +0x2B (C3DMeshFileChunk base, retail size)
    long BoneIndex;                                 // +0x2C
    CArray<C3DVertexBlend2> VertexList;             // +0x30
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileGroupChunk::GetChunkInfo() const
{
    char buffer[1024];
    sprintf(buffer, "Bone Index: %li, Vertex Count: %lu", BoneIndex, VertexList.Size());
    return CCharString(buffer, -1);
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
C3DMeshFileGroupChunk::~C3DMeshFileGroupChunk() { }
static bool is(const CCharString& s, const char* text) { return std::strcmp((const char*)s.m_pStringData, text) == 0; }

int main()
{
    static C3DVertexBlend2 verts[100000];
    C3DMeshFileGroupChunk chunk; std::memset(chunk._base_body, 0, sizeof(chunk._base_body));
    chunk.BoneIndex = 4; chunk.VertexList.m_begin = verts; chunk.VertexList.m_end = verts + 5; chunk.VertexList.m_capacity = verts + 8;

    g_textCalls = 0;
    { CCharString r = chunk.GetChunkInfo(); CHECK(g_textCalls == 1 && g_lenArg == -1 && is(r, "Bone Index: 4, Vertex Count: 5")); }

    g_textCalls = 0; chunk.BoneIndex = -3; chunk.VertexList.m_end = verts;
    { CCharString r = chunk.GetChunkInfo(); CHECK(g_textCalls == 1 && is(r, "Bone Index: -3, Vertex Count: 0")); }

    g_textCalls = 0; chunk.BoneIndex = 0x7fffffff; chunk.VertexList.m_end = verts + 99999;
    { CCharString r = chunk.GetChunkInfo(); CHECK(g_textCalls == 1 && is(r, "Bone Index: 2147483647, Vertex Count: 99999")); }

    if (g_fail) { std::printf("00abae50_TEST FAIL\n"); return 1; }
    std::printf("00abae50_TEST PASS\n");
    return 0;
}
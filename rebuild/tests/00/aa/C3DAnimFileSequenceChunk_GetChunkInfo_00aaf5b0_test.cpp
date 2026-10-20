// Behaviour test for C3DAnimFileSequenceChunk::GetChunkInfo @ 0x00AAF5B0: formats "Bone Name: %s. Bone Index: %li,
// Frames: %lu, Framerate: %f" (name via the null-tested chars accessor, frames = number of 48-byte matrices, rate
// widened to double) into a 1 KB buffer with _snprintf(1023), forces the terminator at [1023], and returns
// CCharString(buffer, -1). Checks a named chunk, a nameless one (empty name), and a name long enough to truncate.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

extern "C" int __cdecl _snprintf(char* buffer, unsigned int count, const char* format, ...);   // 0x00BFEB96
extern const char g_emptyLiteral_0129aaf4[];        // 0x0129AAF4: ""

struct CCharStringData {
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
struct CCharString {
    CCharStringData* m_pStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};

struct CMatrix3x4 { float m_row[3][4]; };           // 48 bytes
template <class T> struct CArray {                  // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return m_end - m_begin; }
};

struct C3DAnimFileSequenceChunk {
    virtual ~C3DAnimFileSequenceChunk();
    unsigned char _base_body[0x28];                 // +0x04 .. +0x2B (CChunkedFileChunk base, retail size)
    long BoneIndex;                                 // +0x2C
    long ParentIndex;                               // +0x30
    CCharString BoneName;                           // +0x34
    float SamplesPerSecond;                         // +0x38
    unsigned char BoneType;                         // +0x3C EBoneType (written as a byte)
    CArray<CMatrix3x4> Matrices;                    // +0x40
    CCharString GetChunkInfo() const;
};

CCharString C3DAnimFileSequenceChunk::GetChunkInfo() const
{
    char buffer[1024];
    _snprintf(buffer, 1023, "Bone Name: %s. Bone Index: %li, Frames: %lu, Framerate: %f",
              BoneName.GetChars(), BoneIndex, Matrices.Size(), (double)SamplesPerSecond);
    buffer[1023] = 0;
    return CCharString(buffer, -1);
}

// ---- instrumented stubs ----
const char g_emptyLiteral_0129aaf4[] = "";
static int g_textCalls = 0;
static long g_lenArg = 0;
CCharString::CCharString(const char* text, long length)
{
    ++g_textCalls; g_lenArg = length;
    size_t n = length < 0 ? std::strlen(text) : (size_t)length;
    CCharStringData* d = (CCharStringData*)std::malloc(sizeof(CCharStringData));
    char* p = (char*)std::malloc(n + 1); std::memcpy(p, text, n); p[n] = 0;
    d->m_pChars = p; d->m_length = n; m_pStringData = d;
}
C3DAnimFileSequenceChunk::~C3DAnimFileSequenceChunk() { }

static void reset() { g_textCalls = 0; g_lenArg = 0; }

int main()
{
    CMatrix3x4 mats[3];
    // CCharString has no default constructor, so the chunk is laid over zeroed storage (GetChunkInfo is non-virtual).
    static union { double align; unsigned char raw[sizeof(C3DAnimFileSequenceChunk)]; } storage;
    std::memset(storage.raw, 0, sizeof(storage.raw));
    C3DAnimFileSequenceChunk& chunk = *reinterpret_cast<C3DAnimFileSequenceChunk*>(storage.raw);
    CCharStringData nameData = { "spine", 5 };
    chunk.BoneIndex = 7; chunk.ParentIndex = 2; chunk.BoneName.m_pStringData = &nameData;
    chunk.SamplesPerSecond = 30.0f; chunk.BoneType = 1;
    chunk.Matrices.m_begin = mats; chunk.Matrices.m_end = mats + 3; chunk.Matrices.m_capacity = mats + 3;

    reset();
    { CCharString r = chunk.GetChunkInfo();
      CHECK(g_textCalls == 1 && g_lenArg == -1);
      CHECK(std::strcmp(r.m_pStringData->m_pChars, "Bone Name: spine. Bone Index: 7, Frames: 3, Framerate: 30.000000") == 0); }

    // No name: the empty literal; negative index, no frames, fractional rate.
    reset(); chunk.BoneName.m_pStringData = 0; chunk.BoneIndex = -1; chunk.Matrices.m_end = mats; chunk.SamplesPerSecond = 12.5f;
    { CCharString r = chunk.GetChunkInfo();
      CHECK(g_textCalls == 1 && std::strcmp(r.m_pStringData->m_pChars, "Bone Name: . Bone Index: -1, Frames: 0, Framerate: 12.500000") == 0); }

    // A 2000-character name: truncated to 1023 characters and terminated.
    static char longName[2001]; std::memset(longName, 'x', 2000); longName[2000] = 0;
    CCharStringData longData = { longName, 2000 };
    reset(); chunk.BoneName.m_pStringData = &longData;
    { CCharString r = chunk.GetChunkInfo();
      const char* t = r.m_pStringData->m_pChars;
      CHECK(g_textCalls == 1 && r.m_pStringData->m_length == 1023 && std::strncmp(t, "Bone Name: xxx", 14) == 0 && t[1022] == 'x'); }

    if (g_fail) { std::printf("00aaf5b0_TEST FAIL\n"); return 1; }
    std::printf("00aaf5b0_TEST PASS\n");
    return 0;
}
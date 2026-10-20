// Behaviour test for C3DAnimationSequenceData::InitialiseAsConstant @ 0x00A4FD70: builds a sequence chunk local
// holding exactly the given matrix, initialises the sequence from it (passing `mode`), then sets FrameCount and
// SamplesPerSecond and clears the 5-bit BoneType (leaving the three flag bits); the chunk is destroyed once, after the
// initialise, name before base. Checks two calls with different arguments and flag patterns.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                  // 0x0099EAE0
};

struct CMatrix3x4 { float m_row[3][4]; };           // 48 bytes

template <class T>
struct CArray {                                     // retail CArray: begin/end/capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pEndOfStorage;
    ~CArray() { if (m_pBegin) free(m_pBegin); }
};

struct CChunkedFileSuperChunk {
    virtual ~CChunkedFileSuperChunk();              // 0x00AA8490
    unsigned char _body[0x28];                      // +0x04 .. +0x2B
};

struct C3DAnimFileSequenceChunk : CChunkedFileSuperChunk {
    long BoneIndex;                                 // +0x2C
    long ParentIndex;                               // +0x30
    CCharString BoneName;                           // +0x34
    float SamplesPerSecond;                         // +0x38
    unsigned char BoneType;                         // +0x3C
    CArray<CMatrix3x4> Matrices;                    // +0x40
    C3DAnimFileSequenceChunk();                     // 0x00AAF730
    void AddMatrix(const CMatrix3x4& matrix);       // 0x00AAF800
};

struct C3DAnimationSequenceData {
    char* BoneName;                                 // +0x00
    float SamplesPerSecond;                         // +0x04
    unsigned short FrameCount;                      // +0x08
    unsigned char BoneType : 5;                     // +0x0A
    unsigned char EnabledFlag : 1;
    unsigned char AnimatedFlag : 1;
    unsigned char IdentityFlag : 1;
    void InitialiseFromChunk(const C3DAnimFileSequenceChunk& chunk, long mode);   // 0x00A4EFC0
    void InitialiseAsConstant(const CMatrix3x4& matrix, unsigned short frameCount, float samplesPerSecond, long mode);
};

void C3DAnimationSequenceData::InitialiseAsConstant(const CMatrix3x4& matrix, unsigned short frameCount, float samplesPerSecond, long mode)
{
    C3DAnimFileSequenceChunk chunk;
    chunk.AddMatrix(matrix);
    InitialiseFromChunk(chunk, mode);
    FrameCount = frameCount;
    SamplesPerSecond = samplesPerSecond;
    BoneType = 0;
}

// ---- instrumented stubs ----
static int g_seq = 0, g_ctorAt = 0, g_addAt = 0, g_initAt = 0, g_nameDtorAt = 0, g_baseDtorAt = 0;
static int g_ctors = 0, g_adds = 0, g_inits = 0, g_nameDtors = 0, g_baseDtors = 0;
static long g_initMode = 0;
static unsigned long g_initCount = 0;
static bool g_initMatrixOk = false, g_initChunkLive = false;
static const CMatrix3x4* g_expectMatrix = 0;
static const void* g_liveChunk = 0;

CCharString::~CCharString() { ++g_nameDtors; g_nameDtorAt = ++g_seq; if (m_pStringData) std::free(m_pStringData); }
CChunkedFileSuperChunk::~CChunkedFileSuperChunk() { ++g_baseDtors; g_baseDtorAt = ++g_seq; if (g_liveChunk == this) g_liveChunk = 0; }
C3DAnimFileSequenceChunk::C3DAnimFileSequenceChunk()
{
    ++g_ctors; g_ctorAt = ++g_seq; g_liveChunk = this;
    std::memset(_body, 0, sizeof(_body));
    BoneIndex = 0; ParentIndex = -1; BoneName.m_pStringData = 0; SamplesPerSecond = 0.0f; BoneType = 0;
    Matrices.m_pBegin = Matrices.m_pEnd = Matrices.m_pEndOfStorage = 0;
}
void C3DAnimFileSequenceChunk::AddMatrix(const CMatrix3x4& matrix)       // Matrices.push_back
{
    ++g_adds; g_addAt = ++g_seq;
    if (&matrix != g_expectMatrix) g_fail = 1;
    unsigned long n = (unsigned long)(Matrices.m_pEnd - Matrices.m_pBegin);
    CMatrix3x4* block = (CMatrix3x4*)std::malloc((n + 1) * sizeof(CMatrix3x4));
    if (n) std::memcpy(block, Matrices.m_pBegin, n * sizeof(CMatrix3x4));
    block[n] = matrix;
    if (Matrices.m_pBegin) std::free(Matrices.m_pBegin);
    Matrices.m_pBegin = block; Matrices.m_pEnd = block + n + 1; Matrices.m_pEndOfStorage = block + n + 1;
}
void C3DAnimationSequenceData::InitialiseFromChunk(const C3DAnimFileSequenceChunk& chunk, long mode)
{
    ++g_inits; g_initAt = ++g_seq; g_initMode = mode;
    g_initChunkLive = &chunk == g_liveChunk;
    g_initCount = (unsigned long)(chunk.Matrices.m_pEnd - chunk.Matrices.m_pBegin);
    g_initMatrixOk = g_initCount == 1 && std::memcmp(chunk.Matrices.m_pBegin, g_expectMatrix, sizeof(CMatrix3x4)) == 0;
    // what the real initialiser leaves behind, to be overwritten by the caller
    FrameCount = 1; SamplesPerSecond = 30.0f; BoneType = 0x15;
}

static void reset() { g_seq = g_ctorAt = g_addAt = g_initAt = g_nameDtorAt = g_baseDtorAt = 0; g_ctors = g_adds = g_inits = g_nameDtors = g_baseDtors = 0; g_initMode = 0; g_initCount = 0; g_initMatrixOk = g_initChunkLive = false; }

int main()
{
    CMatrix3x4 m; for (int r = 0; r < 3; ++r) for (int c = 0; c < 4; ++c) m.m_row[r][c] = (float)(r * 4 + c) + 0.5f;
    g_expectMatrix = &m;
    C3DAnimationSequenceData seq; std::memset(&seq, 0, sizeof(seq));
    char name[] = "bone"; seq.BoneName = name;

    // First call: flags enabled + identity set beforehand.
    reset(); seq.EnabledFlag = 1; seq.AnimatedFlag = 0; seq.IdentityFlag = 1; seq.BoneType = 3;
    seq.InitialiseAsConstant(m, 120, 15.0f, 2);
    CHECK(g_ctors == 1 && g_adds == 1 && g_inits == 1 && g_nameDtors == 1 && g_baseDtors == 1);
    CHECK(g_ctorAt == 1 && g_addAt == 2 && g_initAt == 3 && g_nameDtorAt == 4 && g_baseDtorAt == 5);
    CHECK(g_initChunkLive && g_initMatrixOk && g_initMode == 2 && g_liveChunk == 0);
    CHECK(seq.FrameCount == 120 && seq.SamplesPerSecond == 15.0f && seq.BoneType == 0);
    CHECK(seq.EnabledFlag == 1 && seq.AnimatedFlag == 0 && seq.IdentityFlag == 1 && seq.BoneName == name);

    // Second call: other arguments, other flag pattern.
    reset(); seq.EnabledFlag = 0; seq.AnimatedFlag = 1; seq.IdentityFlag = 0;
    m.m_row[2][3] = -7.0f;
    seq.InitialiseAsConstant(m, 0xffff, 29.97f, -1);
    CHECK(g_ctors == 1 && g_adds == 1 && g_inits == 1 && g_nameDtors == 1 && g_baseDtors == 1 && g_initMatrixOk && g_initMode == -1);
    CHECK(seq.FrameCount == 0xffff && seq.SamplesPerSecond == 29.97f && seq.BoneType == 0);
    CHECK(seq.EnabledFlag == 0 && seq.AnimatedFlag == 1 && seq.IdentityFlag == 0);

    if (g_fail) { std::printf("00a4fd70_TEST FAIL\n"); return 1; }
    std::printf("00a4fd70_TEST PASS\n");
    return 0;
}
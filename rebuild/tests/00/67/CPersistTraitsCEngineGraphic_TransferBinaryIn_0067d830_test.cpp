// Behaviour test (0x0067d830): CPersistTraits<CEngineGraphic>::TransferBinaryIn reads the Type as a long (stored as a byte), BankIndex, AnimStep, RenderSizeX, then the AdditiveAlpha byte as bool
#include <cstdio>
#include <cstring>
#include <cmath>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
// Memory input stream model: the chunk pointer/length fast path of CMemoryDataInputStream::Read and a
// ReadWithSrcChunkOverflow that serves the rest from the backing buffer (counted, so the slow path is checked).
struct TestStream {
    unsigned int         position;
    int                  sourceLength;
    const unsigned char* chunkBase;
    int                  chunkPosition;
    int                  chunkLength;
    const unsigned char* data;
    int                  overflowCalls;
    void Init(const unsigned char* bytes, int size, int chunk)
    {
        position = 0; sourceLength = size; chunkBase = bytes; chunkPosition = 0; chunkLength = chunk; data = bytes; overflowCalls = 0;
    }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        ++overflowCalls;
        std::memcpy(buffer, data + position, size);
        position += size;
        chunkBase = data + position;
        chunkLength = sourceLength - (int)position;
    }
    static bool IsValidPosition(i64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        i64 newPosition = (i64)(unsigned int)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            std::memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
static void PutLong(unsigned char* p, int v) { std::memcpy(p, &v, 4); }
static void PutFloat(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
struct CEngineGraphic { unsigned int BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
static void TransferBinaryIn(TestStream* stream, CEngineGraphic* value)
{
    int type;
    { int n; stream->Read(&n, 4); type = n; }
    value->Type = (unsigned char)type;
    { unsigned int n; stream->Read(&n, 4); value->BankIndex = n; }
    { float f; stream->Read(&f, 4); value->AnimStep = f; }
    { float f; stream->Read(&f, 4); value->RenderSizeX = f; }
    { unsigned char b; stream->Read(&b, 1); value->AdditiveAlpha = b != 0; }
}
int main()
{
    unsigned char bytes[17];
    PutLong(bytes, 0x103); PutLong(bytes + 4, 77); PutFloat(bytes + 8, 0.25f); PutFloat(bytes + 12, 64.0f); bytes[16] = 1;
    TestStream s; s.Init(bytes, 17, 17);
    CEngineGraphic g;
    TransferBinaryIn(&s, &g);
    if (g.Type != 3 || g.BankIndex != 77 || std::fabs(g.AnimStep - 0.25f) > 1e-6f || std::fabs(g.RenderSizeX - 64.0f) > 1e-4f || !g.AdditiveAlpha || s.position != 17) { std::printf("0067d830_TEST FAIL\n"); return 1; }
    std::printf("0067d830_TEST PASS\n");
    return 0;
}
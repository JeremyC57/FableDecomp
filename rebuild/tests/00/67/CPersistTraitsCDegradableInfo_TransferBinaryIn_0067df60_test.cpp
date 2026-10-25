// Behaviour test (0x0067df60): CPersistTraits<CDegradableInfo>::TransferBinaryIn reads HealthPercentage, the raw 14-byte engine graphic, SmashParticleEmitter, the BlocksNavigation byte as bool, then skips a 4-byte check word
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
#pragma pack(push, 1)
struct CEngineGraphic { unsigned int BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
#pragma pack(pop)
struct CDegradableInfo { float HealthPercentage; CEngineGraphic EngineGraphic; int SmashParticleEmitter; bool BlocksNavigation; };
static void TransferBinaryIn(TestStream* stream, CDegradableInfo* value)
{
    { float f; stream->Read(&f, 4); value->HealthPercentage = f; }
    stream->Read(&value->EngineGraphic, sizeof(CEngineGraphic));
    { int n; stream->Read(&n, 4); value->SmashParticleEmitter = n; }
    { unsigned char b; stream->Read(&b, 1); value->BlocksNavigation = b != 0; }
    { int check; stream->Read(&check, 4); }
}
int main()
{
    if ((unsigned int)sizeof(CEngineGraphic) != 14u) { std::printf("0067df60_TEST FAIL\n"); return 1; }
    unsigned char bytes[27];
    PutFloat(bytes, 0.5f);
    CEngineGraphic g; g.BankIndex = 5; g.AnimStep = 1.0f; g.RenderSizeX = 2.0f; g.AdditiveAlpha = true; g.Type = 9;
    std::memcpy(bytes + 4, &g, 14);
    PutLong(bytes + 18, 33); bytes[22] = 1; PutLong(bytes + 23, 0x12345678);
    TestStream s; s.Init(bytes, 27, 27);
    CDegradableInfo d;
    TransferBinaryIn(&s, &d);
    if (std::fabs(d.HealthPercentage - 0.5f) > 1e-6f || d.EngineGraphic.BankIndex != 5 || d.EngineGraphic.Type != 9 || !d.EngineGraphic.AdditiveAlpha
        || d.SmashParticleEmitter != 33 || !d.BlocksNavigation || s.position != 27) { std::printf("0067df60_TEST FAIL\n"); return 1; }
    std::printf("0067df60_TEST PASS\n");
    return 0;
}
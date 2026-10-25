// Behaviour test (0x004e9642): CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn reads PeriodSecs, four particle-effect longs, then the StartPoint and EndPoint def-string arrays
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
struct CDefStringArray {
    int Count; int First;
};
static int g_vectorReads = 0;
static void GFSerialiseVectorBinaryIn(TestStream& stream, CDefStringArray& out)   // stand-in for 0x00465852: count then elements
{
    ++g_vectorReads;
    int n; stream.Read(&n, 4); out.Count = n; out.First = -1;
    for (int i = 0; i < n; ++i) { int v; stream.Read(&v, 4); if (i == 0) out.First = v; }
}
struct CObjectAugmentationParticleSet {
    float PeriodSecs; int ParticleEffect; int ParticleEffectToBlendTo; int AttackParticleEffect; int AttackParticleEffectToBlendTo;
    CDefStringArray StartPoint; CDefStringArray EndPoint;
};
static void ReadLong(TestStream* s, int* v) { int n; s->Read(&n, 4); *v = n; }
static void TransferBinaryIn(TestStream* stream, CObjectAugmentationParticleSet* value)
{
    { float f; stream->Read(&f, 4); value->PeriodSecs = f; }
    ReadLong(stream, &value->ParticleEffect);
    ReadLong(stream, &value->ParticleEffectToBlendTo);
    ReadLong(stream, &value->AttackParticleEffect);
    ReadLong(stream, &value->AttackParticleEffectToBlendTo);
    GFSerialiseVectorBinaryIn(*stream, value->StartPoint);
    GFSerialiseVectorBinaryIn(*stream, value->EndPoint);
}
int main()
{
    unsigned char bytes[44];
    PutFloat(bytes, 2.0f); PutLong(bytes + 4, 11); PutLong(bytes + 8, 12); PutLong(bytes + 12, 13); PutLong(bytes + 16, 14);
    PutLong(bytes + 20, 2); PutLong(bytes + 24, 100); PutLong(bytes + 28, 101);
    PutLong(bytes + 32, 1); PutLong(bytes + 36, 200); PutLong(bytes + 40, 0);
    TestStream s; s.Init(bytes, 44, 44);
    CObjectAugmentationParticleSet p;
    TransferBinaryIn(&s, &p);
    if (std::fabs(p.PeriodSecs - 2.0f) > 1e-6f || p.ParticleEffect != 11 || p.ParticleEffectToBlendTo != 12 || p.AttackParticleEffect != 13
        || p.AttackParticleEffectToBlendTo != 14 || p.StartPoint.Count != 2 || p.StartPoint.First != 100 || p.EndPoint.Count != 1
        || p.EndPoint.First != 200 || g_vectorReads != 2 || s.position != 40) { std::printf("004e9642_TEST FAIL\n"); return 1; }
    std::printf("004e9642_TEST PASS\n");
    return 0;
}
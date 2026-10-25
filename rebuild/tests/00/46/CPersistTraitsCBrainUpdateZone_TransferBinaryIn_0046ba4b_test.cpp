// Behaviour test (0x0046ba4b): CPersistTraits<CBrainUpdateZone>::TransferBinaryIn reads Distance, UpdateFrequency, InterruptionFrequency as three 4-byte longs
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
struct CBrainUpdateZone { int Distance; int UpdateFrequency; int InterruptionFrequency; };
static void TransferBinaryInLong(TestStream* stream, int* value) { int number; stream->Read(&number, sizeof(number)); *value = number; }
static void TransferBinaryIn(TestStream* stream, CBrainUpdateZone* value)
{
    TransferBinaryInLong(stream, &value->Distance);
    TransferBinaryInLong(stream, &value->UpdateFrequency);
    TransferBinaryInLong(stream, &value->InterruptionFrequency);
}
int main()
{
    unsigned char bytes[12];
    PutLong(bytes, 1500); PutLong(bytes + 4, 7); PutLong(bytes + 8, -3);
    TestStream s; s.Init(bytes, 12, 12);
    CBrainUpdateZone z; z.Distance = z.UpdateFrequency = z.InterruptionFrequency = 0;
    TransferBinaryIn(&s, &z);
    if (z.Distance != 1500 || z.UpdateFrequency != 7 || z.InterruptionFrequency != -3 || s.position != 12 || s.overflowCalls != 0) { std::printf("0046ba4b_TEST FAIL\n"); return 1; }
    TestStream t; t.Init(bytes, 12, 6);                              // chunk runs out inside the second long
    CBrainUpdateZone w; w.Distance = w.UpdateFrequency = w.InterruptionFrequency = 0;
    TransferBinaryIn(&t, &w);
    if (w.Distance != 1500 || w.UpdateFrequency != 7 || w.InterruptionFrequency != -3 || t.overflowCalls != 1) { std::printf("0046ba4b_TEST FAIL\n"); return 1; }
    std::printf("0046ba4b_TEST PASS\n");
    return 0;
}
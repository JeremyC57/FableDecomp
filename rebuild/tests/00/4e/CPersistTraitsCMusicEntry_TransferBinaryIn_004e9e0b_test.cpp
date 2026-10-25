// Behaviour test (0x004e9e0b): CPersistTraits<CMusicEntry>::TransferBinaryIn (second-TU copy) reads ID, the Filename def string (its own reader), Gain, then a CacheToHD byte as bool
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
struct CDefString {
    int Index;
    void TransferBinaryIn(TestStream& stream) { int v; stream.Read(&v, sizeof(v)); Index = v; }   // stand-in for 0x0044FC00
};
struct CMusicEntry { unsigned int ID; CDefString Filename; float Gain; bool CacheToHD; };
static void TransferBinaryIn(TestStream* stream, CMusicEntry* value)
{
    { unsigned int n; stream->Read(&n, sizeof(n)); value->ID = n; }
    value->Filename.TransferBinaryIn(*stream);
    { float f; stream->Read(&f, sizeof(f)); value->Gain = f; }
    { unsigned char b; stream->Read(&b, sizeof(b)); value->CacheToHD = b != 0; }
}
int main()
{
    unsigned char bytes[13];
    PutLong(bytes, 7); PutLong(bytes + 4, 3); PutFloat(bytes + 8, 0.75f); bytes[12] = 1;
    TestStream s; s.Init(bytes, 13, 13);
    CMusicEntry e; e.ID = 0; e.Filename.Index = 0; e.Gain = 0.0f; e.CacheToHD = false;
    TransferBinaryIn(&s, &e);
    if (e.ID != 7 || e.Filename.Index != 3 || std::fabs(e.Gain - 0.75f) > 1e-6f || !e.CacheToHD || s.position != 13) { std::printf("004e9e0b_TEST FAIL\n"); return 1; }
    std::printf("004e9e0b_TEST PASS\n");
    return 0;
}
// Behaviour test (0x009950f0): CDataInputStream::Read3DVector reads X, Y, Z floats in order and returns them as a vector
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
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static float ReadFloat(TestStream& s) { float v; s.Read(&v, sizeof(v)); return v; }
static C3DVector Read3DVector(TestStream& s)
{
    float x = ReadFloat(s);
    float y = ReadFloat(s);
    float z = ReadFloat(s);
    return C3DVector(x, y, z);
}
int main()
{
    unsigned char bytes[12];
    PutFloat(bytes, 1.5f); PutFloat(bytes + 4, -2.25f); PutFloat(bytes + 8, 100.0f);
    TestStream s; s.Init(bytes, 12, 12);
    C3DVector v = Read3DVector(s);
    if (std::fabs(v.X - 1.5f) > 1e-6f || std::fabs(v.Y + 2.25f) > 1e-6f || std::fabs(v.Z - 100.0f) > 1e-4f || s.position != 12) { std::printf("009950f0_TEST FAIL\n"); return 1; }
    TestStream t; t.Init(bytes, 12, 4);
    C3DVector w = Read3DVector(t);
    if (std::fabs(w.Y + 2.25f) > 1e-6f || std::fabs(w.Z - 100.0f) > 1e-4f || t.overflowCalls != 1) { std::printf("009950f0_TEST FAIL\n"); return 1; }
    std::printf("009950f0_TEST PASS\n");
    return 0;
}
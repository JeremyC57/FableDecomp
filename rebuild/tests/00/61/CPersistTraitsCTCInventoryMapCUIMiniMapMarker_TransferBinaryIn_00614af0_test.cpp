// Behaviour test (0x00614af0): CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferBinaryIn resets the marker, then reads Region, DefNameLength, a name string of that length, Position, GraphicIndex and Angle
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
struct CCharString { char Text[64]; CCharString(const char* s) { std::strcpy(Text, s); } };
struct C2DVector { float X, Y; };
struct CUIMiniMapMarker {
    int Region; CCharString UIDefName; C2DVector Position; int DefNameLength; int GraphicIndex; float Angle;
    CUIMiniMapMarker() : Region(0), UIDefName(""), DefNameLength(0), GraphicIndex(0), Angle(0.0f) { Position.X = 0.0f; Position.Y = 0.0f; }
};
static CCharString ReadString(TestStream& s, int length)       // stand-in for CDataInputStream::ReadString 0x00996BE0
{
    char buffer[64]; s.Read(buffer, length); buffer[length] = 0; return CCharString(buffer);
}
static void ReadLong(TestStream* s, int* v) { int n; s->Read(&n, 4); *v = n; }
static void ReadFloat(TestStream* s, float* v) { float f; s->Read(&f, 4); *v = f; }
static void TransferBinaryIn(TestStream* stream, CUIMiniMapMarker* value)
{
    *value = CUIMiniMapMarker();
    ReadLong(stream, &value->Region);
    ReadLong(stream, &value->DefNameLength);
    value->UIDefName = ReadString(*stream, value->DefNameLength);
    ReadFloat(stream, &value->Position.X);
    ReadFloat(stream, &value->Position.Y);
    ReadLong(stream, &value->GraphicIndex);
    ReadFloat(stream, &value->Angle);
}
int main()
{
    unsigned char bytes[29];
    PutLong(bytes, 4); PutLong(bytes + 4, 5); std::memcpy(bytes + 8, "HOUSE", 5);
    PutFloat(bytes + 13, 10.0f); PutFloat(bytes + 17, -4.0f); PutLong(bytes + 21, 6); PutFloat(bytes + 25, 1.25f);
    TestStream s; s.Init(bytes, 29, 29);
    CUIMiniMapMarker m; m.GraphicIndex = 99;
    TransferBinaryIn(&s, &m);
    if (m.Region != 4 || m.DefNameLength != 5 || std::strcmp(m.UIDefName.Text, "HOUSE") != 0 || std::fabs(m.Position.X - 10.0f) > 1e-5f
        || std::fabs(m.Position.Y + 4.0f) > 1e-5f || m.GraphicIndex != 6 || std::fabs(m.Angle - 1.25f) > 1e-6f || s.position != 29) { std::printf("00614af0_TEST FAIL\n"); return 1; }
    std::printf("00614af0_TEST PASS\n");
    return 0;
}
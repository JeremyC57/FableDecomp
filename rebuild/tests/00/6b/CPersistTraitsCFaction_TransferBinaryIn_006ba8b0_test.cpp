// Behaviour test (0x006ba8b0): CPersistTraits<CFaction>::TransferBinaryIn reads the text length, that many bytes into a zeroed NUL-terminated buffer, and hands the text to the text TransferIn
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
#include <cstdlib>
struct CByteArray {                                   // CArray<char> (STLport vector) stand-in
    char* Start; char* Finish; char* End;
    explicit CByteArray(unsigned int n) { Start = n ? (char*)std::malloc(n) : 0; End = Start + n; std::memset(Start, 0, n); Finish = Start + n; }
    ~CByteArray() { if (Start) std::free(Start); }
    char* GetData() { return (Finish - Start) ? Start : (char*)this; }
    char& operator[](unsigned int i) { return Start[i]; }
};
struct CFaction { char Text[64]; int Calls; };
static void TransferIn(const char* text, CFaction& value) { std::strcpy(value.Text, text); ++value.Calls; }   // the text parser + TransferIn stand-in
struct TestByteStream : TestStream { void ReadBytes(void* buffer, int size) { Read(buffer, size); } };
static void TransferBinaryIn(TestByteStream& stream, CFaction& value)
{
    int length;
    { int v; stream.Read(&v, sizeof(v)); length = v; }
    CByteArray buffer(length + 1);
    stream.ReadBytes(buffer.GetData(), length);
    buffer[length] = 0;
    TransferIn(buffer.GetData(), value);
}
int main()
{
    unsigned char bytes[20];
    PutLong(bytes, 11); std::memcpy(bytes + 4, "Name F_HERO", 11);
    TestByteStream s; s.Init(bytes, 15, 15);
    CFaction f; f.Text[0] = 'x'; f.Text[1] = 0; f.Calls = 0;
    TransferBinaryIn(s, f);
    if (f.Calls != 1 || std::strcmp(f.Text, "Name F_HERO") != 0 || s.position != 15) { std::printf("006ba8b0_TEST FAIL\n"); return 1; }
    PutLong(bytes, 0);
    TestByteStream t; t.Init(bytes, 4, 4);
    TransferBinaryIn(t, f);
    if (f.Calls != 2 || f.Text[0] != 0) { std::printf("006ba8b0_TEST FAIL\n"); return 1; }
    std::printf("006ba8b0_TEST PASS\n");
    return 0;
}
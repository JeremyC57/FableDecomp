// Behaviour test for 0x009945E0 CDataInputStream::ReadColour: 4 bytes come straight from the current chunk (pointer,
// remaining count and position advance), a short chunk defers to the overflow reader, an out-of-range position leaves the
// colour zero.
#include <cstdio>
#include <cstring>
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) {}
    CRGBColour(const CRGBColour& o) { R = o.R; G = o.G; B = o.B; A = o.A; }
};
static int g_overflow = 0;
struct CStream {
    unsigned int StreamPos; const unsigned char* CurrentSourcePtr; int SourceChunkBytesRemaining;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++g_overflow; std::memset(buffer, 0x5a, size); }
    static bool IsValidPosition(long long p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        long long np = (long long)(unsigned int)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    CRGBColour ReadColour() { CRGBColour c; Read(&c, 4); return c; }
};
int main()
{
    unsigned char data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    CStream s; s.StreamPos = 10; s.CurrentSourcePtr = data; s.SourceChunkBytesRemaining = 8;
    CRGBColour c = s.ReadColour();
    if (c.B != 1 || c.G != 2 || c.R != 3 || c.A != 4 || s.StreamPos != 14 || s.SourceChunkBytesRemaining != 4 || s.CurrentSourcePtr != data + 4)
        { std::printf("009945e0_TEST FAIL\n"); return 1; }
    s.SourceChunkBytesRemaining = 2;
    c = s.ReadColour();
    if (g_overflow != 1 || c.R != 0x5a) { std::printf("009945e0_TEST FAIL\n"); return 1; }
    s.StreamPos = 0x7ffffffe; s.SourceChunkBytesRemaining = 8;
    c = s.ReadColour();
    if (c.B || c.G || c.R || c.A || g_overflow != 1 || s.StreamPos != 0x7ffffffe) { std::printf("009945e0_TEST FAIL\n"); return 1; }
    std::printf("009945e0_TEST PASS\n");
    return 0;
}
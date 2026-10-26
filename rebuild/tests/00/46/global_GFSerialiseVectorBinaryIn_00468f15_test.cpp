// Behaviour test for 0x00468F15 GFSerialiseVectorBinaryIn<CRGBColour>: count, resize, then one colour per element
#include <cstdio>
#include <cstring>
class CRGBColour { public: unsigned char B, G, R, A; };
struct ColourVector {                             // stand-in for the out-of-line resize(n, x) @ 0x00468FA5
    CRGBColour data[16];
    unsigned int count;
    int resizeCalls;
    void resize(unsigned int n, const CRGBColour& x) { ++resizeCalls; for (unsigned int i = count; i < n; ++i) data[i] = x; count = n; }
    CRGBColour& operator[](unsigned int i) { return data[i]; }
};
struct Stream {                                   // CDataInputStream fields used by the inlined Read
    unsigned int StreamPos;
    const unsigned char* CurrentSourcePtr;
    int SourceChunkBytesRemaining;
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++overflowCalls; std::memset(buffer, 0, size); }
    void Read(void* buffer, int size)
    {
        long long newPosition = (long long)(unsigned int)(StreamPos + size);
        if (!(newPosition >= 0 && newPosition <= 0x7fffffff)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    CRGBColour ReadColour() { CRGBColour c; c.B = c.G = c.R = c.A = 0; Read(&c, 4); return c; }
};
static void SerialiseIn(Stream& stream, ColourVector& out)
{
    int count;
    stream.Read(&count, 4);
    CRGBColour unset;                                 // the never-written resize temporary
    out.resize(count, unset);
    for (int i = 0; i < count; ++i)
        out[i] = stream.ReadColour();
}
int main()
{
    unsigned char data[4 + 3 * 4] = { 3, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    Stream s = { 0, data, (int)sizeof(data), 0 };
    ColourVector v; v.count = 0; v.resizeCalls = 0;
    SerialiseIn(s, v);
    if (v.count != 3 || v.resizeCalls != 1 || v[0].B != 1 || v[0].A != 4 || v[2].R != 11 || s.StreamPos != 16
        || s.SourceChunkBytesRemaining != 0 || s.overflowCalls != 0) {
        std::printf("00468f15_TEST FAIL\n"); return 1;
    }
    unsigned char zero[4] = { 0, 0, 0, 0 };
    Stream e = { 0, zero, 4, 0 };
    SerialiseIn(e, v);
    if (v.count != 0 || e.StreamPos != 4) { std::printf("00468f15_TEST FAIL\n"); return 1; }
    unsigned char shortChunk[2] = { 1, 1 };
    Stream o = { 0, shortChunk, 2, 0 };               // chunk too short: the overflow reader supplies the count (0)
    SerialiseIn(o, v);
    if (o.overflowCalls != 1 || v.count != 0) { std::printf("00468f15_TEST FAIL\n"); return 1; }
    std::printf("00468f15_TEST PASS\n");
    return 0;
}
// Test: CDataOutputStream::WriteCompressed -- nothing for size <= 0; sizes 1..3 and incompressible blocks get a zero
// 2-byte header then the raw bytes; otherwise the 2-byte compressed length (or 0xFFFF + a 4-byte length above 0x7FFF),
// the compressed bytes of the first size - 3 bytes, then the last 3 raw bytes. The work memory is zero-filled.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static unsigned long g_fakeCompressed;   // the size the fake compressor reports
static bool g_workZero;
static int fake_compress(const unsigned char* src, unsigned long n, unsigned char* dst, unsigned long* dstLen, void* wrk)
{
    unsigned char* w = (unsigned char*)wrk; g_workZero = w[0] == 0 && w[0x6FFFF] == 0;
    *dstLen = g_fakeCompressed; for (unsigned long i = 0; i < g_fakeCompressed; ++i) dst[i] = (unsigned char)(0xA0 + i); return 0;
}
static int fake_decompress(const unsigned char*, unsigned long, unsigned char*, unsigned long*, void*) { return 0; }
struct Stream {
    unsigned char out[200000]; long pos;
    Stream() : pos(0) {}
    void WriteData(const void* d, long n) { memcpy(out + pos, d, n); pos += n; }
    void WriteCompressed(const void* data, long size)
    {
        if (size <= 0) return;
        if (size > 3) {
            unsigned long compressedSize = size * 65 / 64 + 128;
            unsigned char* compressed = (unsigned char*)calloc(compressedSize, 1);
            unsigned char* work = (unsigned char*)malloc(0x70000); memset(work, 0, 0x70000);
            fake_compress((const unsigned char*)data, size - 3, compressed, &compressedSize, work);
            unsigned long checkSize = size;
            fake_decompress(compressed, compressedSize, (unsigned char*)data, &checkSize, 0);
            if (compressedSize < (unsigned long)(size - 3)) {
                long header;
                if (compressedSize <= 0x7fff) { header = compressedSize; WriteData(&header, 2); }
                else { header = -1; WriteData(&header, 2); header = compressedSize; WriteData(&header, 4); }
                WriteData(compressed, compressedSize);
                WriteData((const unsigned char*)data + size - 3, 3);
            } else { long header = 0; WriteData(&header, 2); WriteData(data, size); }
            free(work); free(compressed);
        } else { long header = 0; WriteData(&header, 2); WriteData(data, size); }
    }
};
static unsigned char src[100000];
int main()
{
    bool ok = true;
    for (int i = 0; i < 100000; ++i) src[i] = (unsigned char)i;
    Stream* s = new Stream; s->WriteCompressed(src, 0); if (s->pos != 0) ok = false;
    s->WriteCompressed(src, 3); if (s->pos != 5 || s->out[0] || s->out[1] || s->out[4] != 2) ok = false;
    s->pos = 0; g_fakeCompressed = 10; s->WriteCompressed(src, 100);
    if (s->pos != 2 + 10 + 3 || s->out[0] != 10 || s->out[1] != 0 || s->out[2] != 0xA0 || s->out[12] != 97 || !g_workZero) ok = false;
    s->pos = 0; g_fakeCompressed = 200; s->WriteCompressed(src, 100);
    if (s->pos != 102 || s->out[0] || s->out[1] || s->out[2] != 0) ok = false;
    s->pos = 0; g_fakeCompressed = 0x8000; s->WriteCompressed(src, 90000);
    if (s->pos != 2 + 4 + 0x8000 + 3 || s->out[0] != 0xFF || s->out[1] != 0xFF || s->out[3] != 0x80 || s->out[2] != 0) ok = false;
    printf(ok ? "00996610_TEST PASS\n" : "00996610_TEST FAIL\n");
    delete s; return ok ? 0 : 1;
}
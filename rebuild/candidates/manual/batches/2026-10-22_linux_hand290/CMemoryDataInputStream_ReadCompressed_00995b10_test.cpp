// Behaviour test for CMemoryDataInputStream::ReadCompressed (0x00995B10): short count (-1 escapes to a long), the compressed
// bytes are decompressed straight from the source buffer, then the last three bytes are read raw; count <= 0 reads raw.
#include <cstdio>
#include <cstring>

static int g_asserts = 0;
// stand-in "decompressor": copies srcLen bytes, reports how many were written
static int Decompress(const unsigned char* src, unsigned int srcLen, unsigned char* dst, unsigned int* dstLen)
{
    std::memcpy(dst, src, srcLen);
    *dstLen = srcLen;
    return 0;
}

struct Stream {
    const unsigned char* base;
    unsigned int pos;
    unsigned int size;
    unsigned int GetPosition() { return pos; }
    void Read(void* out, int n) { if (n <= 0) return; if (pos + n <= size) { std::memcpy(out, base + pos, n); pos += n; } }
    int ReadLong() { int v = 0; Read(&v, 4); return v; }
    short ReadShort() { short v = 0; Read(&v, 2); return v; }
    const unsigned char* GetDataAndSkip(int n) { unsigned int p = GetPosition(); pos = p + n; return base + p; }
    void ReadCompressed(void* data, int n)
    {
        if (n > 0) {
            int compressed = ReadShort();
            if (compressed == -1) compressed = ReadLong();
            if (compressed > 0) {
                const unsigned char* src = GetDataAndSkip(compressed);
                unsigned int written = n;
                Decompress(src, compressed, (unsigned char*)data, &written);
                if (written != (unsigned int)(n - 3)) ++g_asserts;
                Read((unsigned char*)data + n - 3, 3);
            } else
                Read(data, n);
        }
    }
};

int main()
{
    unsigned char buf[32];
    unsigned int i;
    for (i = 0; i < sizeof(buf); ++i) buf[i] = (unsigned char)i;
    short c = 4; std::memcpy(buf, &c, 2);              // 4 "compressed" bytes (2..5), then 3 raw (6..8)
    Stream s; s.base = buf; s.pos = 0; s.size = sizeof(buf);
    unsigned char out[7]; std::memset(out, 0, sizeof(out));
    s.ReadCompressed(out, 7);
    if (out[0] != 2 || out[3] != 5 || out[4] != 6 || out[6] != 8 || s.pos != 9 || g_asserts != 0) { std::printf("00995b10_TEST FAIL\n"); return 1; }
    short m = -1; std::memcpy(buf, &m, 2); int l = 2; std::memcpy(buf + 2, &l, 4);   // long count 2, then 3 raw
    s.pos = 0;
    s.ReadCompressed(out, 7);
    if (s.pos != 11 || g_asserts != 1 || out[4] != 8) { std::printf("00995b10_TEST FAIL\n"); return 1; }
    short z = 0; std::memcpy(buf, &z, 2);
    s.pos = 0;
    s.ReadCompressed(out, 4);
    if (s.pos != 6 || out[0] != 2 || out[1] != 0 || out[3] != 0) { std::printf("00995b10_TEST FAIL\n"); return 1; }
    std::printf("00995b10_TEST PASS\n");
    return 0;
}

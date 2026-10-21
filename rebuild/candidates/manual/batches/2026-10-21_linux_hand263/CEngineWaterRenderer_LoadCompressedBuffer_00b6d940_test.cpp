// behaviour test for 00b6d940 CEngineWaterRenderer::LoadCompressedBuffer: count and stride are read, a zero count or
// stride leaves the buffer uninitialised, otherwise the buffer is created, the compressed block is handed to the
// decompressor with the count and stride, and the count is reported
#include <cstdio>
#include <cstring>
struct Stream {
    const unsigned char* base; unsigned int pos;
    int ReadLong() { int v; std::memcpy(&v, base + pos, 4); pos += 4; return v; }
    const unsigned char* GetDataAndSkip(int size) { const unsigned char* p = base + pos; pos += size; return p; }
};
struct Buffer { int initialised; int count, stride; int locked; };
static int g_decCount = 0, g_decStride = 0; static const unsigned char* g_decSrc = 0;
static void Load(Stream& s, Buffer& b, int* out)
{
    b.initialised = 0;
    int count = s.ReadLong();
    int stride = s.ReadLong();
    if (count && stride) {
        b.initialised = 1; b.count = count; b.stride = stride; b.locked = 1;
        int compressed = s.ReadLong();
        if (compressed > 0) { g_decSrc = s.GetDataAndSkip(compressed); g_decCount = count; g_decStride = stride; }
        b.locked = 0;
    }
    if (out) *out = count;
}
int main()
{
    unsigned char data[32]; std::memset(data, 0, sizeof(data));
    int v[3] = { 7, 24, 5 }; std::memcpy(data, v, sizeof(v));
    Stream s = { data, 0 }; Buffer b; int out = -1;
    Load(s, b, &out);
    if (!b.initialised || out != 7 || g_decCount != 7 || g_decStride != 24 || g_decSrc != data + 12 || s.pos != 17) {
        std::printf("00b6d940_TEST FAIL\n"); return 1;
    }
    int z[2] = { 0, 24 }; std::memcpy(data, z, sizeof(z));
    Stream t = { data, 0 }; Load(t, b, &out);
    if (b.initialised || out != 0 || t.pos != 8) { std::printf("00b6d940_TEST FAIL\n"); return 1; }
    Load(t, b, 0);
    std::printf("00b6d940_TEST PASS\n");
    return 0;
}

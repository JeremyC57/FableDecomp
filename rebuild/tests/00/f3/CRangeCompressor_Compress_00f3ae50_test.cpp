// Behaviour test for 0x00F3AE50 CRangeCompressor::Compress(const void*, long, long, unsigned char*, long): empty or
// out-of-range input writes flag 0 and returns 1; otherwise the scripted stream is used (flag 1, its size + 1) only when its
// size + 2 is below the raw size + 1, else flag 0 and a raw copy (size + 1); both sizes go to the statistics and the output
// is decompressed into a scratch array of the input size.
#include <stdio.h>
#include <string.h>
#include <vector>

#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif

static long g_in, g_out, g_scriptSize, g_decompressed;
struct CRangeCompressor {
    long CalcCompressionScript(std::vector<int>& script, const unsigned char*, long count, long stride, long blockSize)
    {
        script.push_back((int)count);
        return blockSize == stride ? g_scriptSize : -1000;
    }
    long CompressScripted(const unsigned char* data, long, long, unsigned char* dest, const std::vector<int>& script)
    {
        dest[0] = data[0]; dest[1] = (unsigned char)script.size();
        return 2;
    }
    void Decompress(const unsigned char* data, long count, long stride, void* dest) { g_decompressed = count * stride; (void)data; (void)dest; }
    long Compress(const void* data, long count, long stride, unsigned char* dest, long)
    {
        if (count == 0) { dest[0] = 0; return 1; }
        long size = count * stride;
        Int64 size64 = size;
        if (size64 < 0 || size64 > 0x7fffffff) { dest[0] = 0; return 1; }
        std::vector<int> script;
        long written;
        if (CalcCompressionScript(script, (const unsigned char*)data, count, stride, stride) + 2 >= size + 1) {
            dest[0] = 0;
            memcpy(dest + 1, data, size);
            written = size + 1;
        } else {
            dest[0] = 1;
            written = CompressScripted((const unsigned char*)data, count, stride, dest + 1, script) + 1;
        }
        g_in += size;
        g_out += written;
        std::vector<unsigned char> check(size, 0);
        Decompress(dest, count, stride, check.empty() ? (void*)&check : (void*)&check[0]);
        return written;
    }
};

int main()
{
    CRangeCompressor c; unsigned char src[16], dst[32];
    for (int i = 0; i < 16; ++i) src[i] = (unsigned char)(i + 1);
    dst[0] = 0xAA;
    int ok = c.Compress(src, 0, 4, dst, 32) == 1 && dst[0] == 0 && g_in == 0;
    g_scriptSize = 20;                                     // 22 >= 17: raw
    long n = c.Compress(src, 4, 4, dst, 32);
    ok = ok && n == 17 && dst[0] == 0 && memcmp(dst + 1, src, 16) == 0 && g_in == 16 && g_out == 17 && g_decompressed == 16;
    g_scriptSize = 3;                                      // 5 < 17: scripted
    n = c.Compress(src, 4, 4, dst, 32);
    ok = ok && n == 3 && dst[0] == 1 && dst[1] == 1 && dst[2] == 1 && g_in == 32 && g_out == 20;
    ok = ok && c.Compress(src, -2, 4, dst, 32) == 1 && dst[0] == 0 && g_in == 32;
    if (!ok) { printf("00f3ae50_TEST FAIL\n"); return 1; }
    printf("00f3ae50_TEST PASS\n");
    return 0;
}
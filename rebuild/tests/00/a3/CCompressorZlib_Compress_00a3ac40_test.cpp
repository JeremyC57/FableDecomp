// Behaviour test for 00a3ac40 CCompressorZlib::Compress with a stub deflate worker: an empty source
// gives a header-only block (13 bytes), otherwise the payload goes 13 bytes in and the header and info
// report the source size and payload + 13.
#include <cstdio>
#include <string.h>

struct SInfo { unsigned long Uncompressed; unsigned long Compressed; unsigned char Method; };
static unsigned long g_workerDestSize = 0; static unsigned char* g_workerDest = 0;
static unsigned long InternalCompress(const void*, unsigned long srcSize, void* dest, unsigned long destSize)
{ g_workerDest = (unsigned char*)dest; g_workerDestSize = destSize; return srcSize / 2; }

static void PutHeader(unsigned char* d, unsigned long un, unsigned long co)
{ unsigned long tag = 0x434D5031; memcpy(d, &tag, 4); memcpy(d + 4, &un, 4); memcpy(d + 8, &co, 4); d[12] = 6; }

static void Compress(const void* src, unsigned long srcSize, void* dest, unsigned long destSize, SInfo& info)
{
    unsigned char* d = (unsigned char*)dest;
    if (srcSize <= 0) { PutHeader(d, 0, 13); info.Compressed = 13; info.Uncompressed = 0; info.Method = 6; return; }
    PutHeader(d, srcSize, srcSize);
    unsigned long co = InternalCompress(src, srcSize, d + 13, destSize - 13) + 13;
    memcpy(d + 8, &co, 4);
    info.Uncompressed = srcSize; info.Compressed = co; info.Method = 6;
}

int main()
{
    unsigned char out[100]; unsigned char src[40]; SInfo info; unsigned long v;
    Compress(src, 0, out, 100, info);
    memcpy(&v, out + 8, 4);
    bool ok = info.Compressed == 13 && info.Uncompressed == 0 && info.Method == 6 && v == 13 && out[0] == 0x31 && out[3] == 0x43;
    Compress(src, 40, out, 100, info);
    memcpy(&v, out + 8, 4);
    ok = ok && info.Compressed == 33 && info.Uncompressed == 40 && v == 33 && g_workerDest == out + 13 && g_workerDestSize == 87 && out[12] == 6;
    memcpy(&v, out + 4, 4); ok = ok && v == 40;
    if (!ok) { std::printf("00a3ac40_TEST FAIL\n"); return 1; }
    std::printf("00a3ac40_TEST PASS\n");
    return 0;
}
// Behaviour test for CDataInputStream::ReadCompressed (0x00995FB0): a short count (-1 escapes to a long count);
// a positive count reads that many compressed bytes, decompresses size-3 bytes into data (asserting otherwise) and
// reads the final three bytes raw; a count <= 0 reads size raw bytes; size <= 0 reads nothing.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_asserts = 0;
static void AssertHook() { ++g_asserts; }
static void (*g_pfnAssertionFailed)() = AssertHook;

// test decompressor: "decompresses" by copying the source bytes, each incremented by one
static int Decompress(const unsigned char* src, unsigned int srcLen, unsigned char* dst, unsigned int* dstLen, void*)
{
    for (unsigned int i = 0; i < srcLen; ++i) dst[i] = (unsigned char)(src[i] + 1);
    *dstLen = srcLen;
    return 0;
}

struct Stream {
    unsigned long StreamPos;
    unsigned char* CurrentSourcePtr;
    long SourceChunkBytesRemaining;
    void Read(void* buffer, long size)
    {
        if (size <= 0) return;
        if (size <= SourceChunkBytesRemaining) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
    }
    void ReadCompressed(void* data, long size)
    {
        if (size > 0) {
            short s; Read(&s, 2);
            long compressedSize = s;
            if (compressedSize == -1) { int l; Read(&l, 4); compressedSize = l; }
            if (compressedSize > 0) {
                unsigned char* buffer = (unsigned char*)std::malloc(compressedSize);
                Read(buffer, compressedSize);
                unsigned int written = size;
                Decompress(buffer, compressedSize, (unsigned char*)data, &written, 0);
                if (written != (unsigned int)(size - 3)) {
                    if (g_pfnAssertionFailed != 0) g_pfnAssertionFailed();
                }
                Read((unsigned char*)data + size - 3, 3);
                std::free(buffer);
            } else
                Read(data, size);
        }
    }
};

int main()
{
    unsigned char a[] = { 2, 0, 10, 20, 7, 8, 9 };          // count 2, two compressed bytes, three raw
    Stream s = { 0, a, sizeof a };
    unsigned char out[5] = { 0, 0, 0, 0, 0 };
    s.ReadCompressed(out, 5);
    if (out[0] != 11 || out[1] != 21 || out[2] != 7 || out[3] != 8 || out[4] != 9 || g_asserts != 0 || s.StreamPos != 7) {
        std::printf("00995fb0_TEST FAIL\n"); return 1;
    }
    unsigned char b[] = { 0xff, 0xff, 1, 0, 0, 0, 40, 5, 6, 7 };   // escaped count 1 -> one byte, size 5 expects 2: assert
    Stream t = { 0, b, sizeof b };
    s.StreamPos = 0;
    t.ReadCompressed(out, 5);
    if (out[0] != 41 || out[2] != 5 || out[4] != 7 || g_asserts != 1 || t.StreamPos != 10) {
        std::printf("00995fb0_TEST FAIL\n"); return 1;
    }
    unsigned char c[] = { 0, 0, 1, 2, 3 };                   // count 0: raw block
    Stream u = { 0, c, sizeof c };
    u.ReadCompressed(out, 3);
    if (out[0] != 1 || out[1] != 2 || out[2] != 3 || u.StreamPos != 5) { std::printf("00995fb0_TEST FAIL\n"); return 1; }
    u.ReadCompressed(out, 0);
    if (u.StreamPos != 5) { std::printf("00995fb0_TEST FAIL\n"); return 1; }
    std::printf("00995fb0_TEST PASS\n");
    return 0;
}
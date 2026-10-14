#include <cstdio>
static int g_base = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CDataInputStream : CBase {
    unsigned long StreamPos; long StreamSize; void* CurrentSourcePtr; unsigned long SourceChunkStreamPos; long SourceChunkBytesRemaining;
    CDataInputStream(void* src, long size, long rem) : CBase(), StreamPos(0), StreamSize(size), CurrentSourcePtr(src), SourceChunkStreamPos(0), SourceChunkBytesRemaining(rem) { }
};
int main()
{
    char buf[16]; CDataInputStream s(buf, 16, 12);
    if (g_base != 1 || s.StreamPos != 0 || s.StreamSize != 16 || s.CurrentSourcePtr != buf || s.SourceChunkStreamPos != 0 || s.SourceChunkBytesRemaining != 12) { std::printf("00993b50_TEST FAIL\n"); return 1; }
    std::printf("00993b50_TEST PASS\n"); return 0;
}

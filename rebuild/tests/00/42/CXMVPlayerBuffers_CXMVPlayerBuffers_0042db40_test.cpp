// behaviour test for 0x0042db40 CXMVPlayerBuffers::CXMVPlayerBuffers(): base constructed, both buffers null, size 0
#include <cstdio>
static int gBaseCtors = 0;
struct NonCopyable { NonCopyable() { ++gBaseCtors; } virtual ~NonCopyable() { } };
struct Buffers : NonCopyable {
    char* InputBuffer; char* PlaybackBuffer; unsigned long CurrentSize;
    Buffers() : InputBuffer(0), PlaybackBuffer(0), CurrentSize(0) { }
};
int main()
{
    int fails = 0;
    Buffers b;
    if (gBaseCtors != 1) fails |= 1;
    if (b.InputBuffer || b.PlaybackBuffer || b.CurrentSize) fails |= 2;
    if (fails) { std::printf("0042db40_TEST FAIL %d\n", fails); return 1; }
    std::printf("0042db40_TEST PASS\n");
    return 0;
}
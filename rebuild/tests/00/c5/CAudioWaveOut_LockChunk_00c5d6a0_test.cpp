// Behaviour test for 0x00C5D6A0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct WAVEHDR_32 { char* lpData; unsigned char _body[0x1c]; };
struct W { WAVEHDR_32* Headers; int WriteIndex; void* LockChunk() { return Headers[WriteIndex].lpData; } };
int main()
{
    static char a[4], b[4], c[4]; WAVEHDR_32 h[3]; h[0].lpData = a; h[1].lpData = b; h[2].lpData = c;
    W w; w.Headers = h; w.WriteIndex = 2;
    if (w.LockChunk() != c) { std::printf("00c5d6a0_TEST FAIL\n"); return 1; }
    w.WriteIndex = 0;
    if (w.LockChunk() != a) { std::printf("00c5d6a0_TEST FAIL\n"); return 1; }
    std::printf("00c5d6a0_TEST PASS\n");
    return 0;
}
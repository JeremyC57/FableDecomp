#include <cstdio>
// Behaviour model of CTCSolidToThing::FrameUpdate (0x00770E80): on frames where (uid + frame) % 75 == 0 the null entries are removed
// from the not-solid-to-things list (stable remove + erase).
static int Strip(const void** v, int n, unsigned long long uid, long frame)
{
    if ((uid + frame) % 75 != 0 || n == 0) return n;
    int out = 0;
    for (int i = 0; i < n; ++i) if (v[i]) v[out++] = v[i];
    return out;
}
int main()
{
    int a, b;
    const void* v[5] = { &a, 0, &b, 0, 0 };
    int n = Strip(v, 5, 70ULL, 4);       // 74: untouched
    if (n != 5) { std::printf("00770e80_TEST FAIL\n"); return 1; }
    n = Strip(v, 5, 70ULL, 5);           // 75: stripped
    if (n != 2 || v[0] != &a || v[1] != &b) { std::printf("00770e80_TEST FAIL\n"); return 1; }
    std::printf("00770e80_TEST PASS\n");
    return 0;
}

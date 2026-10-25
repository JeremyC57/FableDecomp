// Behaviour test for 00c335c0: AudioSystem_Query -- forwards (a, b) to the audio system's bool query and returns 1/0
#include <cstdio>
struct CAudioSystem {
    long lastA, lastB;
    bool Query(long a, long b) { lastA = a; lastB = b; return a > b; }
};
static CAudioSystem g_audioSystem;
static int AudioSystem_Query(long a, long b) { return g_audioSystem.Query(a, b) ? 1 : 0; }
int main()
{
    if (AudioSystem_Query(5, 3) != 1 || g_audioSystem.lastA != 5 || g_audioSystem.lastB != 3) { std::printf("00c335c0_TEST FAIL\n"); return 1; }
    if (AudioSystem_Query(1, 3) != 0 || g_audioSystem.lastA != 1) { std::printf("00c335c0_TEST FAIL\n"); return 1; }
    std::printf("00c335c0_TEST PASS\n");
    return 0;
}
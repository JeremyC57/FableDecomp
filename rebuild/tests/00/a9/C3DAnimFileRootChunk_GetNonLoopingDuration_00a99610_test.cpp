#include <cstdio>
#include <cmath>
// (frames - 1) / samplesPerSecond with the frame count taken as unsigned.
static float Duration(unsigned int frames, float samplesPerSecond) { float d = ((float)frames - 1.0f) / samplesPerSecond; return d; }
int main() {
    if (std::fabs(Duration(31, 30.0f) - 1.0f) > 1e-6f || std::fabs(Duration(1, 15.0f)) > 1e-6f || Duration(0, 1.0f) != -1.0f) { std::printf("00a99610_TEST FAIL\n"); return 1; }
    std::printf("00a99610_TEST PASS\n"); return 0; }
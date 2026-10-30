// Behaviour test for libvorbis sharedbook.c _dist (0x00C95910): sum of squared differences against a strided vector.
#include <cstdio>
static float _dist(int el, float* ref, float* b, int step)
{
    float acc = 0.f;
    for (int i = 0; i < el; i++) { float val = ref[i] - b[i * step]; acc += val * val; }
    return acc;
}
int main()
{
    float ref[3] = { 1.0f, 2.0f, 3.0f }, b[6] = { 0.0f, 9.0f, 2.0f, 9.0f, 5.0f, 9.0f };
    if (_dist(3, ref, b, 2) != 1.0f + 0.0f + 4.0f || _dist(0, ref, b, 1) != 0.0f) { std::printf("00c95910_TEST FAIL\n"); return 1; }
    std::printf("00c95910_TEST PASS\n"); return 0;
}
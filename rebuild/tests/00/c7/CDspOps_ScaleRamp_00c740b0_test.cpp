#include <cstdio>
// CDspOps::ScaleRamp: buffer[i] *= gain; gain += step.
static void ScaleRamp(float* buffer, float gain, float step, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i) {
        *buffer++ *= gain;
        gain += step;
    }
}
int main()
{
    float b[6] = { 1, 1, 1, 1, 1, 1 };
    ScaleRamp(b, 0.0f, 0.25f, 5);
    for (int i = 0; i < 5; ++i) if (b[i] != 0.25f * i) { std::printf("00c740b0_TEST FAIL\n"); return 1; }
    if (b[5] != 1.0f) { std::printf("00c740b0_TEST FAIL\n"); return 1; }
    std::printf("00c740b0_TEST PASS\n");
    return 0;
}
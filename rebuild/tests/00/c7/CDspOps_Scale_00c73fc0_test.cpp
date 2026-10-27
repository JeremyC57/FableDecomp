#include <cstdio>
// CDspOps::Scale: buffer[i] *= gain over count samples (unrolled by four in retail).
static void Scale(float* buffer, float gain, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
        *buffer++ *= gain;
}
int main()
{
    float b[7] = { 1, 2, 3, 4, 5, 6, 7 };
    Scale(b, 2.0f, 6);
    for (int i = 0; i < 6; ++i) if (b[i] != 2.0f * (i + 1)) { std::printf("00c73fc0_TEST FAIL\n"); return 1; }
    if (b[6] != 7.0f) { std::printf("00c73fc0_TEST FAIL\n"); return 1; }
    Scale(b, 0.5f, 0);
    if (b[0] != 2.0f) { std::printf("00c73fc0_TEST FAIL\n"); return 1; }
    std::printf("00c73fc0_TEST PASS\n");
    return 0;
}
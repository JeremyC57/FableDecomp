// Behaviour test for 0x00C76880 CDspOps::Biquad: the 1-offset direct form II biquad equals the plain biquad.
#include <cstdio>
#include <cmath>
static void Biquad(float* dst, const float* src, const float* feedback, const float* feedforward, float* state, unsigned int count)
{
    float a1 = feedback[1];
    float a2 = feedback[2];
    float b0 = feedforward[0];
    float b1 = feedforward[1];
    float b2 = feedforward[2];
    float inBias = 1.0f - (a2 + a1);
    float outBias = -(b2 + b1 + b0);
    float s1 = state[1];
    float s0 = state[0];
    for (unsigned int i = 0; i < count; i++) {
        float w = s1 * a2 + s0 * a1 + *src++ + inBias;
        *dst++ = s1 * b2 + s0 * b1 + w * b0 + outBias;
        s1 = s0;
        s0 = w;
    }
    state[0] = s0;
    state[1] = s1;
}
int main()
{
    const float fb[3] = { 0.0f, 0.5f, -0.25f }, ff[3] = { 0.3f, 0.2f, 0.1f };
    float src[11], dst[11], state[2] = { 1.0f, 1.0f };   // rest = 1.0 (offset state)
    for (int i = 0; i < 11; i++) src[i] = (float)((i * 7) % 5) - 2.0f;
    Biquad(dst, src, fb, ff, state, 11);
    double w1 = 0, w2 = 0;
    for (int i = 0; i < 11; i++) {
        double w = src[i] + fb[1] * w1 + fb[2] * w2;
        double y = ff[0] * w + ff[1] * w1 + ff[2] * w2;
        w2 = w1; w1 = w;
        if (std::fabs(y - dst[i]) > 1e-4) { std::printf("00c76880_TEST FAIL\n"); return 1; }
    }
    if (std::fabs(state[0] - (w1 + 1.0)) > 1e-4 || std::fabs(state[1] - (w2 + 1.0)) > 1e-4) { std::printf("00c76880_TEST FAIL\n"); return 1; }
    std::printf("00c76880_TEST PASS\n");
    return 0;
}

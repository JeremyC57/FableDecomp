// CReverbFns::ApplyFilterOrder2(float*, const float*, const float*, const float*, float*, unsigned int) -- runs a second-order IIR block with offset state.
// Leaf: ?ApplyFilterOrder2@CReverbFns@@UAEXPAMPBM110I@Z
// Flags: default
#include <stddef.h>
#include "engine/CReverbFns.h"
void CReverbFns::ApplyFilterOrder2(float* dst, const float* src, const float* feedback, const float* feedforward, float* state,
                                     unsigned int count)
{
    float a1 = feedback[1];
    float a2 = feedback[2];
    float b0 = feedforward[0];
    float b1 = feedforward[1];
    float b2 = feedforward[2];
    // State is stored with a +1 offset in the original filter.
    float s1 = state[1];
    float inBias = 1.0f - (a2 + a1);
    float s0 = state[0];
    float outBias = -(b2 + b1 + b0);
    for (unsigned int i = 0; i < count; ++i) {
        float oldest = s1;
        s1 = s0;
        s0 = oldest * a2 + s1 * a1 + *src++ + inBias;
        *dst++ = oldest * b2 + s1 * b1 + s0 * b0 + outBias;
    }
    state[0] = s0;
    state[1] = s1;
}

#include <math.h>
#include <stdio.h>

CReverbFns::eReverbType CReverbFns::Type() { return eReverb_Float; }
void CReverbFns::ScaleSamples(float*, float, unsigned int) {}
void CReverbFns::ScaleSamples(float*, const float*, float, unsigned int) {}
void CReverbFns::ScaleSamples(float*, float, float, unsigned int) {}
void CReverbFns::ScaleSamples(float*, const float*, float, float, unsigned int) {}
void CReverbFns::AddSamples(float*, const float*, unsigned int) {}
void CReverbFns::ScaleAddSamples(float*, const float*, float, unsigned int) {}
void CReverbFns::ScaleAddSamples(float*, const float*, float, float, unsigned int) {}

static bool check(unsigned int count, float older, float newer, bool in_place)
{
    const float feedback[3] = {0.0f, 0.5f, -0.25f};
    const float feedforward[3] = {0.3f, 0.2f, 0.1f};
    float state[2] = {newer + 1.0f, older + 1.0f};
    float input[12];
    float output[12];
    float original[12];
    for (unsigned int i = 0; i < 12; ++i) {
        original[i] = input[i] = (float)((i * 7) % 5) - 2.0f;
        output[i] = 99.0f;
    }
    float* result = in_place ? input : output;
    CReverbFns reverb;
    reverb.ApplyFilterOrder2(result, input, feedback, feedforward, state, count);
    double w1 = newer;
    double w2 = older;
    for (unsigned int i = 0; i < count; ++i) {
        const double w = original[i] + feedback[1] * w1 + feedback[2] * w2;
        const double y = feedforward[0] * w + feedforward[1] * w1 + feedforward[2] * w2;
        if (fabs(y - result[i]) > 0.0001)
            return false;
        w2 = w1;
        w1 = w;
    }
    if (count < 12 && result[count] != (in_place ? original[count] : 99.0f))
        return false;
    return fabs(state[0] - (w1 + 1.0)) < 0.0001 && fabs(state[1] - (w2 + 1.0)) < 0.0001;
}

int main()
{
    if (!check(0, 0.5f, -0.5f, false)
        || !check(1, 0.0f, 0.0f, false)
        || !check(3, -0.75f, 0.5f, false)
        || !check(4, 0.25f, 0.75f, false)
        || !check(5, 0.0f, 0.0f, false)
        || !check(11, 0.0f, 0.0f, true))
        return 1;
    puts("00c76880_TEST PASS");
    return 0;
}
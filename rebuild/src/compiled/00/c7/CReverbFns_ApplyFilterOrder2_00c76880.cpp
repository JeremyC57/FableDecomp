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
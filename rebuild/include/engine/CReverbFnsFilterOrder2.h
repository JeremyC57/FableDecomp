#ifndef FABLE_ENGINE_CREVERBFNSFILTERORDER2_H
#define FABLE_ENGINE_CREVERBFNSFILTERORDER2_H
// Plain C++ for the portable build. State holds the two most recent internal samples plus one.
#include "engine/CReverbFns.h"
void CReverbFns::ApplyFilterOrder2(float* dst, const float* src, const float* feedback, const float* feedforward, float* state,
                     unsigned int count)
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
#endif

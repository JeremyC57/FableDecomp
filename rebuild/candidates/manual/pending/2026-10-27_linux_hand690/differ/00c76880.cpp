// CDspOps::Biquad(float* dst, const float* src, const float* feedback, const float* feedforward, float* state, unsigned int count) -- second-order IIR over a block (direct form II, state kept 1-offset against denormals): w = x + a1*s0 + a2*s1 + (1 - a1 - a2), y = b0*w + b1*s0 + b2*s1 - (b0 + b1 + b2)
// Grade: DIFFER(500v513) -- x87 register allocation of the two-pole state: retail keeps s0 (state[0]) in a frame slot (integer copy in / out, fstp each sample) and carries s1 on the x87 stack, VC7.1 keeps s0 on the stack in every spelling (14 probes; the same mirror chunk 684 parked in CDspGain::ApplyRamps after 37 probes). next: lane gap: x87 two-pole state allocation.
// Leaf: ?Biquad@CDspOps@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C76880, 513 bytes, virtual __thiscall `ret 18h` (this unused, six stack arguments), frame 0x14.
// Identity: CDspOps vtable slot 8 (+0x20): 0x012BF04C in the x87 table 0x012BF02C and 0x012BF214 in the SIMD table 0x012BF1F4
//   (both tables share this kernel, landed CreateSharedOps 0x00C74360); its one caller CDspGain::ApplyRamps 0x00C74510 (chunk
//   684) calls `s_pOps->Biquad(block, source, feedback, feedforward, state, count)` for the second-order case. libpk has no
//   PDB/RTTI: descriptive name (chunk 684's), the landed models declare the slot as the stand-in `v08()`.
// Shape: feedback[1..2] and feedforward[0..2] read once; the 1-offset biases 1.0f (0x0122DED8) - (a2 + a1) and -(b2 + b1 + b0);
//   the pointer walk gives retail's eax/ecx += 0x10 4x-unrolled body with the remainder loop.
class CDspOps {
public:
    virtual long GetKind();
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);
    virtual void Scale(float* buffer, float gain, unsigned int count);
    virtual void Add(float* dst, const float* src, unsigned int count);
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count);
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);
    virtual void Biquad(float* dst, const float* src, const float* feedback, const float* feedforward, float* state,
                        unsigned int count);
};

void CDspOps::Biquad(float* dst, const float* src, const float* feedback, const float* feedforward, float* state,
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

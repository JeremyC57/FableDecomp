// CDspOps::MixRamp(float* dst, const float* src, float gain, float step, unsigned int count) -- accumulate a source float block scaled by a linearly ramped gain (gain, gain+step, ...) into the destination block
// Leaf: ?MixRamp@CDspOps@@UAEXPAMPBMMMI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C742C0, thiscall (this unused). No callees.
//   Slot 6 of 0x012BF02C (xref 0x012BF044).
// Identity: the x87 CDspOps table 0x012BF02C (landed CreateSharedOps 0x00C74360 builds it; the SIMD twin 0x012BF1F4 holds
//   CMixKernelsSSE 0x00C88A40..0x00C88D80 in the same slots). The slot names are the landed descriptive kernel names of the
//   CDspOps model (landed 0x00C73FC0..0x00C741C0 declare this very slot); libpk has no PDB/RTTI. Catalogued as gap-scan.
// Shape: a counted loop over post-incremented pointers (`*p++ op= x` gives retail's bumped [eax-10h].. operands); VC7.1 /O2
//   unrolls it by four (count>=4 block, then the remainder loop).
class CDspOps {                                         // vtable 0x012BF02C: libpk's x87 block kernels (landed CreateSharedOps 0x00C74360)
public:
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);   // 0x00C74130
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);             // 0x00C740B0
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);          // 0x00C74030
    virtual void Scale(float* buffer, float gain, unsigned int count);                             // 0x00C73FC0
    virtual void Add(float* dst, const float* src, unsigned int count);                            // 0x00C741C0
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // 0x00C742C0
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // 0x00C74230
    virtual void v08();                                                                             // 0x00C76880
};

void CDspOps::MixRamp(float* dst, const float* src, float gain, float step, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i) {
        *dst++ += *src++ * gain;
        gain += step;
    }
}
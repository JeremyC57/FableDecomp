// CDspOps::ScaleRamp(float* buffer, float gain, float step, unsigned int count) -- multiply a float block in place by a linearly ramped gain (gain, gain+step, ...)
// Leaf: ?ScaleRamp@CDspOps@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C740B0, 120 bytes, thiscall (this unused), ret 10h. No callees.
//   Slot 2 of 0x012BF02C (xref 0x012BF034).
// Identity: the x87 CDspOps table 0x012BF02C (landed CreateSharedOps 0x00C74360 builds it; the SIMD twin 0x012BF1F4 holds
//   CMixKernelsSSE 0x00C88A40..0x00C88D80 in the same slots). Names are the landed descriptive kernel names (libpk: no
//   PDB/RTTI); the CDspGain::Apply caller 0x00C74CE0 calls slot 4 as Scale(block, master, count).
// Shape: a counted loop over a post-incremented pointer; the running gain stays on the x87 stack (fld st(0) per sample, fstp st(0) at the end); unrolled by four.
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

void CDspOps::ScaleRamp(float* buffer, float gain, float step, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i) {
        *buffer++ *= gain;
        gain += step;
    }
}
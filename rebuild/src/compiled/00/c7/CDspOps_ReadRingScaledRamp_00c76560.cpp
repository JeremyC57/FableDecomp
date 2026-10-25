// CDspOps::ReadRingScaledRamp(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count) -- read `count` floats out of a delay ring at `pos` under a linear gain ramp (slot 1 ScaleRampCopy), wrapping at the end of the buffer; the wrapped run starts at gain + first * step
// Leaf: ?ReadRingScaledRamp@CDspOps
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76560, 148 bytes, ret 18h. Callee: virtual slot 1 (+4). Callers 0x00C758A7, 0x00C76CAA, 0x00C893E1.
// Layout (descriptive; libpk reverb module, no PDB/RTTI): CDspOps is the shared vector-maths table the reverb parts
//   reach through CDspObject::s_pOps (0x0143CCDC, built by 0x00C74360: the x87 class, vtable 0x012BF02C, or a SIMD
//   class when the CPU flag 0x0143BC7C is set). Its slots are block kernels over `count` floats (1 ScaleRampCopy,
//   2 ScaleRamp, 3 ScaleCopy, 4 Scale, 5 Add, 6 MixRamp, 7 Mix); the non-virtual members below wrap one kernel
//   around a CRingBuffer {data, size, pos}: one call when [pos, pos+count) fits, else the tail run and the
//   wrapped head run.
// Shape: the tail length is a SIGNED int (plain `fild`, no 2^32 fix-up), homed in the dead `ring` argument slot for the
//   fild; `gain + first * step` is evaluated right operand first (fild ; fmul step ; fadd gain).
struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
};

struct CDspOps {                           // x87 vtable 0x012BF02C (SIMD build: ctor 0x00C88A30)
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);   // 0x00C74130
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);             // 0x00C740B0
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);          // 0x00C74030
    virtual void Scale(float* buffer, float gain, unsigned int count);                             // 0x00C73FC0
    virtual void Add(float* dst, const float* src, unsigned int count);                            // 0x00C741C0
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // 0x00C742C0
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // 0x00C74230
    void ReadRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count);
    void WriteRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count);
    void ReadRingScaled(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, unsigned int count);
    void ReadRingScaledRamp(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count);
    void AddFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count);
    void AddToRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count);
    void MixFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, unsigned int count);
    void MixRampFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count);
};

void CDspOps::ReadRingScaledRamp(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count)
{
    if (pos + count <= ring->m_size)
        ScaleRampCopy(dst, &ring->m_data[pos], gain, step, count);
    else {
        int first = ring->m_size - pos;
        ScaleRampCopy(dst, &ring->m_data[pos], gain, step, first);
        ScaleRampCopy(dst + first, ring->m_data, gain + first * step, step, count - first);
    }
}
// CDspOps::WriteRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count) -- copy `count` floats into a delay ring starting at `pos`, wrapping at the end of the buffer
// Leaf: ?WriteRing@CDspOps
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76460, 113 bytes, ret 10h. No callees (memcpy intrinsic); `this` is unused. Callers 0x00C6A158, 0x00C75958, 0x00C760D4, 0x00C76BB2, 0x00C76CF6.
// Layout (descriptive; libpk reverb module, no PDB/RTTI): CDspOps is the shared vector-maths table the reverb parts
//   reach through CDspObject::s_pOps (0x0143CCDC, built by 0x00C74360: the x87 class, vtable 0x012BF02C, or a SIMD
//   class when the CPU flag 0x0143BC7C is set). Its slots are block kernels over `count` floats (1 ScaleRampCopy,
//   2 ScaleRamp, 3 ScaleCopy, 4 Scale, 5 Add, 6 MixRamp, 7 Mix); the non-virtual members below wrap one kernel
//   around a CRingBuffer {data, size, pos}: one call when [pos, pos+count) fits, else the tail run and the
//   wrapped head run.
// Shape: the mirror of ReadRing 0x00C763E0.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

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

void CDspOps::WriteRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count)
{
    if (pos + count <= ring->m_size)
        memcpy(&ring->m_data[pos], src, count * sizeof(float));
    else {
        unsigned int first = ring->m_size - pos;
        memcpy(&ring->m_data[pos], src, first * sizeof(float));
        memcpy(ring->m_data, src + first, (count - first) * sizeof(float));
    }
}
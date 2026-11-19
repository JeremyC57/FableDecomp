#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
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

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
    static long s_refCount;                // 0x0143CCE0
};

struct SReverbDelayParams {
    unsigned int m_delay;                  // +0x00 samples
    float m_feedback;                      // +0x04
    float m_wet;                           // +0x08
};

struct CReverbDelay : CDspObject {         // vtable 0x012B957C
    CRingBuffer m_ring;                    // +0x04
    unsigned int m_index;                  // +0x10
    void Process(const SReverbDelayParams* params, const float* input, float* output, unsigned int count);
};

inline void AdvanceRing(CRingBuffer* ring, unsigned int count)
{
    ring->m_pos += count;
    if (ring->m_pos >= ring->m_size)
        ring->m_pos -= ring->m_size;
}

void CReverbDelay::Process(const SReverbDelayParams* params, const float* input, float* output, unsigned int count)
{
    __declspec(align(16)) float delayed[128];
    unsigned int pos = m_ring.m_pos;
    unsigned int read;
    if (pos >= params->m_delay)
        read = pos - params->m_delay;
    else
        read = m_ring.m_size - params->m_delay + pos;
    s_pOps->ReadRingScaled(delayed, &m_ring, read, params->m_feedback, count);
    s_pOps->WriteRing(&m_ring, pos, input, count);
    s_pOps->AddToRing(&m_ring, pos, delayed, count);
    if (params->m_wet != 0.0f)
        s_pOps->Mix(output, delayed, params->m_wet, count);
    AdvanceRing(&m_ring, count);
}
static int fail() { std::printf("00c76b60_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
long CDspOps::GetKind() { return 0; }
void CDspOps::ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count)
{ for (unsigned int i = 0; i < count; ++i) dst[i] = src[i] * (gain + i * step); }
void CDspOps::ScaleRamp(float* buffer, float gain, float step, unsigned int count)
{ for (unsigned int i = 0; i < count; ++i) buffer[i] *= gain + i * step; }
void CDspOps::ScaleCopy(float* dst, const float* src, float gain, unsigned int count)
{ for (unsigned int i = 0; i < count; ++i) dst[i] = src[i] * gain; }
void CDspOps::Scale(float* buffer, float gain, unsigned int count)
{ for (unsigned int i = 0; i < count; ++i) buffer[i] *= gain; }
void CDspOps::Add(float* dst, const float* src, unsigned int count)
{ for (unsigned int i = 0; i < count; ++i) dst[i] += src[i]; }
void CDspOps::MixRamp(float* dst, const float* src, float gain, float step, unsigned int count)
{ for (unsigned int i = 0; i < count; ++i) dst[i] += src[i] * (gain + i * step); }
void CDspOps::Mix(float* dst, const float* src, float gain, unsigned int count)
{ for (unsigned int i = 0; i < count; ++i) dst[i] += src[i] * gain; }
CDspObject::CDspObject() { }
CDspObject::~CDspObject() { }

void CDspOps::ReadRingScaled(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, unsigned int count)
{
    if (pos + count <= ring->m_size)
        ScaleCopy(dst, &ring->m_data[pos], gain, count);
    else {
        unsigned int first = ring->m_size - pos;
        ScaleCopy(dst, &ring->m_data[pos], gain, first);
        ScaleCopy(dst + first, ring->m_data, gain, count - first);
    }
}

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

void CDspOps::AddToRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count)
{
    if (pos + count <= ring->m_size)
        Add(&ring->m_data[pos], src, count);
    else {
        unsigned int first = ring->m_size - pos;
        Add(&ring->m_data[pos], src, first);
        Add(ring->m_data, src + first, count - first);
    }
}
CDspOps* CDspObject::s_pOps = 0;
long CDspObject::s_refCount = 0;
static int run(unsigned int pos, unsigned int delay, float wet)
{
    float data[8], before[8], input[4] = { 10, 20, 30, 40 }, output[5] = { 1, 1, 1, 1, 7 };
    for (int k = 0; k < 8; ++k) before[k] = data[k] = 1.0f + k;
    CReverbDelay line;
    line.m_ring.m_data = data; line.m_ring.m_size = 8; line.m_ring.m_pos = pos;
    SReverbDelayParams params; params.m_delay = delay; params.m_feedback = 0.5f; params.m_wet = wet;
    line.Process(&params, input, output, 4);
    unsigned int read = (pos + 8 - delay) % 8;
    for (unsigned int i = 0; i < 4; ++i) {
        float delayed = before[(read + i) % 8] * 0.5f;
        if (!nearly(data[(pos + i) % 8], input[i] + delayed)) return 0;
        if (!nearly(output[i], 1.0f + (wet != 0.0f ? delayed * wet : 0.0f))) return 0;
    }
    if (output[4] != 7) return 0;
    if (line.m_ring.m_pos != (pos + 4) % 8) return 0;
    return 1;
}
int main()
{
    CDspOps ops; CDspObject::s_pOps = &ops;
    if (!run(2, 3, 0.25f)) return fail();      // read position wraps below zero
    if (!run(6, 6, 0.25f)) return fail();      // ring wraps on write, position wraps past the end
    if (!run(5, 1, 0.0f)) return fail();       // dry: the output is left alone
    std::printf("00c76b60_TEST PASS\n");
    return 0;
}
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
static int fail() { std::printf("00c76560_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
int main()
{
    CDspOps ops;
    for (int c = 0; c < 3; ++c) {
        unsigned int pos = c == 0 ? 1 : c == 1 ? 3 : 6;   // [1,6) and [3,8) fit, [6,11) wraps
        unsigned int count = 5;
        float data[8], dst[8], src[8];
        for (unsigned int k = 0; k < 8; ++k) { data[k] = 1.0f + k; dst[k] = 100.0f + k; src[k] = 10.0f + k; }
        CRingBuffer ring;
        ring.m_data = data; ring.m_size = 8; ring.m_pos = 0;
        ops.ReadRingScaledRamp(dst, &ring, pos, 0.5f, 0.25f, count);
        for (unsigned int i = 0; i < 8; ++i) {
            float dst0 = 100.0f + i;
            if (i >= count) { if (!nearly(dst[i], dst0)) return fail(); continue; }
            float s = 1.0f + (pos + i) % 8;
            if (!nearly(dst[i], s * (0.5f + i * 0.25f))) return fail();
        }
        for (unsigned int k = 0; k < 8; ++k)
            if (!nearly(data[k], 1.0f + k)) return fail();
        if (ring.m_pos != 0 || ring.m_size != 8 || ring.m_data != data) return fail();
    }
    std::printf("00c76560_TEST PASS\n");
    return 0;
}
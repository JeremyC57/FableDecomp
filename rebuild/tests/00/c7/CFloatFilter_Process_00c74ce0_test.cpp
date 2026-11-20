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

struct SGainParams {
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
    void ApplyRamps(const float* current, const float* target, float* state, const float* source, float* block,
        int mode, unsigned int count);     // 0x00C74510
    void Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count);
};

void CDspGain::Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count)
{
    ApplyRamps(params->m_current, params->m_target, m_gain, source, block, mode, count);
    if (params->m_master != 1.0f)
        s_pOps->Scale(block, params->m_master, count);
}
static int fail() { std::printf("00c74ce0_TEST FAIL\n"); return 1; }
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
CDspOps* CDspObject::s_pOps = 0;
long CDspObject::s_refCount = 0;
static const float* g_current; static const float* g_target; static float* g_state; static int g_mode; static unsigned int g_count;
void CDspGain::ApplyRamps(const float* current, const float* target, float* state, const float* source, float* block, int mode, unsigned int count)
{
    g_current = current; g_target = target; g_state = state; g_mode = mode; g_count = count;
    for (unsigned int i = 0; i < count; ++i) block[i] = source[i] * current[0];
}
int main()
{
    CDspOps ops; CDspObject::s_pOps = &ops;
    SGainParams params;
    params.m_master = 1.0f;
    for (int i = 0; i < 3; ++i) { params.m_current[i] = 0.5f; params.m_target[i] = 0.75f; }
    float source[4] = { 1, 2, 3, 4 }, block[5] = { 0, 0, 0, 0, 9 };
    CDspGain gain;
    gain.Apply(&params, source, block, 3, 4);
    if (g_current != params.m_current || g_target != params.m_target || g_state != gain.m_gain || g_mode != 3 || g_count != 4) return fail();
    for (int i = 0; i < 4; ++i) if (!nearly(block[i], source[i] * 0.5f)) return fail();
    params.m_master = 2.0f;                                  // a master gain other than unity scales the block
    gain.Apply(&params, source, block, 1, 4);
    for (int i = 0; i < 4; ++i) if (!nearly(block[i], source[i])) return fail();
    if (block[4] != 9) return fail();
    std::printf("00c74ce0_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
};

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    float* m_data;                         // +0x18
    unsigned int m_size;                   // +0x1C
    unsigned int m_pos;                    // +0x20
    unsigned int m_index;                  // +0x24
    void Reset()
    {
        memset(m_data, 0, m_size * sizeof(float));
        m_gain.Reset();
    }
};

struct CReverbTapBank : CDspObject {       // vtable 0x012B9590
    CReverbFilterTap m_taps[4];            // +0x04
    unsigned int m_channels;               // +0xA4
    unsigned int m_firstIndex;             // +0xA8
    unsigned int m_endIndex;               // +0xAC
    void Reset();
};

void CReverbTapBank::Reset()
{
    for (int i = 0; i < 4; ++i)
        m_taps[i].Reset();
}
static int fail() { std::printf("00c75ca0_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
CDspObject::CDspObject() { }
CDspObject::~CDspObject() { }
void CDspGain::Reset() { for (int i = 0; i < 4; ++i) m_gain[i] = 1.0f; }
int main()
{
    static float data[4][5];
    CReverbTapBank bank;
    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 5; ++k) data[i][k] = 1.0f + k;
        bank.m_taps[i].m_data = data[i];
        bank.m_taps[i].m_size = 1 + i;          // tap i clears its first i+1 samples
        for (int g = 0; g < 4; ++g) bank.m_taps[i].m_gain.m_gain[g] = 0.25f;
    }
    bank.Reset();
    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 5; ++k)
            if (data[i][k] != (k <= i ? 0.0f : 1.0f + k)) return fail();
        for (int g = 0; g < 4; ++g) if (bank.m_taps[i].m_gain.m_gain[g] != 1.0f) return fail();
    }
    std::printf("00c75ca0_TEST PASS\n");
    return 0;
}
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

struct CReverbDiffuserStage : CDspObject { // vtable 0x012BF07C
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDspGain m_gain;                       // +0x10
    unsigned int m_index;                  // +0x24
    void Reset()
    {
        memset(m_data, 0, m_size * sizeof(float));
        m_gain.Reset();
    }
};

struct CReverbConfig;

struct CReverbDiffuser : CDspObject {      // vtable 0x012BF080
    CDspGain m_gains[3];                   // +0x04
    CReverbDiffuserStage m_stages[2];      // +0x40
    CReverbConfig* m_pConfig;              // +0x90
    unsigned int m_firstIndex;             // +0x94
    unsigned int m_endIndex;               // +0x98
    void Reset();
};

void CReverbDiffuser::Reset()
{
    for (int i = 0; i < 3; ++i)
        m_gains[i].Reset();
    for (int j = 0; j < 2; ++j)
        m_stages[j].Reset();
}
static int fail() { std::printf("00c75670_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
CDspObject::CDspObject() { }
CDspObject::~CDspObject() { }
void CDspGain::Reset() { for (int i = 0; i < 4; ++i) m_gain[i] = 1.0f; }
int main()
{
    static float d0[4] = { 1, 2, 3, 4 }, d1[4] = { 5, 6, 7, 8 };
    CReverbDiffuser diffuser;
    for (int i = 0; i < 3; ++i) for (int g = 0; g < 4; ++g) diffuser.m_gains[i].m_gain[g] = 0.5f;
    diffuser.m_stages[0].m_data = d0; diffuser.m_stages[0].m_size = 4; diffuser.m_stages[0].m_pos = 2;
    diffuser.m_stages[1].m_data = d1; diffuser.m_stages[1].m_size = 3; diffuser.m_stages[1].m_pos = 1;
    for (int j = 0; j < 2; ++j) for (int g = 0; g < 4; ++g) diffuser.m_stages[j].m_gain.m_gain[g] = 0.0f;
    diffuser.Reset();
    for (int i = 0; i < 3; ++i) for (int g = 0; g < 4; ++g) if (diffuser.m_gains[i].m_gain[g] != 1.0f) return fail();
    for (int j = 0; j < 2; ++j) for (int g = 0; g < 4; ++g) if (diffuser.m_stages[j].m_gain.m_gain[g] != 1.0f) return fail();
    for (int k = 0; k < 4; ++k) if (d0[k] != 0.0f) return fail();
    if (d1[0] != 0.0f || d1[1] != 0.0f || d1[2] != 0.0f || d1[3] != 8.0f) return fail();
    if (diffuser.m_stages[0].m_pos != 2 || diffuser.m_stages[1].m_pos != 1) return fail();   // Reset leaves the positions
    std::printf("00c75670_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
struct CDelayLine {
    virtual ~CDelayLine();                 // 0x00C75400
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDelayLine();
    void Clear()
    {
        if (m_data)
            memset(m_data, 0, m_size * sizeof(float));
        m_pos = 0;
    }
};

struct CStereoDelayLine {
    virtual ~CStereoDelayLine();
    CDelayLine m_lines[2];                 // +0x04
    void Clear();
};

void CStereoDelayLine::Clear()
{
    for (int i = 0; i < 2; ++i)
        m_lines[i].Clear();
}
static int fail() { std::printf("00c75010_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
CDelayLine::CDelayLine() : m_data(0), m_size(0), m_pos(0) { }
CDelayLine::~CDelayLine() { }
CStereoDelayLine::~CStereoDelayLine() { }
int main()
{
    float a[6] = { 1, 2, 3, 4, 5, 6 };
    CStereoDelayLine pair;
    pair.m_lines[0].m_data = a; pair.m_lines[0].m_size = 4; pair.m_lines[0].m_pos = 3;
    pair.m_lines[1].m_data = 0; pair.m_lines[1].m_size = 9; pair.m_lines[1].m_pos = 2;   // no buffer yet
    pair.Clear();
    for (int i = 0; i < 4; ++i) if (a[i] != 0.0f) return fail();
    if (a[4] != 5.0f || a[5] != 6.0f) return fail();
    if (pair.m_lines[0].m_pos != 0 || pair.m_lines[1].m_pos != 0 || pair.m_lines[1].m_size != 9) return fail();
    std::printf("00c75010_TEST PASS\n");
    return 0;
}
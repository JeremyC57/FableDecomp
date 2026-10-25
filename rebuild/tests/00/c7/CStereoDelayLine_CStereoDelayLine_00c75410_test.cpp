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
    CDelayLine();                          // 0x00C74F60
};

struct CStereoDelayLine {
    virtual ~CStereoDelayLine();           // 0x00C75440
    CDelayLine m_lines[2];                 // +0x04
    CStereoDelayLine();
};

CStereoDelayLine::CStereoDelayLine()
{
}
static int fail() { std::printf("00c75410_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
static int g_lines = 0;
CDelayLine::CDelayLine() : m_data(0), m_size(0) { ++g_lines; }
CDelayLine::~CDelayLine() { }
CStereoDelayLine::~CStereoDelayLine() { }
int main()
{
    CStereoDelayLine pair;
    if (g_lines != 2) return fail();
    for (int i = 0; i < 2; ++i)
        if (pair.m_lines[i].m_data != 0 || pair.m_lines[i].m_size != 0) return fail();
    std::printf("00c75410_TEST PASS\n");
    return 0;
}
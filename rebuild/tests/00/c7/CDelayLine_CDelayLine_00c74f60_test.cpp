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
};

CDelayLine::CDelayLine()
    : m_data(0), m_size(0)
{
}
static int fail() { std::printf("00c74f60_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
#include <new>
CDelayLine::~CDelayLine() { }
static union { double align; unsigned char bytes[sizeof(CDelayLine)]; } g_storage;
int main()
{
    std::memset(g_storage.bytes, 0xab, sizeof(g_storage.bytes));
    CDelayLine* line = new (g_storage.bytes) CDelayLine;
    if (line->m_data != 0 || line->m_size != 0) return fail();
    if (line->m_pos != 0xababababu) return fail();       // the position is left for Allocate / Clear
    CDelayLine local;
    if (local.m_data != 0 || local.m_size != 0) return fail();
    std::printf("00c74f60_TEST PASS\n");
    return 0;
}
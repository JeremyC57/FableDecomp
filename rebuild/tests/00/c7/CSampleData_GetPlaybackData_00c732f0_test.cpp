#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
struct CSampleSource {
    unsigned long m_format;                // +0x00
    unsigned long m_channels;              // +0x04
    unsigned long m_08;
    unsigned long m_length;                // +0x0C
    unsigned long m_10;
    unsigned long m_14;
    void* m_pData;                         // +0x18
};

struct CSampleData {
    void* m_pSource;                       // +0x00
    void* m_pConverted;                    // +0x04
    unsigned char m_flags;                 // +0x08
    unsigned char _pad09[3];
    CSampleSource* m_pDescriptor;          // +0x0C
    long MakeStereo();                     // 0x00C4DE00
    long GetPlaybackData(void** data);
};

long CSampleData::GetPlaybackData(void** data)
{
    if (m_pDescriptor->m_channels == 2) {
        *data = m_pSource;
        return 0;
    }
    long result = 0;
    if (!(m_flags & 2))
        result = MakeStereo();
    *data = m_pConverted;
    return result;
}
static int fail() { std::printf("00c732f0_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
static int g_makes = 0; static long g_makeResult = 0;
long CSampleData::MakeStereo() { ++g_makes; m_flags |= 2; return g_makeResult; }

int main()
{
    int src, conv;
    CSampleSource desc; desc.m_format = 1; desc.m_channels = 2; desc.m_length = 0; desc.m_pData = 0;
    CSampleData sd; sd.m_pSource = &src; sd.m_pConverted = &conv; sd.m_flags = 0; sd.m_pDescriptor = &desc;
    void* data = 0;
    if (sd.GetPlaybackData(&data) != 0 || data != &src || g_makes != 0) return fail();       // already stereo
    desc.m_channels = 1; g_makeResult = 3;
    if (sd.GetPlaybackData(&data) != 3 || data != &conv || g_makes != 1) return fail();      // converted on first use
    if (sd.GetPlaybackData(&data) != 0 || data != &conv || g_makes != 1) return fail();      // then reused
    std::printf("00c732f0_TEST PASS\n");
    return 0;
}
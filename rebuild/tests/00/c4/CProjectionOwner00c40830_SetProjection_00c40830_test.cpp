#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
struct SProjectionBlock { unsigned long m_words[27]; };
static unsigned long g_renderFlags_0143bc28 = 0x10;
struct CProjectionOwner_00c40830 {
    unsigned long    m_head[3];
    SProjectionBlock m_projection;
    unsigned long    m_dirtyFlags;
    void SetProjection(const SProjectionBlock* projection)
    {
        m_projection = *projection;
        m_dirtyFlags |= 0xfffff;
        g_renderFlags_0143bc28 |= 0x1000000;
    }
};
int main()
{
    SProjectionBlock p; for (int i = 0; i < 27; ++i) p.m_words[i] = 100 + i;
    CProjectionOwner_00c40830 o; o.m_dirtyFlags = 0x80000000u; o.m_head[2] = 77;
    o.SetProjection(&p);
    for (int i = 0; i < 27; ++i) if (o.m_projection.m_words[i] != (unsigned long)(100 + i)) { std::printf("00c40830_TEST FAIL\n"); return 1; }
    if (o.m_dirtyFlags != 0x800fffffu || g_renderFlags_0143bc28 != 0x1000010u || o.m_head[2] != 77) { std::printf("00c40830_TEST FAIL\n"); return 1; }
    std::printf("00c40830_TEST PASS\n");
    return 0;
}
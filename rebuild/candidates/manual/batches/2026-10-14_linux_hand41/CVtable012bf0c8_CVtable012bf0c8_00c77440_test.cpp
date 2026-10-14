#include <cstdio>
#include <cstring>
#include <new>

static int g_baseRuns = 0;
struct CVtable_012bf050 { virtual ~CVtable_012bf050() {} CVtable_012bf050() { ++g_baseRuns; } };
struct CVtable_012bf0c8 : CVtable_012bf050 {
    long m_first; long m_second; long m_third; long m_count; bool m_active; unsigned char _pad15[0xf]; bool m_dirty;
    CVtable_012bf0c8() { m_count = 0; m_first = 0; m_second = 0; m_third = 0; m_active = false; m_dirty = false; }
};
int main()
{
    unsigned char raw[sizeof(CVtable_012bf0c8)]; std::memset(raw, 0xaa, sizeof raw);
    CVtable_012bf0c8* o = new (raw) CVtable_012bf0c8;
    if (g_baseRuns != 1 || o->m_first || o->m_second || o->m_third || o->m_count || o->m_active || o->m_dirty) { std::printf("00c77440_TEST FAIL\n"); return 1; }
    o->~CVtable_012bf0c8();
    std::printf("00c77440_TEST PASS\n");
    return 0;
}

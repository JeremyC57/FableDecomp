#include <cstdio>
#include <new>
static int g_base = 0;
struct TeleportData_00c4d000 { virtual void v00() { } unsigned char _members[0x54 - 4]; TeleportData_00c4d000() { ++g_base; } };
struct CTeleportState_00c3e520 : TeleportData_00c4d000 {
    unsigned char _pad54[4]; long m_pending; unsigned char _pad5c[0x174 - 0x5c]; long m_a, m_b, m_c, m_d, m_e, m_f; unsigned char _pad18c[0x1a4 - 0x18c];
    long m_g, m_h; unsigned char _pad1ac[0x1b8 - 0x1ac]; long m_i, m_j; unsigned char _pad1c0[0x1d4 - 0x1c0]; long m_k;
    CTeleportState_00c3e520() : TeleportData_00c4d000() { m_g = 0; m_h = 0; m_d = 0; m_e = 0; m_f = 0; m_i = 0; m_j = 0; m_pending = 0; m_c = 0; m_k = 0; m_a = 0; m_b = 0; }
};
int main()
{
    unsigned char raw[sizeof(CTeleportState_00c3e520)]; for (unsigned i = 0; i < sizeof raw; ++i) raw[i] = 0xee;
    CTeleportState_00c3e520* s = new (raw) CTeleportState_00c3e520;
    if (g_base != 1 || s->m_pending || s->m_a || s->m_b || s->m_c || s->m_d || s->m_e || s->m_f || s->m_g || s->m_h || s->m_i || s->m_j || s->m_k) { std::printf("00c3e520_TEST FAIL\n"); return 1; }
    std::printf("00c3e520_TEST PASS\n"); return 0;
}
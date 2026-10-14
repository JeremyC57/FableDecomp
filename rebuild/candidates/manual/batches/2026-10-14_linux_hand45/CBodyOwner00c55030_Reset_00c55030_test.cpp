#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static void* g_reset[2]; static int g_n = 0;
struct CReleasable_00c55030 { virtual void Release() { std::strcat(g_seq, "R"); } };
struct SBodyRecord_00c55030 { unsigned char _body[0x24]; void Reset() { std::strcat(g_seq, "B"); g_reset[g_n++] = this; } };
struct CBodyOwner_00c55030 { unsigned char _pad[0xdc]; SBodyRecord_00c55030 m_bodyA; SBodyRecord_00c55030 m_bodyB; unsigned char _pad124[0xfc]; CReleasable_00c55030* m_pTarget; unsigned char _pad224[8]; long m_22c; unsigned char _pad230[8]; long m_238;
    void Reset() { m_pTarget->Release(); m_bodyA.Reset(); m_bodyB.Reset(); m_22c = 0; m_238 = 0; } };
int main() {
    CBodyOwner_00c55030 o; std::memset(&o, 0x11, sizeof o); CReleasable_00c55030 r; o.m_pTarget = &r; o.Reset(); if (std::strcmp(g_seq, "RBB") || g_reset[0] != &o.m_bodyA || g_reset[1] != &o.m_bodyB || o.m_22c != 0 || o.m_238 != 0) { std::printf("00c55030_TEST FAIL\n"); return 1; }
    std::printf("00c55030_TEST PASS\n"); return 0;
}

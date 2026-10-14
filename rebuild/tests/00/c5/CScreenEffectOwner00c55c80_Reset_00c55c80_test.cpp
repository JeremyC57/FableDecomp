#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_mode = 0; struct CStack { unsigned char _body[8]; void Reset(long m) { g_mode = m; } };
struct COwner : CStack { long m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28; unsigned char _pad2c[0xd0]; long m_fc; unsigned char _p[4]; bool m_104; unsigned char _q[0xa7]; long m_1ac; bool m_1b0;
    void Reset() { CStack::Reset(1); m_8 = 0; m_c = 0; m_10 = 0; m_14 = 0; m_18 = 0; m_1c = 0; m_20 = 0; m_24 = 0; m_28 = 0; m_fc = 0; m_1ac = 0; m_104 = false; m_1b0 = false; } };

int main() {
    COwner* o = new COwner; std::memset(o, 0xff, sizeof *o); o->Reset(); if (g_mode != 1 || o->m_8 || o->m_28 || o->m_fc || o->m_1ac || o->m_104 || o->m_1b0 || o->_pad2c[0] != 0xff) { std::printf("00c55c80_TEST FAIL\n"); return 1; } delete o;
    std::printf("00c55c80_TEST PASS\n"); return 0;
}
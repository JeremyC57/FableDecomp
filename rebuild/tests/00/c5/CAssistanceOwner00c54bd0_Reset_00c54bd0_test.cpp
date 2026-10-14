#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CScript { void Init() { g_init = this; } static CScript* g_init; }; CScript* CScript::g_init = 0; static CScript*& g_init = CScript::g_init;
struct COwner { unsigned char _pad[0xc]; long m_c; unsigned char _p[0xa4]; CScript m_script; void Reset() { m_script.Init(); m_c = 0; } };

int main() {
    COwner o; o.m_c = 5; o.Reset(); if (g_init != &o.m_script || o.m_c) { std::printf("00c54bd0_TEST FAIL\n"); return 1; }
    std::printf("00c54bd0_TEST PASS\n"); return 0;
}
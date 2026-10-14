#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SState { long m_0, m_4, m_8, m_c; }; static const void* g_res = 0; static const void* g_text = 0; struct CRes { unsigned char m_body[0x14]; CRes& operator=(const CRes& o) { g_res = &o; return *this; } }; struct CText { unsigned char m_body[4]; CText& operator=(const CText& o) { g_text = &o; return *this; } };
struct CBankStateBlock { virtual void v00() { } SState m_state; CRes m_resource; CText m_text; CBankStateBlock() { std::memset(&m_state, 0, sizeof m_state); } }; struct CMeshBankStateBlock : CBankStateBlock { virtual void Set(const CBankStateBlock* o) { m_state = o->m_state; m_resource = o->m_resource; m_text = o->m_text; } };

int main() {
    CMeshBankStateBlock a, b; b.m_state.m_0 = 1; b.m_state.m_4 = 2; b.m_state.m_8 = 3; b.m_state.m_c = 4; a.Set(&b); if (a.m_state.m_0 != 1 || a.m_state.m_c != 4 || g_res != &b.m_resource || g_text != &b.m_text) { std::printf("00a2d6c0_TEST FAIL\n"); return 1; }
    std::printf("00a2d6c0_TEST PASS\n"); return 0;
}

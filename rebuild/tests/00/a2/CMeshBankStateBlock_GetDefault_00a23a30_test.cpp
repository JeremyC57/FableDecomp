#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SState { bool m_loaded, m_5, m_6; long m_8, m_c, m_10; }; struct CBlock { virtual void v00() { } SState m_state; };
class CMeshBankStateBlock { public: void GetDefault(CBlock* b) { std::memset(&b->m_state, 0, sizeof(b->m_state)); b->m_state.m_loaded = true; b->m_state.m_5 = false; b->m_state.m_6 = false; b->m_state.m_8 = 0; b->m_state.m_c = 0; b->m_state.m_10 = 0; } };

int main() {
    CBlock* b = new CBlock; std::memset(&b->m_state, 0xff, sizeof b->m_state); CMeshBankStateBlock m; m.GetDefault(b); if (!b->m_state.m_loaded || b->m_state.m_5 || b->m_state.m_6 || b->m_state.m_8 || b->m_state.m_c || b->m_state.m_10) { std::printf("00a23a30_TEST FAIL\n"); return 1; } delete b;
    std::printf("00a23a30_TEST PASS\n"); return 0;
}
#include <cstdio>
#include <cstring>
class CInstance { public: unsigned char _body[0x180]; };
struct SThemeRequest { long m_words[7]; };
class CInstanceManager {
public:
    unsigned char _pad_0x000[0x18]; CInstance m_instances[2]; long m_current; unsigned char _pad_0x31c[4]; SThemeRequest m_pending; bool m_bPending;
    CInstance* GetCurrentInstance() { if (m_bPending) return 0; return &m_instances[m_current]; }
};
int main()
{
    static CInstanceManager m; std::memset(&m, 0, sizeof(m));
    m.m_bPending = true; m.m_current = 1;
    bool ok = m.GetCurrentInstance() == 0;
    m.m_bPending = false;
    ok = ok && m.GetCurrentInstance() == &m.m_instances[1] && (char*)m.GetCurrentInstance() - (char*)&m == 0x18 + 0x180
        && (char*)&m.m_current - (char*)&m == 0x318 && (char*)&m.m_bPending - (char*)&m == 0x33c;
    if (!ok) { std::printf("00c2d2b0_TEST FAIL\n"); return 1; }
    std::printf("00c2d2b0_TEST PASS\n");
    return 0;
}

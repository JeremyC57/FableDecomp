#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CMeleeApprentice { unsigned char _pad[0x48]; bool m_48, m_49, m_4a, m_4b, m_4c, m_4d, m_4e, m_4f, m_50, m_51, m_52, m_53, m_54; unsigned char _p[3]; long m_58; unsigned char _q[0xc]; bool m_68;
    void Init() { m_48 = false; m_49 = false; m_4a = false; m_4b = false; m_4c = false; m_4d = false; m_4e = false; m_4f = false; m_51 = false; m_53 = false; m_54 = false; m_50 = false; m_52 = false; m_58 = 0; m_68 = false; } };

int main() {
    CMeleeApprentice* a = new CMeleeApprentice; std::memset(a, 0xff, sizeof *a); a->Init(); unsigned char* p = (unsigned char*)a; for (int i = 0x48; i <= 0x54; ++i) if (p[i]) { std::printf("00e9fec0_TEST FAIL\n"); return 1; } if (a->m_58 || a->m_68 || p[0x55] != 0xff || p[0x47] != 0xff) { std::printf("00e9fec0_TEST FAIL\n"); return 1; } delete a;
    std::printf("00e9fec0_TEST PASS\n"); return 0;
}

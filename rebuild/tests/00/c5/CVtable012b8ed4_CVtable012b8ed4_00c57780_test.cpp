#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CLayer { virtual void v00() { } unsigned char _pad[0x600]; long m_604; unsigned char _p[0xc]; bool m_614, m_615; unsigned char _q[2]; long m_618; CLayer() : m_604(0) { m_618 = 0; m_614 = false; m_615 = false; } };

int main() {
    CLayer* c = new CLayer; if (c->m_604 != 0 || c->m_618 != 0 || c->m_614 || c->m_615) { std::printf("00c57780_TEST FAIL\n"); return 1; } delete c;
    std::printf("00c57780_TEST PASS\n"); return 0;
}
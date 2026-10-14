#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CObject { virtual void v00() { } long m_4; unsigned char _pad08[0xe0]; long m_e8; long m_ec; CObject(bool flag) : m_4(0), m_e8(flag != false), m_ec(0) { } };

int main() {
    CObject a(true), b(false); if (a.m_4 || a.m_ec || a.m_e8 != 1 || b.m_e8 != 0) { std::printf("00c4e2a0_TEST FAIL\n"); return 1; }
    std::printf("00c4e2a0_TEST PASS\n"); return 0;
}

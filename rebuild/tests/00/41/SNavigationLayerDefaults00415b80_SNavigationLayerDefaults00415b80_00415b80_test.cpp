#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SDef { long m_0, m_4, m_8, m_c, m_10, m_14, m_18; bool m_1c; long m_20, m_24; bool m_28, m_29, m_2a; SDef() : m_0(-1), m_4(-1), m_8(-1), m_c(-1), m_10(-1), m_14(-1), m_18(-1), m_1c(false), m_20(-1), m_24(-1), m_28(true), m_29(false), m_2a(true) { } };

int main() {
    SDef d; if (d.m_0 != -1 || d.m_4 != -1 || d.m_8 != -1 || d.m_c != -1 || d.m_10 != -1 || d.m_14 != -1 || d.m_18 != -1 || d.m_1c || d.m_20 != -1 || d.m_24 != -1 || !d.m_28 || d.m_29 || !d.m_2a) { std::printf("00415b80_TEST FAIL\n"); return 1; }
    std::printf("00415b80_TEST PASS\n"); return 0;
}
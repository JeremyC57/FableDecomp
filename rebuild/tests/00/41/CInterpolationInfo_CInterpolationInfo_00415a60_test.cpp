#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
class CInterpolationInfo { public: long m_0, m_4, m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28, m_2c, m_30; CInterpolationInfo() : m_0(0), m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0), m_20(0), m_24(0), m_28(0), m_2c(0), m_30(0) { } };

int main() {
    unsigned char raw[sizeof(CInterpolationInfo)]; std::memset(raw, 0xff, sizeof raw); CInterpolationInfo& i = *new (raw) CInterpolationInfo; if (i.m_0 || i.m_18 || i.m_30) { std::printf("00415a60_TEST FAIL\n"); return 1; }
    std::printf("00415a60_TEST PASS\n"); return 0;
}
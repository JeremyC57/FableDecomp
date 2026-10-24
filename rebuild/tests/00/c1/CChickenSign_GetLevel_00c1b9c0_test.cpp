// Behaviour test for 00c1b9c0: CChickenSign::GetLevel -- the 0..127 level byte as a 0..1 float
#include <cstdio>

struct CChickenSign {
    unsigned char  m_level;
    unsigned char  m_offset;
    unsigned char  m_flags;
    unsigned char  m_byteD;
    float GetLevel() { return (float)m_level * (1.0f / 127.0f); }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    CChickenSign sign;
    sign.m_offset = 9; sign.m_flags = 9; sign.m_byteD = 9;
    sign.m_level = 0;   if (!Near(sign.GetLevel(), 0.0f)) { std::printf("00c1b9c0_TEST FAIL\n"); return 1; }
    sign.m_level = 127; if (!Near(sign.GetLevel(), 1.0f)) { std::printf("00c1b9c0_TEST FAIL\n"); return 1; }
    sign.m_level = 254; if (!Near(sign.GetLevel(), 2.0f)) { std::printf("00c1b9c0_TEST FAIL\n"); return 1; }
    sign.m_level = 64;  if (!Near(sign.GetLevel(), 64.0f / 127.0f)) { std::printf("00c1b9c0_TEST FAIL\n"); return 1; }
    std::printf("00c1b9c0_TEST PASS\n");
    return 0;
}
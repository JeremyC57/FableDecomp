#include <cstdio>
struct CRenderSetting_00c44880 {
    unsigned long m_flags; long m_value;
    void SetValue(long value) { if (m_value != value) { m_value = value; m_flags |= 0x4000; } }
};
int main()
{
    CRenderSetting_00c44880 s; s.m_flags = 1; s.m_value = 7;
    s.SetValue(7);
    if (s.m_flags != 1 || s.m_value != 7) { std::printf("00c44880_TEST FAIL\n"); return 1; }
    s.SetValue(-3);
    if (s.m_flags != 0x4001 || s.m_value != -3) { std::printf("00c44880_TEST FAIL\n"); return 1; }
    std::printf("00c44880_TEST PASS\n");
    return 0;
}
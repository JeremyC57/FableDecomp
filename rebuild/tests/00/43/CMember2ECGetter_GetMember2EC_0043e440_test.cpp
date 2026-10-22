// Behaviour test for 0043e440 GetMember2EC (plain member getter at +0x2EC)
#include <cstdio>
struct CMember2ECGetter {
    unsigned char _pad000[0x2ec];
    long m_value2EC;
    long GetMember2EC();
};
long CMember2ECGetter::GetMember2EC()
{
    return m_value2EC;
}
int main()
{
    static CMember2ECGetter g;
    g.m_value2EC = -12345;
    if (g.GetMember2EC() != -12345) { std::printf("0043e440_TEST FAIL\n"); return 1; }
    g.m_value2EC = 99;
    if (g.GetMember2EC() != 99) { std::printf("0043e440_TEST FAIL\n"); return 1; }
    std::printf("0043e440_TEST PASS\n");
    return 0;
}
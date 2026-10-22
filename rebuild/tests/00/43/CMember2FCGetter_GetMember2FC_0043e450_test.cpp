// Behaviour test for 0043e450 GetMember2FC (plain member getter at +0x2FC)
#include <cstdio>
struct CMember2FCGetter {
    unsigned char _pad000[0x2fc];
    long m_value2FC;
    long GetMember2FC();
};
long CMember2FCGetter::GetMember2FC()
{
    return m_value2FC;
}
int main()
{
    static CMember2FCGetter g;
    g.m_value2FC = -12345;
    if (g.GetMember2FC() != -12345) { std::printf("0043e450_TEST FAIL\n"); return 1; }
    g.m_value2FC = 99;
    if (g.GetMember2FC() != 99) { std::printf("0043e450_TEST FAIL\n"); return 1; }
    std::printf("0043e450_TEST PASS\n");
    return 0;
}
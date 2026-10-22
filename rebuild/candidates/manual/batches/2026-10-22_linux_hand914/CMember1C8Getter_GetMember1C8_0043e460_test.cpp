// Behaviour test for 0043e460 GetMember1C8 (plain member getter at +0x1C8)
#include <cstdio>
struct CMember1C8Getter {
    unsigned char _pad000[0x1c8];
    long m_value1C8;
    long GetMember1C8();
};
long CMember1C8Getter::GetMember1C8()
{
    return m_value1C8;
}
int main()
{
    static CMember1C8Getter g;
    g.m_value1C8 = -12345;
    if (g.GetMember1C8() != -12345) { std::printf("0043e460_TEST FAIL\n"); return 1; }
    g.m_value1C8 = 99;
    if (g.GetMember1C8() != 99) { std::printf("0043e460_TEST FAIL\n"); return 1; }
    std::printf("0043e460_TEST PASS\n");
    return 0;
}

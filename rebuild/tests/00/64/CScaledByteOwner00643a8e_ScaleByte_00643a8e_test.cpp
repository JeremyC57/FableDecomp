#include <cstdio>
struct CScaledByteOwner_00643a8e {
    unsigned char _pad_0x00[0x2A0];
    float m_scale;
    long ScaleByte(unsigned char value);
};
long CScaledByteOwner_00643a8e::ScaleByte(unsigned char value)
{
    return (long)(value * m_scale);
}
int main()
{
    static CScaledByteOwner_00643a8e o;
    o.m_scale = 2.5f;
    if (o.ScaleByte(4) != 10 || o.ScaleByte(255) != 637 || o.ScaleByte(0) != 0) { std::printf("00643a8e_TEST FAIL\n"); return 1; }
    o.m_scale = -1.5f;
    if (o.ScaleByte(3) != -4) { std::printf("00643a8e_TEST FAIL\n"); return 1; }
    std::printf("00643a8e_TEST PASS\n");
    return 0;
}
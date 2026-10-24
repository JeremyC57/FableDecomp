// Behaviour test for 00c3d310 CVtable_012B5870::SetSign8C (copy of the model).
#include <cstdio>
struct CVtable_012B5870 {
    virtual ~CVtable_012B5870() {}
    unsigned char _body[0x84];
    long          m_value88;
    float         m_sign8C;
    unsigned char _pad90[0x14];
    unsigned long m_dirtyFlags;
    void SetSign8C(long mode);
};
void CVtable_012B5870::SetSign8C(long mode)
{
    float sign = mode == 1 ? -1.0f : 1.0f;
    if (sign != m_sign8C) {
        m_sign8C = sign;
        m_dirtyFlags |= 0xbf8000;
    }
}
int main()
{
    CVtable_012B5870 o; o.m_sign8C = 1.0f; o.m_dirtyFlags = 0x1;
    int ok = 1;
    o.SetSign8C(0);                                     // already +1: unchanged
    if (o.m_sign8C != 1.0f || o.m_dirtyFlags != 0x1) ok = 0;
    o.SetSign8C(1);                                     // flips to -1, dirty
    if (o.m_sign8C != -1.0f || o.m_dirtyFlags != (0x1ul | 0xbf8000ul)) ok = 0;
    o.m_dirtyFlags = 0;
    o.SetSign8C(1);                                     // same again: clean
    if (o.m_dirtyFlags != 0) ok = 0;
    o.SetSign8C(2);                                     // any other mode is +1
    if (o.m_sign8C != 1.0f || o.m_dirtyFlags != 0xbf8000ul) ok = 0;
    if (!ok) { std::printf("00c3d310_TEST FAIL\n"); return 1; }
    std::printf("00c3d310_TEST PASS\n");
    return 0;
}
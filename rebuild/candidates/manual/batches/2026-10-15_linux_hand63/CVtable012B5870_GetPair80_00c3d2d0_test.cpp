// Behaviour test for CVtable_012B5870::GetPair80 (0x00C3D2D0): both words copied out.
#include <cstdio>

struct CVtable_012B5870 {
    virtual void v00() {}
    unsigned char _body[0x7c];
    long m_value80;
    long m_value84;
    void GetPair80(long* first, long* second) const;
};

void CVtable_012B5870::GetPair80(long* first, long* second) const
{
    *first = m_value80;
    *second = m_value84;
}

int main()
{
    CVtable_012B5870 o;
    o.m_value80 = 640;
    o.m_value84 = -480;
    long a = 1, b = 2;
    o.GetPair80(&a, &b);
    if (a != 640 || b != -480) {
        std::printf("00c3d2d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00c3d2d0_TEST PASS\n");
    return 0;
}

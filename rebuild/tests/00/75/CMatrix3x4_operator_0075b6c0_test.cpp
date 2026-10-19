// Behaviour test for 0x0075B6C0 CMatrix3x4::operator*(float): every element scaled, source untouched.
#include <cstdio>
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    void PostScale(float s)
    {
        E11 *= s; E21 *= s; E31 *= s; E41 *= s;
        E12 *= s; E22 *= s; E32 *= s; E42 *= s;
        E13 *= s; E23 *= s; E33 *= s; E43 *= s;
    }
    CMatrix3x4 operator*(float s);
};
CMatrix3x4 CMatrix3x4::operator*(float s)
{
    CMatrix3x4 result(*this);
    result.PostScale(s);
    return result;
}
int main()
{
    CMatrix3x4 m;
    float* p = &m.E11;
    for (int i = 0; i < 12; ++i) p[i] = (float)(i + 1);
    CMatrix3x4 r = m * 2.0f;
    const float* q = &r.E11;
    for (int i = 0; i < 12; ++i) {
        float want = (float)(2 * (i + 1));
        float orig = (float)(i + 1);
        if (q[i] != want || p[i] != orig) { std::printf("0075b6c0_TEST FAIL\n"); return 1; }
    }
    std::printf("0075b6c0_TEST PASS\n");
    return 0;
}
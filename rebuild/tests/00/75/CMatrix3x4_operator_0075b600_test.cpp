// Behaviour test for 0x0075B600 CMatrix3x4::operator+(const CMatrix3x4&): element-wise sum, operands untouched.
#include <cstdio>

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    void Add(const CMatrix3x4& o)
    {
        E11 += o.E11; E12 += o.E12; E13 += o.E13;
        E21 += o.E21; E22 += o.E22; E23 += o.E23;
        E31 += o.E31; E32 += o.E32; E33 += o.E33;
        E41 += o.E41; E42 += o.E42; E43 += o.E43;
    }
    CMatrix3x4 operator+(const CMatrix3x4& other)
    {
        CMatrix3x4 result(*this);
        result.Add(other);
        return result;
    }
};

int main()
{
    CMatrix3x4 a, b;
    float* pa = &a.E11; float* pb = &b.E11;
    for (int i = 0; i < 12; ++i) { pa[i] = (float)i; pb[i] = (float)(100 - i * 2); }
    CMatrix3x4 c = a + b;
    float* pc = &c.E11;
    int ok = 1;
    for (int i = 0; i < 12; ++i) if (pc[i] != (float)(100 - i) || pa[i] != (float)i) ok = 0;
    if (!ok) { std::printf("0075b600_TEST FAIL\n"); return 1; }
    std::printf("0075b600_TEST PASS\n");
    return 0;
}
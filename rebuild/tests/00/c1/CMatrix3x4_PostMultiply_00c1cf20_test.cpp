// Behaviour test for CMatrix3x4::PostMultiply @ 0x00C1CF20: this = this * b (row-vector convention, row 4 = translation).
#include <cstdio>

class CMatrix3x4 {
public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    CMatrix3x4() {}
    CMatrix3x4(const CMatrix3x4& o)
        : E11(o.E11), E12(o.E12), E13(o.E13), E21(o.E21), E22(o.E22), E23(o.E23),
          E31(o.E31), E32(o.E32), E33(o.E33), E41(o.E41), E42(o.E42), E43(o.E43) {}
    void PostMultiply(const CMatrix3x4& b);
};

void CMatrix3x4::PostMultiply(const CMatrix3x4& b)
{
    CMatrix3x4 a(*this);
    E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
    E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
    E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
    E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
    E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
    E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
    E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
    E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
    E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
    E41 = a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31 + b.E41;
    E42 = a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32 + b.E42;
    E43 = a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33 + b.E43;
}

static void set(CMatrix3x4& m, const float* v)
{
    m.E11 = v[0]; m.E12 = v[1]; m.E13 = v[2]; m.E21 = v[3]; m.E22 = v[4]; m.E23 = v[5];
    m.E31 = v[6]; m.E32 = v[7]; m.E33 = v[8]; m.E41 = v[9]; m.E42 = v[10]; m.E43 = v[11];
}

int main()
{
    // a: 90-degree rotation about Z (x->y) plus translation (1,2,3); b: scale 2 plus translation (10,0,0)
    static const float av[12] = { 0, 1, 0, -1, 0, 0, 0, 0, 1, 1, 2, 3 };
    static const float bv[12] = { 2, 0, 0, 0, 2, 0, 0, 0, 2, 10, 0, 0 };
    CMatrix3x4 a, b;
    set(a, av); set(b, bv);
    a.PostMultiply(b);
    int ok = a.E11 == 0 && a.E12 == 2 && a.E13 == 0 && a.E21 == -2 && a.E22 == 0 && a.E33 == 2
          && a.E41 == 12 && a.E42 == 4 && a.E43 == 6;
    if (!ok) { std::printf("00c1cf20_TEST FAIL\n"); return 1; }
    std::printf("00c1cf20_TEST PASS\n");
    return 0;
}
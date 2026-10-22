// Behaviour test for CMatrix3x4::PostMultiply @ 0x00C1CF20: this = this * b (row-vector convention, row 4 = translation).
#include <cstdio>

class CMatrix3x4 {
public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    CMatrix3x4() {}
    CMatrix3x4(const CMatrix3x4& o)
        : E11(o.E11), E12(o.E12), E13(o.E13), E21(o.E21), E22(o.E22), E23(o.E23),
          E31(o.E31), E32(o.E32), E33(o.E33), E41(o.E41), E42(o.E42), E43(o.E43) {}
    void PostMultiply(const CMatrix3x4& matrix);
};

void CMatrix3x4::PostMultiply(const CMatrix3x4& matrix)
{
    CMatrix3x4 a(*this);
    E11 = a.E11 * matrix.E11 + a.E12 * matrix.E21 + a.E13 * matrix.E31;
    E12 = a.E11 * matrix.E12 + a.E12 * matrix.E22 + a.E13 * matrix.E32;
    E13 = a.E11 * matrix.E13 + a.E12 * matrix.E23 + a.E13 * matrix.E33;
    E21 = a.E21 * matrix.E11 + a.E22 * matrix.E21 + a.E23 * matrix.E31;
    E22 = a.E21 * matrix.E12 + a.E22 * matrix.E22 + a.E23 * matrix.E32;
    E23 = a.E21 * matrix.E13 + a.E22 * matrix.E23 + a.E23 * matrix.E33;
    E31 = a.E31 * matrix.E11 + a.E32 * matrix.E21 + a.E33 * matrix.E31;
    E32 = a.E31 * matrix.E12 + a.E32 * matrix.E22 + a.E33 * matrix.E32;
    E33 = a.E31 * matrix.E13 + a.E32 * matrix.E23 + a.E33 * matrix.E33;
    E41 = a.E41 * matrix.E11 + a.E42 * matrix.E21 + a.E43 * matrix.E31 + matrix.E41;
    E42 = a.E41 * matrix.E12 + a.E42 * matrix.E22 + a.E43 * matrix.E32 + matrix.E42;
    E43 = a.E41 * matrix.E13 + a.E42 * matrix.E23 + a.E43 * matrix.E33 + matrix.E43;
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
    CMatrix3x4 a, matrix;
    set(a, av); set(matrix, bv);
    a.PostMultiply(matrix);
    int ok = a.E11 == 0 && a.E12 == 2 && a.E13 == 0 && a.E21 == -2 && a.E22 == 0 && a.E33 == 2
          && a.E41 == 12 && a.E42 == 4 && a.E43 == 6;
    if (!ok) { std::printf("00c1cf20_TEST FAIL\n"); return 1; }
    std::printf("00c1cf20_TEST PASS\n");
    return 0;
}
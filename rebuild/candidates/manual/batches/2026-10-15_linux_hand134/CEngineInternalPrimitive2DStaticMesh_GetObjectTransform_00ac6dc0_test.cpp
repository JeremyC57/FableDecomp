#include <cstdio>
struct CMatrix3x4 { float m[12]; };
struct CDoubleMatrix3x3 {
    double m[9];
    CMatrix3x4 GetObjectTransform() const
    {
        CMatrix3x4 r;
        r.m[0] = (float)m[0]; r.m[1] = (float)m[1]; r.m[2] = (float)m[2];
        r.m[3] = (float)m[1]; r.m[4] = (float)m[4]; r.m[5] = (float)m[5];
        r.m[6] = (float)m[2]; r.m[7] = (float)m[5]; r.m[8] = (float)m[7];
        r.m[9] = 0.0f; r.m[10] = 0.0f; r.m[11] = 0.0f;
        return r;
    }
};
int main()
{
    CDoubleMatrix3x3 d;
    for (int i = 0; i < 9; ++i) d.m[i] = 1.5 * (i + 1);
    CMatrix3x4 r = d.GetObjectTransform();
    const float want[12] = { 1.5f, 3.0f, 4.5f, 3.0f, 7.5f, 9.0f, 4.5f, 9.0f, 12.0f, 0.0f, 0.0f, 0.0f };
    for (int j = 0; j < 12; ++j)
        if (r.m[j] != want[j]) { std::printf("00ac6dc0_TEST FAIL\n"); return 1; }
    std::printf("00ac6dc0_TEST PASS\n");
    return 0;
}

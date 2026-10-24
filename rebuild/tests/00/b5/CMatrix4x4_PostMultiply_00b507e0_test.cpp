#include <cstdio>
#include <cmath>
// Behaviour test for CMatrix4x4::PostMultiply(const CMatrix4x4&) @ 0x00B507E0: this = this * m (row i of this against
// column j of m), safe when m aliases this.
class CMatrix4x4 {
public:
    float M[4][4];
    void PostMultiply(const CMatrix4x4& m)
    {
        CMatrix4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.M[i][j] = M[i][0] * m.M[0][j] + M[i][1] * m.M[1][j] + M[i][2] * m.M[2][j] + M[i][3] * m.M[3][j];
        *this = result;
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }
int main()
{
    CMatrix4x4 a, b, ref, sq;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            a.M[i][j] = (float)(i * 4 + j + 1);
            b.M[i][j] = (float)((i + 2) * (j + 1) % 7) - 3.0f;
        }
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            float s = 0.0f;
            for (int k = 0; k < 4; ++k) s += a.M[i][k] * b.M[k][j];
            ref.M[i][j] = s;
        }
    sq = a;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            float s = 0.0f;
            for (int k = 0; k < 4; ++k) s += a.M[i][k] * a.M[k][j];
            sq.M[i][j] = s;
        }
    CMatrix4x4 c = a;
    c.PostMultiply(b);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (!Near(c.M[i][j], ref.M[i][j])) { std::printf("00b507e0_TEST FAIL\n"); return 1; }
    CMatrix4x4 d = a;
    d.PostMultiply(d);                                   // aliased
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (!Near(d.M[i][j], sq.M[i][j])) { std::printf("00b507e0_TEST FAIL\n"); return 1; }
    std::printf("00b507e0_TEST PASS\n");
    return 0;
}
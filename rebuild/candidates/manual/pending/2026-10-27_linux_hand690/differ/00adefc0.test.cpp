// Behaviour test for 0x00ADEFC0 CMatrix4x4::AlignedPreMultiplyTransposed (the x87 fallback path; the SSE block computes the same product)
#include <cstdio>
#include <cmath>
class CMatrix4x4 {
public:
    float m[4][4];
    void AlignedPreMultiplyTransposed(const CMatrix4x4& other)
    {
        CMatrix4x4 copy = *this;
        for (long row = 0; row < 4; ++row) {
            const float* a = copy.m[row];
            for (long column = 0; column < 4; ++column)
                m[row][column] = a[0] * other.m[0][column] + a[1] * other.m[1][column]
                               + a[2] * other.m[2][column] + a[3] * other.m[3][column];
        }
    }
};
int main()
{
    CMatrix4x4 a, b, ref;
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) { a.m[i][j] = (float)(i * 4 + j + 1); b.m[i][j] = (float)((i + 2) * (j + 1) % 7) - 3.0f; }
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) { float s = 0; for (int k = 0; k < 4; k++) s += a.m[i][k] * b.m[k][j]; ref.m[i][j] = s; }
    a.AlignedPreMultiplyTransposed(b);
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++)
        if (std::fabs(a.m[i][j] - ref.m[i][j]) > 1e-3f) { std::printf("00adefc0_TEST FAIL\n"); return 1; }
    CMatrix4x4 id; for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) id.m[i][j] = i == j ? 1.0f : 0.0f;
    CMatrix4x4 c = ref; c.AlignedPreMultiplyTransposed(id);
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++)
        if (std::fabs(c.m[i][j] - ref.m[i][j]) > 1e-3f) { std::printf("00adefc0_TEST FAIL\n"); return 1; }
    std::printf("00adefc0_TEST PASS\n");
    return 0;
}

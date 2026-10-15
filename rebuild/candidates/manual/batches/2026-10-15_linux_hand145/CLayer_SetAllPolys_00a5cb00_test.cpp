// Behaviour test for 0x00A5CB00 (CLayer::SetAllPolys = set to the inverse of a 4x4 matrix): copy in, invert, copy back.
#include <cstdio>
struct M { float m[4][4]; };
static float g_det;
static M* Inverse(M* out, float* det, const M* in)          // stand-in for D3DXMatrixInverse: diagonal matrices only
{
    float d = 1.0f;
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) out->m[r][c] = (r == c) ? 1.0f / in->m[r][c] : 0.0f;
    for (int i = 0; i < 4; ++i) d *= in->m[i][i];
    *det = d; g_det = d;
    return out;
}
struct Layer {
    float m[4][4];
    void SetAllPolys(const Layer& source)
    {
        M in;
        for (long column = 0; column < 4; ++column) for (long row = 0; row < 4; ++row) in.m[row][column] = source.m[row][column];
        float determinant = 0.0f;
        M out;
        Inverse(&out, &determinant, &in);
        for (long column = 0; column < 4; ++column) for (long row = 0; row < 4; ++row) m[row][column] = out.m[row][column];
    }
};
int main()
{
    Layer src, dst;
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) src.m[r][c] = (r == c) ? (float)(1 << r) : 0.0f;
    dst.SetAllPolys(src);
    int ok = dst.m[0][0] == 1.0f && dst.m[1][1] == 0.5f && dst.m[2][2] == 0.25f && dst.m[3][3] == 0.125f && dst.m[0][1] == 0.0f && g_det == 64.0f;
    if (!ok) { std::printf("00a5cb00_TEST FAIL\n"); return 1; }
    std::printf("00a5cb00_TEST PASS\n");
    return 0;
}

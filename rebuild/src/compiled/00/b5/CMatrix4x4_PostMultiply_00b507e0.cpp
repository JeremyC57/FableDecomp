// CMatrix4x4::PostMultiply(const CMatrix4x4& m) -- this = this * m: each row of this times m's columns into a frame temporary, then block-copied back
// Leaf: ?PostMultiply@CMatrix4x4@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B507E0, 186 bytes. __fastcall, `ret 4`.
// Identity: FableWin.pdb `?PostMultiply@CMatrix4x4@@QAEXABV1@@Z` (void (const CMatrix4x4&)); the body is the row-by-
//   column product with the result staged in a 0x40-byte frame matrix and copied over this (`rep movsd` x16). No
//   direct caller in the image (inlined at its uses). The catalog label "Matrix_Multiply4x4" is descriptive.
// Layout: CMatrix4x4 is Ego_r's 64 bytes E11..E44 (row-major) in an anonymous union with the [4][4] array view.
// Shape: a plain i/j double loop over `M[i][0]*m.M[0][j] + ... + M[i][3]*m.M[3][j]`: VC7.1 fully unrolls the inner j
//   loop (and reorders the products per column); writing the four column sums out by hand compiles differently.
class CMatrix4x4 {
public:
    union {
        struct {
            float E11, E12, E13, E14;
            float E21, E22, E23, E24;
            float E31, E32, E33, E34;
            float E41, E42, E43, E44;
        };
        float M[4][4];
    };
    CMatrix4x4() { }
    void PostMultiply(const CMatrix4x4& m);
};

void CMatrix4x4::PostMultiply(const CMatrix4x4& m)
{
    CMatrix4x4 result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.M[i][j] = M[i][0] * m.M[0][j] + M[i][1] * m.M[1][j] + M[i][2] * m.M[2][j] + M[i][3] * m.M[3][j];
    *this = result;
}
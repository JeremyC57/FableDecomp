// Behaviour test for CPreTransposedBoneMatrix::Set (0x00987F70): Data[r][c] = axis c's component r (X, Y, Z axes then
// the position as the fourth column).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { C3DVector XAxis, YAxis, ZAxis, Pos; };
struct CPreTransposedBoneMatrix {
    float Data[3][4];
    void Set(const CMatrix3x4& m)
    {
        Data[0][0] = m.XAxis.X; Data[0][1] = m.YAxis.X; Data[0][2] = m.ZAxis.X; Data[0][3] = m.Pos.X;
        Data[1][0] = m.XAxis.Y; Data[1][1] = m.YAxis.Y; Data[1][2] = m.ZAxis.Y; Data[1][3] = m.Pos.Y;
        Data[2][0] = m.XAxis.Z; Data[2][1] = m.YAxis.Z; Data[2][2] = m.ZAxis.Z; Data[2][3] = m.Pos.Z;
    }
};
int main()
{
    CMatrix3x4 m;
    C3DVector* axes[4] = { &m.XAxis, &m.YAxis, &m.ZAxis, &m.Pos };
    for (int c = 0; c < 4; ++c) { axes[c]->X = (float)(c * 10 + 0); axes[c]->Y = (float)(c * 10 + 1); axes[c]->Z = (float)(c * 10 + 2); }
    CPreTransposedBoneMatrix b; b.Set(m);
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 4; ++c)
            if (b.Data[r][c] != (float)(c * 10 + r)) { std::printf("00987f70_TEST FAIL\n"); return 1; }
    std::printf("00987f70_TEST PASS\n");
    return 0;
}

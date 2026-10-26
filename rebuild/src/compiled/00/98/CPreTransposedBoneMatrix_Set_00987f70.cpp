// CPreTransposedBoneMatrix::Set(const CMatrix3x4&) -- store a 3x4 bone matrix transposed (one row per axis component) for the vertex-shader constants
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987F70, length 77 bytes.
// Callees: none. Callers: CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList 0x00BAB2FB, 0x00BAC518.
// Layout: Ego_r.pdb CPreTransposedBoneMatrix = float Data[3][4]; CMatrix3x4 is the four axes X, Y, Z, Pos
// (12 floats).
// Shape: twelve float assignments, which VC7.1 copies through edx; the stores follow the destination order,
// the loads the transposed source order (0, 3, 6, 9, 1, 4, ...).
class C3DVector { public:
    float X, Y, Z;
};

class CMatrix3x4 { public:                                         // 48 bytes: three axes and the position
    C3DVector XAxis;                                        // +0x00
    C3DVector YAxis;                                        // +0x0c
    C3DVector ZAxis;                                        // +0x18
    C3DVector Pos;                                          // +0x24
};

struct CPreTransposedBoneMatrix {
    float Data[3][4];                                       // +0x00
    void Set(const CMatrix3x4& m);
};

void CPreTransposedBoneMatrix::Set(const CMatrix3x4& m)
{
    Data[0][0] = m.XAxis.X;
    Data[0][1] = m.YAxis.X;
    Data[0][2] = m.ZAxis.X;
    Data[0][3] = m.Pos.X;
    Data[1][0] = m.XAxis.Y;
    Data[1][1] = m.YAxis.Y;
    Data[1][2] = m.ZAxis.Y;
    Data[1][3] = m.Pos.Y;
    Data[2][0] = m.XAxis.Z;
    Data[2][1] = m.YAxis.Z;
    Data[2][2] = m.ZAxis.Z;
    Data[2][3] = m.Pos.Z;
}
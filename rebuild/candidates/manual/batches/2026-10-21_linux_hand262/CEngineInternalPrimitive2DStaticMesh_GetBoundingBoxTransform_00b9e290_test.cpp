// behaviour test for 00b9e290 GetBoundingBoxTransform: CalculateMatrix of the mesh box, then *= ObjectTransform
#include <cstdio>
#include <cmath>
struct C3DVector { float X, Y, Z; };
class CMatrix3x4 {
public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    CMatrix3x4& operator*=(const CMatrix3x4& b)
    {
        CMatrix3x4 a(*this);
        E11 = a.E11 * b.E11 + a.E13 * b.E31 + a.E12 * b.E21;
        E12 = a.E11 * b.E12 + a.E13 * b.E32 + a.E12 * b.E22;
        E13 = a.E11 * b.E13 + a.E13 * b.E33 + a.E12 * b.E23;
        E21 = a.E21 * b.E11 + a.E23 * b.E31 + a.E22 * b.E21;
        E22 = a.E21 * b.E12 + a.E23 * b.E32 + a.E22 * b.E22;
        E23 = a.E21 * b.E13 + a.E23 * b.E33 + a.E22 * b.E23;
        E31 = a.E31 * b.E11 + a.E33 * b.E31 + a.E32 * b.E21;
        E32 = a.E31 * b.E12 + (a.E32 * b.E22 + a.E33 * b.E32);
        E33 = a.E31 * b.E13 + (a.E32 * b.E23 + a.E33 * b.E33);
        E41 = (a.E41 * b.E11 + (a.E43 * b.E31 + a.E42 * b.E21)) + b.E41;
        E42 = (a.E41 * b.E12 + (a.E42 * b.E22 + a.E43 * b.E32)) + b.E42;
        E43 = (a.E41 * b.E13 + (a.E42 * b.E23 + a.E43 * b.E33)) + b.E43;
        return *this;
    }
};
struct C3DBoundingBox {
    C3DVector MinCoord, MaxCoord;
    void CalculateMatrix(CMatrix3x4& m) const   // simplified 0x00ADEBC0 (no epsilon clamp)
    {
        m.E11 = (MaxCoord.X - MinCoord.X) * 0.5f; m.E12 = 0; m.E13 = 0;
        m.E21 = 0; m.E22 = (MaxCoord.Y - MinCoord.Y) * 0.5f; m.E23 = 0;
        m.E31 = 0; m.E32 = 0; m.E33 = (MaxCoord.Z - MinCoord.Z) * 0.5f;
        m.E41 = (MaxCoord.X + MinCoord.X) * 0.5f; m.E42 = (MaxCoord.Y + MinCoord.Y) * 0.5f; m.E43 = (MaxCoord.Z + MinCoord.Z) * 0.5f;
    }
};
struct C3DMeshInfo { C3DBoundingBox BoundingBox; };
struct CMesh { C3DMeshInfo* MeshInfo; CMatrix3x4 ObjectTransform; };
static bool GetBoundingBoxTransform(CMesh* self, CMatrix3x4& transform)
{
    {
        C3DBoundingBox box(self->MeshInfo->BoundingBox);
        box.CalculateMatrix(transform);
    }
    transform *= self->ObjectTransform;
    return true;
}
static int Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    C3DMeshInfo info;
    info.BoundingBox.MinCoord.X = -1; info.BoundingBox.MinCoord.Y = -2; info.BoundingBox.MinCoord.Z = -3;
    info.BoundingBox.MaxCoord.X = 3; info.BoundingBox.MaxCoord.Y = 2; info.BoundingBox.MaxCoord.Z = 5;
    CMesh mesh; mesh.MeshInfo = &info;
    float o[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    float* po = &mesh.ObjectTransform.E11;
    for (int i = 0; i < 12; ++i) po[i] = o[i];
    CMatrix3x4 m;
    if (!GetBoundingBoxTransform(&mesh, m)) { std::printf("00b9e290_TEST FAIL\n"); return 1; }
    // reference: box matrix B = diag(2,2,4) with centre (1,0,1); result = B * O
    float b[12] = { 2, 0, 0, 0, 2, 0, 0, 0, 4, 1, 0, 1 };
    float* pm = &m.E11;
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 3; ++c) {
            float want = b[r * 3] * o[c] + b[r * 3 + 1] * o[3 + c] + b[r * 3 + 2] * o[6 + c] + (r == 3 ? o[9 + c] : 0.0f);
            if (!Near(pm[r * 3 + c], want)) { std::printf("00b9e290_TEST FAIL\n"); return 1; }
        }
    std::printf("00b9e290_TEST PASS\n");
    return 0;
}

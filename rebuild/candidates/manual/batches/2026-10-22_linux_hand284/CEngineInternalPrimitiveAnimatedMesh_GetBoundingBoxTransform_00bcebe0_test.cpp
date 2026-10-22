// Behaviour test for CEngineInternalPrimitiveAnimatedMesh::GetBoundingBoxTransform (0x00BCEBE0): the box is a scaled cube
// of half-size ((|centre| + radius) * Scale + margin) / Scale translated to the sphere centre, then post-multiplied by
// the interpolated object transform (row-vector convention, translation in row 4); always true.
#include <cstdio>
#include <cmath>

struct M { float e[4][3]; };
static void Mul(M& r, const M& a, const M& b)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j)
            r.e[i][j] = a.e[i][0] * b.e[0][j] + a.e[i][1] * b.e[1][j] + a.e[i][2] * b.e[2][j] + (i == 3 ? b.e[3][j] : 0.0f);
}
struct Mesh {
    float cx, cy, cz, radius, scale, margin; M object;
    bool GetBoundingBoxTransform(M& t) const
    {
        float size = ((radius + (float)std::sqrt(cz * cz + cy * cy + cx * cx)) * scale + margin) / scale;
        for (int i = 0; i < 4; ++i) for (int j = 0; j < 3; ++j) t.e[i][j] = 0.0f;
        t.e[0][0] = t.e[1][1] = t.e[2][2] = size;
        t.e[3][0] = cx; t.e[3][1] = cy; t.e[3][2] = cz;
        M a = t;
        Mul(t, a, object);
        return true;
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Mesh m;
    m.cx = 3; m.cy = 0; m.cz = 4; m.radius = 1; m.scale = 2; m.margin = 4;
    // object transform: uniform scale 2, translation (10, 20, 30)
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 3; ++j) m.object.e[i][j] = 0.0f;
    m.object.e[0][0] = m.object.e[1][1] = m.object.e[2][2] = 2.0f;
    m.object.e[3][0] = 10; m.object.e[3][1] = 20; m.object.e[3][2] = 30;
    M t;
    int bad = 0;
    if (!m.GetBoundingBoxTransform(t)) ++bad;
    // size = ((1 + 5) * 2 + 4) / 2 = 8; diagonal 8 * 2 = 16; translation (3,0,4) * 2 + (10,20,30)
    if (!Near(t.e[0][0], 16) || !Near(t.e[1][1], 16) || !Near(t.e[2][2], 16)) ++bad;
    if (!Near(t.e[0][1], 0) || !Near(t.e[1][2], 0) || !Near(t.e[2][0], 0)) ++bad;
    if (!Near(t.e[3][0], 16) || !Near(t.e[3][1], 20) || !Near(t.e[3][2], 38)) ++bad;
    if (bad == 0) std::printf("00bcebe0_TEST PASS\n"); else std::printf("00bcebe0_TEST FAIL %d\n", bad);
    return bad;
}

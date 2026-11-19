// Behaviour test for TransformPointThroughMirroredFrame (0x00C1C330), a model of the row: p' = a * point (4x3, row vectors,
// translation last), z' = |z'| when negative, result = b * p'. Dyadic inputs keep every product and sum exact, so the model's term
// order cannot change the result; checked against a plain row-vector reference for a mirrored and an unmirrored case.
#include <stdio.h>
class C3DVector { public: float x, y, z; };
struct C3DMatrix { C3DVector Row[3]; C3DVector Translation; };
static C3DVector Model(const C3DMatrix& a, const C3DVector& point, const C3DMatrix& b)
{
    float x = a.Row[2].x * point.z + a.Row[1].x * point.y + a.Row[0].x * point.x + a.Translation.x;
    float y = a.Row[2].y * point.z + a.Row[0].y * point.x + a.Row[1].y * point.y + a.Translation.y;
    float z = a.Row[2].z * point.z + a.Row[0].z * point.x + a.Row[1].z * point.y + a.Translation.z;
    if (z < 0.0f)
        z *= -1.0f;
    C3DVector result;
    float rx = b.Row[1].x * y; rx += b.Row[2].x * z; rx = rx + b.Row[0].x * x + b.Translation.x;
    float ry = b.Row[1].y * y; ry += b.Row[0].y * x; ry = ry + b.Row[2].y * z + b.Translation.y;
    float rz = b.Row[1].z * y; rz += b.Row[0].z * x; rz = rz + b.Row[2].z * z + b.Translation.z;
    result.x = rx; result.y = ry; result.z = rz;
    return result;
}
static void Ref(const C3DMatrix& m, const float in[3], float out[3])
{
    const float* r[4] = { &m.Row[0].x, &m.Row[1].x, &m.Row[2].x, &m.Translation.x };
    for (int c = 0; c < 3; ++c) out[c] = in[0] * r[0][c] + in[1] * r[1][c] + in[2] * r[2][c] + r[3][c];
}
int main()
{
    int bad = 0;
    C3DMatrix a = { { { 1.0f, 0.5f, -2.0f }, { 0.25f, 2.0f, 1.0f }, { -1.0f, 0.0f, 0.5f } }, { 3.0f, -1.0f, 0.75f } };
    C3DMatrix b = { { { 0.5f, 1.0f, 0.0f }, { 2.0f, -0.5f, 1.0f }, { 1.0f, 0.25f, -1.0f } }, { -2.0f, 4.0f, 0.5f } };
    const C3DVector points[2] = { { 1.0f, 2.0f, -0.5f }, { 4.0f, -1.0f, 2.0f } };
    for (int k = 0; k < 2; ++k) {
        float p[3] = { points[k].x, points[k].y, points[k].z }, q[3], want[3];
        Ref(a, p, q);
        bool mirrored = q[2] < 0.0f;
        if (mirrored) q[2] = -q[2];
        Ref(b, q, want);
        C3DVector got = Model(a, points[k], b);
        if (got.x != want[0] || got.y != want[1] || got.z != want[2]) {
            printf("case %d (mirrored %d): %g %g %g != %g %g %g\n", k, (int)mirrored, got.x, got.y, got.z, want[0], want[1], want[2]); ++bad;
        }
    }
    printf(bad ? "00c1c330_TEST FAIL\n" : "00c1c330_TEST PASS\n");
    return bad ? 1 : 0;
}
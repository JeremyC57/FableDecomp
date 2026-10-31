// Test: C3DMesh2::CalculateBoneKeyframes -- each bone's keyframe matrix is the inverse of its localization matrix,
// post-multiplied by the parent's localization matrix when the bone has a parent (Parent != -1).
#include <cstdio>
#include <cmath>
struct M34 { float E[4][3]; };
static void Inverse(const M34& m, M34& r)
{
    const float (*e)[3] = m.E;
    float det = (e[2][1] * e[0][2] - e[2][2] * e[0][1]) * e[1][0] + (e[1][2] * e[0][1] - e[1][1] * e[0][2]) * e[2][0]
              + (e[1][1] * e[2][2] - e[2][1] * e[1][2]) * e[0][0];
    float inv = 1.0f / det;
    r.E[0][0] = (e[1][1] * e[2][2] - e[2][1] * e[1][2]) * inv;
    r.E[1][0] = (e[1][2] * e[2][0] - e[1][0] * e[2][2]) * inv;
    r.E[2][0] = (e[2][1] * e[1][0] - e[1][1] * e[2][0]) * inv;
    r.E[0][1] = (e[2][1] * e[0][2] - e[2][2] * e[0][1]) * inv;
    r.E[1][1] = (e[0][0] * e[2][2] - e[0][2] * e[2][0]) * inv;
    r.E[2][1] = (e[2][0] * e[0][1] - e[0][0] * e[2][1]) * inv;
    r.E[0][2] = (e[1][2] * e[0][1] - e[1][1] * e[0][2]) * inv;
    r.E[1][2] = (e[1][0] * e[0][2] - e[0][0] * e[1][2]) * inv;
    r.E[2][2] = (e[0][0] * e[1][1] - e[1][0] * e[0][1]) * inv;
    for (int c = 0; c < 3; ++c)
        r.E[3][c] = -(r.E[0][c] * e[3][0] + r.E[2][c] * e[3][2] + r.E[1][c] * e[3][1]);
}
static void Mul(M34& a, const M34& b)
{
    M34 t = a;
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 3; ++c)
        a.E[r][c] = t.E[r][0] * b.E[0][c] + t.E[r][1] * b.E[1][c] + t.E[r][2] * b.E[2][c] + (r == 3 ? b.E[3][c] : 0.0f);
}
struct Bone { long Parent; M34 Loc; };
static M34 g_out[3];
static void CalculateBoneKeyframes(const Bone* bones, unsigned long n)
{
    for (unsigned long i = 0; i < n; ++i) {
        const Bone& bone = bones[i];
        M34 t; Inverse(bone.Loc, t);
        if (bone.Parent != -1) Mul(t, bones[bone.Parent].Loc);
        g_out[i] = t;
    }
}
static M34 Make(float ang, float tx, float ty, float tz, float s)
{
    M34 m; float c = (float)cos(ang) * s, sn = (float)sin(ang) * s;
    m.E[0][0] = c; m.E[0][1] = sn; m.E[0][2] = 0; m.E[1][0] = -sn; m.E[1][1] = c; m.E[1][2] = 0;
    m.E[2][0] = 0; m.E[2][1] = 0; m.E[2][2] = s; m.E[3][0] = tx; m.E[3][1] = ty; m.E[3][2] = tz; return m;
}
int main()
{
    Bone b[3];
    b[0].Parent = -1; b[0].Loc = Make(0.5f, 1, 2, 3, 2.0f);
    b[1].Parent = 0;  b[1].Loc = Make(-0.3f, 4, 5, 6, 1.0f);
    b[2].Parent = 0;  b[2].Loc = b[0].Loc;
    CalculateBoneKeyframes(b, 3);
    bool ok = true;
    M34 id = g_out[0]; Mul(id, b[0].Loc);           // inverse * original = identity
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 3; ++c)
        if (fabs(id.E[r][c] - ((r == c) ? 1.0f : 0.0f)) > 1e-4) ok = false;
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 3; ++c)   // bone 2: inverse(L0) * L0 = identity
        if (fabs(g_out[2].E[r][c] - ((r == c) ? 1.0f : 0.0f)) > 1e-4) ok = false;
    if (fabs(g_out[0].E[0][0] - (float)cos(0.5) / 2) > 1e-5) ok = false;   // scaled rotation inverse
    printf(ok ? "00a8a9d0_TEST PASS\n" : "00a8a9d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
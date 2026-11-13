// Behaviour test for 0x00875B10 NCameraModes::CTools::TestMeshForOccupationAndGetType: a copy of the row logic over a model mesh
// whose intersection answers come from a script (and which records the probe end points).
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V operator+(const V& o) const { return V(X + o.X, Y + o.Y, Z + o.Z); }
    V operator*(float s) const { V r(X * s, Y * s, Z * s); return r; }
    float Sq() const { return X * X + Y * Y + Z * Z; }
    void Normalise() { float f = 1.0f / (float)std::sqrt(Z * Z + Y * Y + X * X); X *= f; Y *= f; Z *= f; } };
struct Isect { V Pos; V Normal; float Dist; };
struct Mesh {
    V xe, ye; int calls; const bool* answers; V tos[8];
    bool Probe(const V&, const V& to, Isect& out) { tos[calls] = to; out.Pos = V(0, 0, 100.0f + calls); return answers[calls++]; }
};
static int Test(Mesh& mesh, const V& position, const V& up, float& height)
{
    float size = mesh.xe.Sq() + mesh.ye.Sq();
    V normal = up; normal.Normalise();
    V side(normal.Y, -normal.X, 0.0f); side.Normalise();
    normal = normal * size; side = side * size;
    long hits = 0; Isect isect;
    for (long i = 0; i < 2; ++i) {
        V to = position + V(0.0f, 0.0f, (float)(long)((0.5f - (float)i) * 2.0f) * size * 3.0f);
        if (!mesh.Probe(position, to, isect)) return 0;
        height = isect.Pos.Z; ++hits;
    }
    for (long j = 0; j < 4; ++j) {
        double angle = (float)j * 0.25f * 6.283185307179586;
        V to = position + normal * (float)std::cos(angle) + side * (float)std::sin(angle);
        if (mesh.Probe(position, to, isect)) ++hits;
    }
    switch (hits) { case 5: return 1; case 6: return 1; default: return 0; }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
int main()
{
    V pos(1.0f, 2.0f, 3.0f), up(0.0f, 2.0f, 0.0f);
    float h = -1.0f;
    const bool all[6] = { true, true, true, true, true, true };
    Mesh m; m.xe = V(1, 0, 0); m.ye = V(0, 1, 0); m.calls = 0; m.answers = all;   // size = 2
    if (Test(m, pos, up, h) != 1 || m.calls != 6 || !Near(h, 101.0f)) { std::printf("00875b10_TEST FAIL\n"); return 1; }
    if (!Near(m.tos[0].Z, 3.0f + 6.0f) || !Near(m.tos[1].Z, 3.0f - 6.0f) || !Near(m.tos[0].X, 1.0f)) { std::printf("00875b10_TEST FAIL\n"); return 1; }
    // first rotated probe: angle 0 -> position + normal*size = (1, 2+2, 3)
    if (!Near(m.tos[2].X, 1.0f) || !Near(m.tos[2].Y, 4.0f)) { std::printf("00875b10_TEST FAIL\n"); return 1; }
    // quarter turn: + side*size, side = (ny, -nx, 0) = (1, 0, 0) -> (3, 2, 3)
    if (!Near(m.tos[3].X, 3.0f) || !Near(m.tos[3].Y, 2.0f)) { std::printf("00875b10_TEST FAIL\n"); return 1; }
    const bool five[6] = { true, true, false, true, true, true };
    m.calls = 0; m.answers = five;
    if (Test(m, pos, up, h) != 1) { std::printf("00875b10_TEST FAIL\n"); return 1; }
    const bool four[6] = { true, true, false, false, true, true };
    m.calls = 0; m.answers = four;
    if (Test(m, pos, up, h) != 0) { std::printf("00875b10_TEST FAIL\n"); return 1; }
    const bool downMiss[6] = { true, false, true, true, true, true };
    m.calls = 0; m.answers = downMiss;
    if (Test(m, pos, up, h) != 0 || m.calls != 2) { std::printf("00875b10_TEST FAIL\n"); return 1; }
    std::printf("00875b10_TEST PASS\n");
    return 0;
}
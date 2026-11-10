// Behaviour test for 0x0063AB90 CWorldMap::GetThemeFaceNormalAt (host model of the row's body; stub height lookup and dimensions).
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator^(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    void Normalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
};
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
class C2DCoordI { public: long X, Y; C2DCoordI() {} C2DCoordI(long x, long y) : X(x), Y(y) {} };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
static int g_lookups;
class CWorldMap { public:
    C2DBoxI Dims;
    float (*Height)(long, long);
    const C2DBoxI& GetDimensions() const { return Dims; }
    float GetThemeSizeZAtBlock(const C2DCoordI& c) const { ++g_lookups; return Height(c.X, c.Y); }
    C3DVector GetThemeFaceNormalAt(const C3DVector& pos) const;
};
C3DVector CWorldMap::GetThemeFaceNormalAt(const C3DVector& pos) const
{
    C2DVector p(pos.X, pos.Y);
    long ix = (long)p.X;
    long iy;
    float h00;
    {
        C2DCoordI cell;
        cell.X = ix;
        iy = (long)p.Y;
        cell.Y = iy;
        h00 = GetThemeSizeZAtBlock(cell);
    }
    float h10 = GetThemeSizeZAtBlock(C2DCoordI(ix + 1 < GetDimensions().BRX - 1 ? ix + 1 : GetDimensions().BRX - 1, iy));
    float h01 = GetThemeSizeZAtBlock(C2DCoordI(ix, iy + 1 < GetDimensions().BRY - 1 ? iy + 1 : GetDimensions().BRY - 1));
    float h11 = GetThemeSizeZAtBlock(C2DCoordI(ix + 1 < GetDimensions().BRX - 1 ? ix + 1 : GetDimensions().BRX - 1,
                                               iy + 1 < GetDimensions().BRY - 1 ? iy + 1 : GetDimensions().BRY - 1));
    float dx, dy;
    if ((ix + iy) % 2 == 1) {
        if ((p.X - ix) + (p.Y - iy) > 1.0f) { dx = h11 - h01; dy = h11 - h10; }
        else { dx = h10 - h00; dy = h01 - h00; }
    } else if ((p.Y - iy) > (p.X - ix)) { dx = h11 - h01; dy = h01 - h00; }
    else { dx = h10 - h00; dy = h11 - h10; }
    C3DVector normal = C3DVector(1.0f, 0.0f, dx) ^ C3DVector(0.0f, 1.0f, dy);
    normal.Normalise();
    return normal;
}
static float Plane(long x, long y) { return 2.0f * x + 3.0f * y; }
static float Bump(long x, long y) { return (x == 6 && y == 6) ? 4.0f : 0.0f; }   // one raised corner
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
#define FAIL(n) do { std::printf("0063ab90_TEST FAIL %d\n", n); return 1; } while (0)
int main()
{
    CWorldMap m; m.Dims.TLX = 0; m.Dims.TLY = 0; m.Dims.BRX = 64; m.Dims.BRY = 64;
    m.Height = Plane;
    // a plane gives the same normal in every triangle: (-2, -3, 1) normalised
    float l = (float)std::sqrt(14.0f);
    float pts[4][2] = { { 5.2f, 5.3f }, { 5.7f, 5.6f }, { 4.25f, 5.75f }, { 4.75f, 5.2f } };
    for (int i = 0; i < 4; ++i) {
        g_lookups = 0;
        C3DVector n = m.GetThemeFaceNormalAt(C3DVector(pts[i][0], pts[i][1], 0.0f));
        if (g_lookups != 4) FAIL(1);
        if (!Near(n.X, -2.0f / l) || !Near(n.Y, -3.0f / l) || !Near(n.Z, 1.0f / l)) FAIL(2);
    }
    // cell (5,5) is even: the lower-right triangle (y-frac <= x-frac) uses h10-h00 and h11-h10; h11 = (6,6) raised
    m.Height = Bump;
    C3DVector a = m.GetThemeFaceNormalAt(C3DVector(5.7f, 5.2f, 0.0f));   // dx = 0, dy = 4
    float la = (float)std::sqrt(17.0f);
    if (!Near(a.X, 0.0f) || !Near(a.Y, -4.0f / la) || !Near(a.Z, 1.0f / la)) FAIL(3);
    C3DVector b = m.GetThemeFaceNormalAt(C3DVector(5.2f, 5.7f, 0.0f));   // upper-left: dx = h11 - h01 = 4, dy = h01 - h00 = 0
    if (!Near(b.X, -4.0f / la) || !Near(b.Y, 0.0f) || !Near(b.Z, 1.0f / la)) FAIL(4);
    // the far edge clamps the +1 neighbours to BRX - 1 / BRY - 1
    m.Height = Plane;
    C3DVector e = m.GetThemeFaceNormalAt(C3DVector(63.5f, 63.4f, 0.0f));   // all four lookups at (63,63): flat
    if (!Near(e.X, 0.0f) || !Near(e.Y, 0.0f) || !Near(e.Z, 1.0f)) FAIL(5);
    std::printf("0063ab90_TEST PASS\n");
    return 0;
}
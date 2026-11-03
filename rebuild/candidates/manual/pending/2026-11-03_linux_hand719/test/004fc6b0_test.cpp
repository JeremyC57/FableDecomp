#include <cstdio>
// Model of CWorldMap::DrawIsPosInMap: the CIDrawWorldMap override forwards to IsPosInMap(pos).
struct Vec { float x, y, z; };
struct IDraw { virtual bool DrawIsPosInMap(const Vec& p) const = 0; };
struct WorldMap : IDraw {
    float w, h;
    bool IsPosInMap(const Vec& p) const { return p.x >= 0.0f && p.y >= 0.0f && p.x < w && p.y < h; }
    virtual bool DrawIsPosInMap(const Vec& p) const { return IsPosInMap(p); }
};
int main()
{
    WorldMap m; m.w = 100.0f; m.h = 50.0f; const IDraw& d = m;
    Vec in = { 10.0f, 10.0f, 0.0f }, out = { 10.0f, 60.0f, 0.0f };
    if (!d.DrawIsPosInMap(in) || d.DrawIsPosInMap(out)) { std::printf("004fc6b0_TEST FAIL\n"); return 1; }
    std::printf("004fc6b0_TEST PASS\n"); return 0;
}

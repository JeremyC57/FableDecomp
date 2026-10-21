// Behaviour test for 0x0063E480 (CWorldMap::GetHeightZBelowPos): start from the ground height, cast pos -> ground
// against each nearby thing's mesh and keep the highest hit Z.
#include <cstdio>
#include <vector>
struct V3 { float X, Y, Z; };
struct Thing { bool hasMesh; float top; long x, y; };   // a flat mesh at height `top`
static std::vector<Thing> g_things;
static float g_ground = 1.0f;
static int g_meshDtors = 0;
struct Mesh {
    const Thing* t;
    ~Mesh() { ++g_meshDtors; }
    bool Intersect(const V3* from, const V3* to, V3* out) const
    {
        if (!t->hasMesh || t->top > from->Z || t->top < to->Z) return false;
        out->X = from->X; out->Y = from->Y; out->Z = t->top; return true;
    }
};
static float GetHeightZBelowPos(const V3& pos)
{
    float ground = g_ground;
    float best = ground;
    std::vector<const Thing*> things;
    long x = (long)pos.X, y = (long)pos.Y;
    for (size_t i = 0; i < g_things.size(); ++i)
        if (g_things[i].x >= x - 2 && g_things[i].x <= x + 2 && g_things[i].y >= y - 2 && g_things[i].y <= y + 2)
            things.push_back(&g_things[i]);
    for (size_t i = 0; i < things.size(); ++i) {
        V3 below = { pos.X, pos.Y, ground };
        V3 data;
        bool hit;
        { Mesh m; m.t = things[i]; hit = m.Intersect(&pos, &below, &data); }
        if (hit && data.Z > best) best = data.Z;
    }
    return best;
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    V3 p = { 10.5f, 10.5f, 20.0f };
    CHECK(GetHeightZBelowPos(p) == 1.0f);                         // nothing nearby: the ground
    Thing a = { true, 5.0f, 11, 11 }; g_things.push_back(a);
    Thing b = { true, 7.0f, 9, 12 };  g_things.push_back(b);
    Thing c = { true, 15.0f, 20, 20 }; g_things.push_back(c);     // outside the 4x4 box
    Thing d = { false, 12.0f, 10, 10 }; g_things.push_back(d);    // no mesh hit
    Thing e = { true, 25.0f, 10, 10 }; g_things.push_back(e);     // above the position: missed
    g_meshDtors = 0;
    CHECK(GetHeightZBelowPos(p) == 7.0f);
    CHECK(g_meshDtors == 4);                                      // one mesh object per thing in the box
    g_ground = 8.0f; CHECK(GetHeightZBelowPos(p) == 8.0f);        // ground above every mesh
    if (g_fail) { std::printf("0063e480_TEST FAIL\n"); return 1; }
    std::printf("0063e480_TEST PASS\n");
    return 0;
}

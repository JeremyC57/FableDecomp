#include <cstdio>
#include <cmath>
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
// A wall at x = 3: a segment hits when it reaches x >= 3.
struct CWorldMap {
    C3DVector GetGroundFaceNormalAt(const C3DVector* p) const { return C3DVector(-1.0f, 0.0f, p->Z); }
};
static CWorldMap g_map;
struct CWorldMapOwner { CWorldMap* GetWorldMap() { return &g_map; } };
struct CToolsOwner { CWorldMapOwner* WorldMapOwner; };
static int g_tests = 0;
typedef bool (*TWorldMapTest)(CWorldMap* map, const C3DVector* from, const C3DVector* to, bool flag);
static bool WallTest(CWorldMap*, const C3DVector* from, const C3DVector* to, bool flag)
{
    ++g_tests;
    return flag && to->X >= 3.0f && from->X < 3.0f;
}
struct CTools {
    CToolsOwner* Owner;
    bool GetIntersectionWithWorldMap(const C3DVector& start, const C3DVector& end, C3DVector* position, C3DVector* normal,
                                     bool flag, TWorldMapTest test);
};

bool CTools::GetIntersectionWithWorldMap(const C3DVector& start, const C3DVector& end, C3DVector* position, C3DVector* normal,
                                         bool flag, TWorldMapTest test)
{
    float step = 0.5f;
    CWorldMap* map = Owner->WorldMapOwner->GetWorldMap();
    C3DVector direction = end - start;
    float lengthSquared = direction.GetLengthSquared();
    if (lengthSquared < 0.0001f)
        return false;
    float length = (float)sqrt(lengthSquared);
    C3DVector from = start;
    C3DVector to = end;
    long hits = 0;
    long steps = 0;
    float invLength = 1.0f / length;
    direction.X *= invLength;
    direction.Y *= invLength;
    direction.Z *= invLength;
    C3DVector remaining;
    C3DVector initial = from - to;
    do {
        ++steps;
        C3DVector next = from + direction * step;
        if (test(map, &from, &next, flag)) {
            step *= 0.1f;
            to = next;
            remaining = initial;
            *position = (to + from) * 0.5f;
            *normal = map->GetGroundFaceNormalAt(position);
            ++hits;
        } else {
            from = next;
            remaining = from - to;
        }
    } while (steps < 100 && Dot(remaining, initial) > 0.0f && hits < 4);
    return hits > 0 ? true : false;
}

int main()
{
    CWorldMapOwner o; CToolsOwner to; to.WorldMapOwner = &o; CTools t; t.Owner = &to;
    C3DVector pos(0, 0, 0), nrm(0, 0, 0);
    CHECK(!t.GetIntersectionWithWorldMap(C3DVector(1, 1, 1), C3DVector(1, 1, 1), &pos, &nrm, true, WallTest) && g_tests == 0);
    CHECK(t.GetIntersectionWithWorldMap(C3DVector(0, 0, 0), C3DVector(10, 0, 0), &pos, &nrm, true, WallTest));
    CHECK(pos.X > 2.9f && pos.X < 3.6f && nrm.X == -1.0f && g_tests <= 100);
    g_tests = 0;
    CHECK(!t.GetIntersectionWithWorldMap(C3DVector(0, 0, 0), C3DVector(2, 0, 0), &pos, &nrm, true, WallTest));
    CHECK(g_tests == 4);
    g_tests = 0;
    CHECK(!t.GetIntersectionWithWorldMap(C3DVector(0, 0, 0), C3DVector(10, 0, 0), &pos, &nrm, false, WallTest));
    if (g_fail) { std::printf("008751d0_TEST FAIL\n"); return 1; }
    std::printf("008751d0_TEST PASS\n");
    return 0;
}
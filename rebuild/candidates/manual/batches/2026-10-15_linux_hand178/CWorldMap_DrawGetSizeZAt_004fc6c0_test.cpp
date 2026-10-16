#include <cstdio>
#include <cstring>
struct V { float X, Y, Z; };
struct Map { bool loaded; long TLX, TLY; float SizeZ(const V* p, float f) const { return p->X + p->Y * 10.0f + p->Z * 100.0f + f; } };
static float SizeZAt(const Map* maps, long map, const V& pos, float f)
{
    if (map && maps[map].loaded) { V local; local.X = pos.X - maps[map].TLX; local.Y = pos.Y - maps[map].TLY; local.Z = pos.Z; return maps[map].SizeZ(&local, f); }
    return 0.0f;
}
static bool Run() { Map m[2] = { { true, 0, 0 }, { true, 100, 200 } }; V p = { 101.0f, 202.0f, 3.0f }; if (SizeZAt(m, 1, p, 0.5f) != 321.5f) return false; if (SizeZAt(m, 0, p, 0.5f) != 0.0f) return false; m[1].loaded = false; return SizeZAt(m, 1, p, 0.5f) == 0.0f; }
int main()
{
    if (!Run()) { std::printf("004fc6c0_TEST FAIL\n"); return 1; }
    std::printf("004fc6c0_TEST PASS\n");
    return 0;
}

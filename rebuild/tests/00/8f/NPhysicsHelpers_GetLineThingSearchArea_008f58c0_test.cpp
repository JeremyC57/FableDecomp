// behaviour test for 0x008F58C0 NPhysicsHelpers::GetLineThingSearchArea
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct Box { long TLX, TLY, BRX, BRY; };
static Box Area(const V& a, const V& b, float radius)
{
    V c = { (a.X + b.X) * 0.5f, (a.Y + b.Y) * 0.5f, (a.Z + b.Z) * 0.5f };
    V l = { b.X - a.X, b.Y - a.Y, b.Z - a.Z };
    long cx = (long)c.X, cy = (long)c.Y;
    long r = (long)((float)sqrt(l.X * l.X + l.Y * l.Y + l.Z * l.Z) + radius * 2.0f);
    Box box; box.BRX = cx + r; box.BRY = cy + r; box.TLX = cx - r; box.TLY = cy - r;
    return box;
}
int main()
{
    V a = { 0, 0, 0 }, b = { 6, 8, 0 };
    Box x = Area(a, b, 1.0f);
    bool ok = x.TLX == -9 && x.TLY == -8 && x.BRX == 15 && x.BRY == 16;
    std::printf(ok ? "008f58c0_TEST PASS\n" : "008f58c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
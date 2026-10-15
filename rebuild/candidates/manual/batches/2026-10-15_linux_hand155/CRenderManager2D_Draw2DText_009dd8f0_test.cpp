// Behaviour test for 0x009DD8F0: Draw2DText forwards (x, y, 0), scale 1 and a copy of the string to AddText and returns true.
#include <cstdio>
static int g_live;
struct WStr { int id; WStr(int i) : id(i) { ++g_live; } WStr(const WStr& o) : id(o.id) { ++g_live; } ~WStr() { --g_live; } };
struct V2 { float X, Y; };
struct V3 { float X, Y, Z; V3(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct R { V3 lastPos; float lastScale; int lastText; int lastFlags; int liveDuring; R() : lastPos(0, 0, 0) {}
  void AddText(const V3& p, float s, const WStr& t, void*, const unsigned int&, unsigned int f) { lastPos = p; lastScale = s; lastText = t.id; lastFlags = f; liveDuring = g_live; }
  bool Draw2DText(const V2& pos, WStr text, void* font, const unsigned int& colour, unsigned int flags) {
    V3 sp(pos.X, pos.Y, 0.0f); WStr s(text); AddText(sp, 1.0f, s, font, colour, flags); return true; } };
int main()
{
    R r; V2 p = {3.0f, 4.0f}; unsigned int col = 0xffffffffu;
    bool res = r.Draw2DText(p, WStr(7), 0, col, 5);
    bool ok = res && r.lastPos.X == 3.0f && r.lastPos.Y == 4.0f && r.lastPos.Z == 0.0f && r.lastScale == 1.0f && r.lastText == 7 && r.lastFlags == 5
           && r.liveDuring == 2 && g_live == 0;
    if (!ok) { std::printf("009dd8f0_TEST FAIL\n"); return 1; }
    std::printf("009dd8f0_TEST PASS\n");
    return 0;
}

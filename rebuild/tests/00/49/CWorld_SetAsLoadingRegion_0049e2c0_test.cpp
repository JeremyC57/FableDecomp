#include <cstdio>
#include <cstring>
struct V { float X, Y, Z; };
struct W { bool movie; long status; V pos; float angle; bool tele, door, wait;
  void Set(const V& p, float a, bool t, bool force, bool d) { if (!force && movie) return; if (status == 0 || status >= 8) { pos = p; angle = a; tele = t; door = d; wait = false; status = 1; } } };
static bool Run()
{
    W w; w.movie = true; w.status = 0; w.wait = true; V p = { 1, 2, 3 };
    w.Set(p, 0.5f, true, false, true); if (w.status != 0) return false;
    w.Set(p, 0.5f, true, true, true); if (w.status != 1 || w.pos.Y != 2 || !w.tele || !w.door || w.wait || w.angle != 0.5f) return false;
    w.status = 3; w.Set(p, 1.0f, false, true, false); if (w.angle != 0.5f) return false;
    w.status = 8; w.movie = false; w.Set(p, 1.0f, false, false, false); return w.status == 1 && w.angle == 1.0f;
}
int main()
{
    if (!Run()) { std::printf("0049e2c0_TEST FAIL\n"); return 1; }
    std::printf("0049e2c0_TEST PASS\n");
    return 0;
}
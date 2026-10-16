#include <cstdio>
#include <cstring>
struct Box { long TLX, TLY, BRX, BRY;
  void ClipTo(const Box& o) {
    if (TLX < o.TLX) TLX = o.TLX; else if (TLX > o.BRX) TLX = o.BRX;
    if (TLY < o.TLY) TLY = o.TLY; else if (TLY > o.BRY) TLY = o.BRY;
    if (BRX < o.TLX) BRX = o.TLX; else if (BRX > o.BRX) BRX = o.BRX;
    if (BRY < o.TLY) BRY = o.TLY; else if (BRY > o.BRY) BRY = o.BRY; } };
static Box Clip(const Box& mapArea, const Box& area) { Box b = area; Box m = mapArea; b.ClipTo(m); b.TLX -= mapArea.TLX; b.BRX -= mapArea.TLX; b.TLY -= mapArea.TLY; b.BRY -= mapArea.TLY; return b; }
static bool Run() { Box m = { 100, 200, 164, 264 }, a = { 90, 210, 120, 300 }; Box r = Clip(m, a); return r.TLX == 0 && r.TLY == 10 && r.BRX == 20 && r.BRY == 64; }
int main()
{
    if (!Run()) { std::printf("004fc4d0_TEST FAIL\n"); return 1; }
    std::printf("004fc4d0_TEST PASS\n");
    return 0;
}
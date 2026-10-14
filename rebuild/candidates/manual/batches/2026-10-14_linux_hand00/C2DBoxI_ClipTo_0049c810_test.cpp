#include <cstdio>
struct C2DBoxI { long TLX; long TLY; long BRX; long BRY; void ClipTo(const C2DBoxI& other); };
void C2DBoxI::ClipTo(const C2DBoxI& other)
{
    if (TLX < other.TLX) TLX = other.TLX; else if (TLX > other.BRX) TLX = other.BRX;
    if (TLY < other.TLY) TLY = other.TLY; else if (TLY > other.BRY) TLY = other.BRY;
    if (BRX < other.TLX) BRX = other.TLX; else if (BRX > other.BRX) BRX = other.BRX;
    if (BRY < other.TLY) BRY = other.TLY; else if (BRY > other.BRY) BRY = other.BRY;
}
int main()
{
    C2DBoxI b = { -10, 5, 50, 90 };
    C2DBoxI clip = { 0, 0, 40, 40 };
    b.ClipTo(clip);
    if (b.TLX != 0 || b.TLY != 5 || b.BRX != 40 || b.BRY != 40) { std::printf("0049c810_TEST FAIL\n"); return 1; }
    std::printf("0049c810_TEST PASS\n");
    return 0;
}

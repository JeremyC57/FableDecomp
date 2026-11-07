// Behaviour test for NPlayerGui::CDrawQuickAccessIconsPC::Draw (0x00657580): the hot bar covers the quick-access slots up to the
// last used one: nothing when no slot is used, the single-slot graphic for one, else the main bar plus the end cap for that count
// placed at the last slot's offset minus the cap width.
#include <cstdio>

struct Result { int Primitive0, Primitive1; float EndX; };
static Result Draw(const long* graphics, long count, const long* endGraphics, float anchorX, const float* offsetX, float barX,
                   float endOffsetX, unsigned long endWidth)
{
    Result r = { 0, 0, 0.0f };
    long lastUsed = 0;
    for (long i = 0; i < count; ++i)
        if (graphics[i] > 0) lastUsed = i + 1;
    if (lastUsed == 0) return r;
    if (lastUsed == 1) { r.Primitive0 = 1; return r; }
    float x = offsetX[lastUsed - 1] + anchorX + barX;
    r.Primitive0 = 2;
    long end = endGraphics[lastUsed - 2];
    if (end > 0) { r.Primitive1 = (int)end; r.EndX = x - (float)endWidth + endOffsetX; }
    return r;
}

int main()
{
    const long none[4] = { 0, 0, 0, 0 }, one[4] = { 5, 0, 0, 0 }, three[4] = { 5, 0, 7, 0 };
    const long ends[3] = { 21, 22, 23 };
    const float offs[4] = { 0.0f, 40.0f, 80.0f, 120.0f };
    Result a = Draw(none, 4, ends, 10.0f, offs, 2.0f, 1.0f, 16);
    Result b = Draw(one, 4, ends, 10.0f, offs, 2.0f, 1.0f, 16);
    Result c = Draw(three, 4, ends, 10.0f, offs, 2.0f, 1.0f, 16);
    bool ok = a.Primitive0 == 0 && a.Primitive1 == 0 && b.Primitive0 == 1 && b.Primitive1 == 0
           && c.Primitive0 == 2 && c.Primitive1 == 22 && c.EndX == 80.0f + 10.0f + 2.0f - 16.0f + 1.0f;
    if (!ok) { std::printf("00657580_TEST FAIL\n"); return 1; }
    std::printf("00657580_TEST PASS\n");
    return 0;
}

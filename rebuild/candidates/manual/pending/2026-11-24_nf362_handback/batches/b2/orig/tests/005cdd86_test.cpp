// behaviour test for CTCInventoryStats::CreateExperienceSliders 0x005CDD86: four sliders (general, strength, skill, will) at
// y = 0, s+25, 2(s+25), 3(s+25); each gets value = the stat and level = current ? value : the original experience
#include <cstdio>
#include <vector>
struct Slider { long y, value, level; unsigned char r, g, b; };
static long g_stats[4] = { 120, 30, 45, 60 };
static long g_original[4] = { 100, 20, 40, 50 };
static std::vector<Slider> Create(long spacing, bool current)
{
    std::vector<Slider> sliders;
    static const unsigned char colours[4][3] = { { 0, 0xb4, 0 }, { 0xb4, 0, 0 }, { 0xb4, 0xb4, 0 }, { 0, 0, 0xb4 } };
    long value, level;
    for (int i = 0; i < 4; ++i) {
        value = g_stats[i];
        level = current ? value : g_original[i];
        Slider s = { (spacing + 0x19) * i, value, level, colours[i][0], colours[i][1], colours[i][2] };
        sliders.push_back(s);
    }
    return sliders;
}
int main()
{
    std::vector<Slider> a = Create(5, true), b = Create(5, false);
    bool ok = a.size() == 4 && b.size() == 4;
    for (int i = 0; ok && i < 4; ++i) {
        ok = a[i].y == 30 * i && a[i].value == g_stats[i] && a[i].level == g_stats[i] && b[i].level == g_original[i] &&
             b[i].value == g_stats[i];
    }
    ok = ok && a[1].r == 0xb4 && a[0].g == 0xb4 && a[3].b == 0xb4;
    std::printf(ok ? "005cdd86_TEST PASS\n" : "005cdd86_TEST FAIL\n");
    return ok ? 0 : 1;
}
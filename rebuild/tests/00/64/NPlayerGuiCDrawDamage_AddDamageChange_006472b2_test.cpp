// Behaviour test for 0x006472B2 NPlayerGui::CDrawDamage::AddDamageChange: queue positive changes scaled by ten and rounded, shown for one second of frames (standalone copy of the logic)
#include <cstdio>
#include <list>
#include <cmath>
struct Value { long Duration; float Change; bool Hero; };
static float g_fps = 30.0f;
long Round(float v) { return (long)std::floor(v + 0.5f); }   // the retail helper is a bare fistp (round to nearest)
struct Damage { std::list<Value> Values;
    void Add(float change, bool hero) { if (change > 0.0f) { change *= 10.0f; long r = Round(change); Value v; v.Duration = (long)g_fps; v.Change = (float)r; v.Hero = hero; Values.push_front(v); } } };

int main()
{
    Damage d; d.Add(0.0f, true); d.Add(-1.0f, true); if (!d.Values.empty()) { std::printf("006472b2_TEST FAIL\n"); return 1; }
    d.Add(1.26f, true); d.Add(0.31f, false);
    if (d.Values.size() != 2) { std::printf("006472b2_TEST FAIL\n"); return 1; }
    if (d.Values.front().Change != 3.0f || d.Values.front().Hero || d.Values.front().Duration != 30) { std::printf("006472b2_TEST FAIL\n"); return 1; }
    if (d.Values.back().Change != 13.0f || !d.Values.back().Hero) { std::printf("006472b2_TEST FAIL\n"); return 1; }
    std::printf("006472b2_TEST PASS\n");
    return 0;
}
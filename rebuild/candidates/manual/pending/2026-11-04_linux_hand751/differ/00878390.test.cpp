// Behaviour test for NCameraModes::CTools::GetThingsOfType<CThingBuilding> (0x00878390, parked DIFFER row): a model of the row -- things of the type inside the box and overlapping the sphere are collected.
#include <stdio.h>
#include <list>
struct B { long Type; float X, Y, R; };
static void Get(const B* all, int n, std::list<const B*>& out, long type, float x, float y, float radius)
{ long ix = (long)x, iy = (long)y, r = (long)radius; for (int i = 0; i < n; ++i) { const B& b = all[i]; if (b.X < ix - r || b.X > ix + r || b.Y < iy - r || b.Y > iy + r) continue;
    float dx = b.X - x, dy = b.Y - y; if (b.Type == type && dx * dx + dy * dy < (b.R + radius) * (b.R + radius)) out.push_back(&b); } }
static int Check() { B all[3] = { { 3, 1, 1, 1 }, { 2, 1, 1, 1 }, { 3, 50, 50, 1 } }; std::list<const B*> out; Get(all, 3, out, 3, 0, 0, 10); return out.size() == 1 && out.front() == &all[0]; }
int main()
{
    int ok = Check();
    printf(ok ? "00878390_TEST PASS\n" : "00878390_TEST FAIL\n");
    return ok ? 0 : 1;
}

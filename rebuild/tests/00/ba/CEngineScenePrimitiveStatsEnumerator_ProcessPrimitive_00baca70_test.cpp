// Behaviour test for 0x00BACA70 CEngineScenePrimitiveStatsEnumerator::ProcessPrimitive (model copy).
#include <cstdio>

struct V3 { float X, Y, Z; };
struct Box3 { V3 Min, Max; };
struct Prim { bool hasPos; V3 pos; bool hasBox; Box3 box; int counted; };
struct Area { float MinX, MinY, MaxX, MaxY; };
static void Process(const Area& a, Prim& p)
{
    if (p.hasPos) {
        if (!(p.pos.X >= a.MinX && p.pos.X < a.MaxX && p.pos.Y >= a.MinY && p.pos.Y < a.MaxY)) return;
    } else {
        if (!p.hasBox) return;
        float lx = p.box.Min.X > a.MinX ? p.box.Min.X : a.MinX, hx = p.box.Max.X < a.MaxX ? p.box.Max.X : a.MaxX;
        if (!(lx < hx)) return;
        float ly = p.box.Min.Y > a.MinY ? p.box.Min.Y : a.MinY, hy = p.box.Max.Y < a.MaxY ? p.box.Max.Y : a.MaxY;
        if (!(ly < hy)) return;
    }
    ++p.counted;
}

int main()
{
    Area a = { 0, 0, 10, 10 };
    Prim p; p.counted = 0; p.hasPos = true; p.pos.X = 5; p.pos.Y = 5; p.hasBox = false;
    Process(a, p);
    p.pos.X = 10; Process(a, p);                    // half-open: x == max is outside
    if (p.counted != 1) { std::printf("00baca70_TEST FAIL\n"); return 1; }
    p.hasPos = false; p.hasBox = true;
    p.box.Min.X = 8; p.box.Min.Y = -5; p.box.Max.X = 20; p.box.Max.Y = 1; Process(a, p);   // overlaps
    p.box.Min.X = 10; Process(a, p);                                                       // touches only
    p.hasBox = false; Process(a, p);
    if (p.counted != 2) { std::printf("00baca70_TEST FAIL\n"); return 1; }
    std::printf("00baca70_TEST PASS\n");
    return 0;
}
// Behaviour test for CCreatureAction_DragonRoar::PerformAction (0x008F5490): the barrier starts at the mouth dummy's
// X/Y at ground Z, pushed two units along the facing, ends at the dragon; a created barrier gets a 3 s lifetime and
// the dragon as a passenger.
#include <cstdio>
#include <vector>

struct C3DVector { float X, Y, Z; };
struct CThing;
struct CDriver { float Lifetime; std::vector<CThing*> Passengers; };
struct CThing { C3DVector Pos, Mouth, Facing; CDriver* Driver; };
static C3DVector g_start, g_end; static CThing* g_barrier;
static CThing* CreateBarrier(const C3DVector& s, const C3DVector& e, float r) { g_start = s; g_end = e; return r == 2.0f ? g_barrier : 0; }
static void PerformAction(CThing* dragon)
{
    C3DVector p = dragon->Pos;
    p = dragon->Mouth;                       // GetDummyObject fills the position
    p.Z = dragon->Pos.Z;
    p.X += dragon->Facing.X * 2.0f; p.Y += dragon->Facing.Y * 2.0f; p.Z += dragon->Facing.Z * 2.0f;
    CThing* barrier = CreateBarrier(p, dragon->Pos, 2.0f);
    if (barrier) {
        CDriver* d = barrier->Driver;
        d->Lifetime = 3.0f;
        d->Passengers.push_back(dragon);
    }
}

int main()
{
    CDriver drv; drv.Lifetime = 0;
    CThing wall = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, &drv };
    CThing dragon = { { 10, 20, 5 }, { 11, 21, 9 }, { 1, 0, 0 }, 0 };
    g_barrier = &wall;
    PerformAction(&dragon);
    bool ok = g_start.X == 13 && g_start.Y == 21 && g_start.Z == 5 && g_end.X == 10 && drv.Lifetime == 3.0f
        && drv.Passengers.size() == 1 && drv.Passengers[0] == &dragon;
    g_barrier = 0; PerformAction(&dragon);
    ok = ok && drv.Passengers.size() == 1;
    if (!ok) { std::printf("008f5490_TEST FAIL\n"); return 1; }
    std::printf("008f5490_TEST PASS\n");
    return 0;
}

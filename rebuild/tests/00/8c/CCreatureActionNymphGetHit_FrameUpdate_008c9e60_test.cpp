// Behaviour test: CCreatureAction_NymphGetHit::FrameUpdate (0x008C9E60) -- after the base update, when the "DUMMY_CENTRE"
// dummy is found, move the creature so the dummy returns to its initial position and stop it moving.
#include <cstdio>
#include <cstring>
struct V3 { float X, Y, Z; };
static V3 Sub(const V3& a, const V3& b) { V3 r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static V3 Add(const V3& a, const V3& b) { V3 r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
struct CPhysics { V3 Pos; int SetCount; bool Moving; };
struct CNymph { V3 Pos; CPhysics Physics; bool DummyFound; V3 Dummy; const char* AskedFor; };
static bool g_baseRan;
static void FrameUpdate(CNymph& n, const V3& init_dummy_pos)
{
    g_baseRan = true;
    V3 dummy;
    n.AskedFor = "DUMMY_CENTRE";
    if (n.DummyFound) {
        dummy = n.Dummy;
        n.Physics.Pos = Add(Sub(init_dummy_pos, dummy), n.Pos);
        ++n.Physics.SetCount;
        n.Physics.Moving = false;
    }
}
int main()
{
    CNymph n; std::memset(&n, 0, sizeof(n));
    n.Pos.X = 10; n.Pos.Y = 20; n.Pos.Z = 30; n.DummyFound = true; n.Dummy.X = 1; n.Dummy.Y = 2; n.Dummy.Z = 3;
    n.Physics.Moving = true;
    V3 init = { 4, 4, 4 };
    FrameUpdate(n, init);
    bool ok = g_baseRan && n.Physics.SetCount == 1 && n.Physics.Pos.X == 13 && n.Physics.Pos.Y == 22
        && n.Physics.Pos.Z == 31 && !n.Physics.Moving && std::strcmp(n.AskedFor, "DUMMY_CENTRE") == 0;
    CNymph m; std::memset(&m, 0, sizeof(m)); m.Physics.Moving = true;
    FrameUpdate(m, init);
    ok = ok && m.Physics.SetCount == 0 && m.Physics.Moving;
    if (!ok) { std::printf("008c9e60_TEST FAIL\n"); return 1; }
    std::printf("008c9e60_TEST PASS\n");
    return 0;
}
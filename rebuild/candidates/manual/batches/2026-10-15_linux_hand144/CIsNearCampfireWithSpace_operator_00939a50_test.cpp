// Behaviour test for 0x00939a50 (CIsNearCampfireWithSpace::operator()): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct C3DVector { float X, Y, Z; };
struct CTCUserActionPointBase { bool free_; bool IsActionPointFree() { return free_; } };
struct CThing { unsigned int flags; C3DVector pos; unsigned int mask[9]; CTCUserActionPointBase* point;
    unsigned int GetThingTypeFlags() const { return flags; }
    const C3DVector& GetPosition() const { return pos; }
    unsigned int HasTC(long k) const { return mask[k >> 5] & (1u << (k & 31)); } };
struct CIsNearCampfireWithSpace {
    float DistanceSq; C3DVector FromPos;
    bool operator()(CThing* thing) const
    {
        if ((unsigned char)(thing->GetThingTypeFlags() >> 14) & 1) {
            const C3DVector& pos = thing->GetPosition();
            float dx = pos.X - FromPos.X, dy = pos.Y - FromPos.Y, dz = pos.Z - FromPos.Z;
            if (dx * dx + dy * dy + dz * dz <= DistanceSq && thing->HasTC(0x2d))
                return thing->point->IsActionPointFree();
        }
        return false;
    }
};
static void Run()
{
    CTCUserActionPointBase ap; ap.free_ = true;
    CThing t; t.flags = 1u << 14; t.pos.X = 1; t.pos.Y = 2; t.pos.Z = 2; std::memset(t.mask, 0, sizeof(t.mask)); t.mask[1] = 1u << 13; t.point = &ap;
    CIsNearCampfireWithSpace f; f.DistanceSq = 9.0f; f.FromPos.X = 0; f.FromPos.Y = 0; f.FromPos.Z = 0;
    CHECK(f(&t));
    ap.free_ = false; CHECK(!f(&t)); ap.free_ = true;
    f.DistanceSq = 8.9f; CHECK(!f(&t)); f.DistanceSq = 9.0f;
    t.mask[1] = 0; CHECK(!f(&t)); t.mask[1] = 1u << 13;
    t.flags = 1u << 13; CHECK(!f(&t));
}

int main()
{
    Run();
    if (g_fail) { std::printf("00939a50_TEST FAIL\n"); return 1; }
    std::printf("00939a50_TEST PASS\n");
    return 0;
}

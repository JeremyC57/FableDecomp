// Behaviour test for 0x00C53E40: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of the 3D sound source update: compare, store, mirror into the pending block, raise the dirty bit.
struct V3 { float X, Y, Z; bool operator!=(const V3& o) const { return X != o.X || Y != o.Y || Z != o.Z; } };
struct Src {
    V3 Velocity; V3 PendingVelocity; unsigned int DirtyFlags;
    V3 Get() const { return Velocity; }
    void SetVelocity(const V3& v) { if (v != Get()) { Velocity = v; PendingVelocity = v; DirtyFlags |= 0x4000; } }
};
static void Run()
{
    Src s; V3 z = {0, 0, 0}; s.Velocity = z; s.PendingVelocity = z; s.DirtyFlags = 1;
    s.SetVelocity(z); CHECK(s.DirtyFlags == 1);
    V3 v = {1, 2, 3}; s.SetVelocity(v);
    CHECK(s.DirtyFlags == (1 | 0x4000) && s.Velocity.Z == 3 && s.PendingVelocity.Y == 2);
    s.DirtyFlags = 0; V3 w = {1, 2, 4}; s.SetVelocity(w); CHECK(s.DirtyFlags == 0x4000 && s.PendingVelocity.Z == 4);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00c53e40_TEST FAIL\n"); return 1; }
    std::printf("00c53e40_TEST PASS\n");
    return 0;
}
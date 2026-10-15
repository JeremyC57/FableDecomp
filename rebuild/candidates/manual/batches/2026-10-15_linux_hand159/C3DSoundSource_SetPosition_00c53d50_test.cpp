// Behaviour test for 0x00C53D50: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of the 3D sound source update: compare, store, mirror into the pending block, raise the dirty bit.
struct V3 { float X, Y, Z; bool operator!=(const V3& o) const { return X != o.X || Y != o.Y || Z != o.Z; } };
struct Src {
    V3 Position; V3 PendingPosition; unsigned int DirtyFlags;
    V3 Get() const { return Position; }
    void SetPosition(const V3& v) { if (v != Get()) { Position = v; PendingPosition = v; DirtyFlags |= 0x800; } }
};
static void Run()
{
    Src s; V3 z = {0, 0, 0}; s.Position = z; s.PendingPosition = z; s.DirtyFlags = 1;
    s.SetPosition(z); CHECK(s.DirtyFlags == 1);
    V3 v = {1, 2, 3}; s.SetPosition(v);
    CHECK(s.DirtyFlags == (1 | 0x800) && s.Position.Z == 3 && s.PendingPosition.Y == 2);
    s.DirtyFlags = 0; V3 w = {1, 2, 4}; s.SetPosition(w); CHECK(s.DirtyFlags == 0x800 && s.PendingPosition.Z == 4);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00c53d50_TEST FAIL\n"); return 1; }
    std::printf("00c53d50_TEST PASS\n");
    return 0;
}

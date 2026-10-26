// Behaviour test for 0x00963810 (CIsImposterHit::operator()): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CModes { long active; bool IsModeActive(long m) const { return active == m; } };
class CThing { public: unsigned int mask[9]; bool Dead; CModes* modes;
    template <class T> bool GetInterface(T** out, long k) { if (mask[k >> 5] & (1u << (k & 31))) { *out = (T*)modes; return true; } return false; } };
struct CPtr { CThing* item; int gets; CThing* GetPItem() { ++gets; return item; } };
struct CIsImposterHit {
    CPtr PImposter;
    bool operator()()
    {
        if (PImposter.GetPItem() && !PImposter.GetPItem()->Dead) {
            CModes* modes;
            if (PImposter.GetPItem()->GetInterface(&modes, 0x42) && modes->IsModeActive(0x25)) return true;
        }
        return false;
    }
};
static void Run()
{
    CModes m; m.active = 0x25; CThing t; std::memset(t.mask, 0, sizeof(t.mask)); t.Dead = false; t.modes = &m; t.mask[2] = 4;
    CIsImposterHit f; f.PImposter.item = &t; f.PImposter.gets = 0;
    CHECK(f() && f.PImposter.gets == 3);
    m.active = 0x24; CHECK(!f());
    m.active = 0x25; t.mask[2] = 0; CHECK(!f());
    t.mask[2] = 4; t.Dead = true; CHECK(!f());
    f.PImposter.item = 0; CHECK(!f());
}

int main()
{
    Run();
    if (g_fail) { std::printf("00963810_TEST FAIL\n"); return 1; }
    std::printf("00963810_TEST PASS\n");
    return 0;
}
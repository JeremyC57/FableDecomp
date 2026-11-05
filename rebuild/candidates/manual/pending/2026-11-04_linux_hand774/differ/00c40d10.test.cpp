// Behaviour test for 0x00c40d10: CEffectOwner_00c40750::CommitProperties -- clamp two pan vectors to unit length and commit when dirty
#include <cstdio>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct V { float x, y, z; };
struct Owner;
struct Effect { int Calls; Owner* Last; long SetProperties(Owner* o) { ++Calls; Last = o; return 0; } };
struct Owner { V panA, panB; unsigned int dirty; Effect* effect;
    void Clamp(V& p) { float x = p.x, y = p.y, z = p.z; float len = (float)std::sqrt(z * z + y * y + x * x);
        if (len > 1.0f) { float s = 1.0f / len; p.x = x * s; p.y = y * s; p.z = z * s; } }
    void CommitProperties() { if (dirty) { Clamp(panA); Clamp(panB); effect->SetProperties(this); dirty = 0; } } };
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    Effect e; e.Calls = 0; e.Last = 0; Owner o; o.effect = &e;
    o.panA.x = 3; o.panA.y = 0; o.panA.z = 4; o.panB.x = 0.2f; o.panB.y = 0.3f; o.panB.z = 0.1f; o.dirty = 0;
    o.CommitProperties(); CHECK(e.Calls == 0 && o.panA.x == 3);
    o.dirty = 0x20; o.CommitProperties();
    CHECK(e.Calls == 1 && e.Last == &o && o.dirty == 0);
    CHECK(Near(o.panA.x, 0.6f) && Near(o.panA.y, 0.0f) && Near(o.panA.z, 0.8f));
    CHECK(Near(o.panB.x, 0.2f) && Near(o.panB.y, 0.3f) && Near(o.panB.z, 0.1f));
    if (g_fail) { std::printf("00c40d10_TEST FAIL\n"); return 1; }
    std::printf("00c40d10_TEST PASS\n");
    return 0;
}

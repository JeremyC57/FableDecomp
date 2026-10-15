// Behaviour test for 0x008E2FC0 (CIcicleData copy constructor): both pointers re-registered on the same things, timers and position copied.
#include <cstdio>
static int g_registrations = 0;
struct Thing { int id; };
struct IntelligentPointer {
    Thing* item;
    IntelligentPointer() : item(0) {}
    IntelligentPointer(const IntelligentPointer& o) : item(0) { SetPItem(o.GetPItem()); }
    Thing* GetPItem() const { return item; }
    void SetPItem(Thing* t) { item = t; if (t) ++g_registrations; }
};
struct Vec3 { float x, y, z; Vec3() {} Vec3(const Vec3& o) : x(o.x), y(o.y), z(o.z) {} };
struct IcicleData {
    IntelligentPointer PEffect, PTrap; int EffectTimer, TrapTimer; Vec3 Position;
    IcicleData() {}
    IcicleData(const IcicleData& o) : PEffect(o.PEffect), PTrap(o.PTrap), EffectTimer(o.EffectTimer), TrapTimer(o.TrapTimer), Position(o.Position) {}
};
int main()
{
    Thing effect, trap; IcicleData a; a.PEffect.SetPItem(&effect); a.PTrap.SetPItem(&trap); a.EffectTimer = 5; a.TrapTimer = 9;
    a.Position.x = 1.5f; a.Position.y = -2.0f; a.Position.z = 3.25f; g_registrations = 0;
    IcicleData b(a);
    int ok = b.PEffect.item == &effect && b.PTrap.item == &trap && g_registrations == 2 && b.EffectTimer == 5 && b.TrapTimer == 9;
    ok = ok && b.Position.x == 1.5f && b.Position.y == -2.0f && b.Position.z == 3.25f;
    if (!ok) { std::printf("008e2fc0_TEST FAIL\n"); return 1; }
    std::printf("008e2fc0_TEST PASS\n");
    return 0;
}

// behaviour test for 0x00AEB8C0 CParticleEmitterManager::CreateInterpolatedEmitter: threshold shortcut, template lookup, hand-over
#include <cstdio>
struct CEmitter { int id; };
static CEmitter g_templates[4] = { { 0 }, { 1 }, { 2 }, { 3 } };
static int g_last = 0;
struct CDb { CEmitter* GetEmitterTemplate(int handle, bool) { return (handle > 0 && handle < 4) ? &g_templates[handle] : 0; } };
static CDb g_db;
struct CMgr {
    CEmitter created;
    CEmitter* CreateEmitter(int handle) { g_last = 100 + handle; created.id = 100 + handle; return &created; }
    CEmitter* Interp(CEmitter* from, CEmitter* to, float) { g_last = from->id * 10 + to->id; created.id = g_last; return &created; }
    CEmitter* FromTemplate(CEmitter* from, int toHandle, float t, bool flag)
    {
        if (t >= 1.0f) return CreateEmitter(toHandle);
        CEmitter* to = g_db.GetEmitterTemplate(toHandle, flag);
        if (to) return Interp(from, to, t);
        return 0;
    }
    CEmitter* FromHandles(int fromHandle, int toHandle, float t, bool flag)
    {
        if (t <= 0.0f) return CreateEmitter(fromHandle);
        CEmitter* from = g_db.GetEmitterTemplate(fromHandle, flag);
        if (from) return FromTemplate(from, toHandle, t, flag);
        return 0;
    }
};
int main()
{
    CMgr m;
    bool ok = m.FromHandles(2, 3, 0.0f, false)->id == 102 && m.FromHandles(2, 3, 1.0f, false)->id == 103 &&
              m.FromHandles(2, 3, 0.5f, false)->id == 23 && m.FromHandles(7, 3, 0.5f, false) == 0 &&
              m.FromTemplate(&g_templates[1], 9, 0.5f, false) == 0;
    if (!ok) { std::printf("00aeb8c0_TEST FAIL\n"); return 1; }
    std::printf("00aeb8c0_TEST PASS\n");
    return 0;
}
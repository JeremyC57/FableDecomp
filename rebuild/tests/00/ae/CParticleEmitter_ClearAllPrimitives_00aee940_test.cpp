// behaviour test for 0x00AEE940 CParticleEmitter::ClearAllPrimitives: only with a group: systems cleared, engine removes it, handle cleared
#include <cstdio>
static int g_cleared = 0, g_removed = 0, g_notified = 0, g_destroyed = 0;
struct CPrim { int refs; };
struct CHandle {
    CPrim* p;
    void Clear() { if ((p ? p->refs : 0) == 1) ++g_notified; if (p) { if (--p->refs == 0) ++g_destroyed; p = 0; } p = 0; }
};
struct CSystem { void ClearAllPrimitives(CHandle*) { ++g_cleared; } };
class CEngine { public: void RemovePrimitive(CHandle*) { ++g_removed; } };
struct CEmitter {
    CSystem* first; CSystem* last; CHandle PrimitiveGroupHandle;
    void ClearAllPrimitives(CEngine* engine)
    {
        if (PrimitiveGroupHandle.p) {
            for (CSystem* it = first; it != last; ++it) it->ClearAllPrimitives(&PrimitiveGroupHandle);
            if (engine) engine->RemovePrimitive(&PrimitiveGroupHandle);
            PrimitiveGroupHandle.Clear();
        }
    }
};
int main()
{
    CSystem s[3]; CPrim prim = { 1 }; CEngine eng;
    CEmitter e = { s, s + 3, { &prim } };
    e.ClearAllPrimitives(&eng);
    e.ClearAllPrimitives(&eng);
    if (g_cleared != 3 || g_removed != 1 || g_notified != 1 || g_destroyed != 1 || e.PrimitiveGroupHandle.p != 0) { std::printf("00aee940_TEST FAIL\n"); return 1; }
    std::printf("00aee940_TEST PASS\n");
    return 0;
}
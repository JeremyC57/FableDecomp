// Behaviour model of CCreatureAction_NymphTeleportLimboBase::DoEffect (0x008C9FE0): look up TELEPORT_POS in the
// creature's special-effects sub-def, spawn the effect at the creature, attach its particle emitter (if any) to
// DUMMY_CENTRE, and release the sub-def reference.
#include <cstdio>
#include <cstring>
static int g_attach = 0, g_destroy = 0, g_create = 0; static long g_lastEffect = -1;
struct Def { int refs; bool hasSub; bool hasName; long value; };
struct Emitter { int attached; };
struct Fx { bool dead; bool hasEmitter; Emitter em; };
static Fx g_fx;
static bool GetSubDef(Def* d, Def** out) { if (!d->hasSub) return false; ++d->refs; *out = d; return true; }
static bool CalcAndLookup(Def* d, const char* name, long* v) { if (!d->hasName || std::strcmp(name, "TELEPORT_POS")) return false; *v = d->value; return true; }
static Fx* CreateFx(long effect) { ++g_create; g_lastEffect = effect; return effect ? &g_fx : 0; }
static Emitter* FindInterface(Fx* fx) { Emitter* r = 0; if (fx->hasEmitter) r = &fx->em; return r; }
static void Release(Def* d) { if (d && --d->refs == 0) ++g_destroy; }
static void DoEffect(Def* thingDef)
{
    Def* effects = 0; long effect = 0;
    bool found = GetSubDef(thingDef, &effects) && CalcAndLookup(effects, "TELEPORT_POS", &effect);
    if (found) {
        Fx* fx = CreateFx(effect);
        if (fx && !fx->dead) { Emitter* e = FindInterface(fx); if (e) { ++e->attached; ++g_attach; } }
    }
    Release(effects);
}
int main()
{
    int ok = 1;
    Def d = { 1, true, true, 7 }; g_fx.dead = false; g_fx.hasEmitter = true; g_fx.em.attached = 0;
    DoEffect(&d); ok &= g_attach == 1 && g_create == 1 && g_lastEffect == 7 && d.refs == 1;
    Def n = { 1, false, true, 7 }; DoEffect(&n); ok &= g_create == 1 && n.refs == 1;
    Def m = { 1, true, false, 7 }; DoEffect(&m); ok &= g_create == 1 && m.refs == 1;
    g_fx.dead = true; DoEffect(&d); ok &= g_create == 2 && g_attach == 1;
    std::printf(ok ? "008c9fe0_TEST PASS\n" : "008c9fe0_TEST FAIL\n");
    return ok ? 0 : 1;
}
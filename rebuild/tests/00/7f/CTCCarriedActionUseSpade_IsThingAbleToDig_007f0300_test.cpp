#include <cstdio>
// Behaviour test: CTCCarriedActionUseSpade::IsThingAbleToDig -- false when standing on a thing; else the strongest engine theme under the creature must resolve to a def whose material def is diggable; every def reference taken is released.
#define CHECK(c) if (!(c)) { std::printf("007f0300_TEST FAIL\n"); return 1; }
struct C3DVector { float X, Y, Z; };
static int g_live = 0;
struct CDef { long RefCount; long MaterialDef; bool IsDiggable; void Release() { if (--RefCount == 0) --g_live; } };
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
static CDef g_defs[4];
static bool GetDef(long index, CDefPointer<CDef>* out)
{
    if (index < 0 || index >= 4) return false;
    if (g_defs[index].RefCount++ == 0) ++g_live;
    out->Object = &g_defs[index];
    return true;
}
struct CThing { CThing* StandingOn; C3DVector Position; long Theme; };
static long GetStrongestEngineThemeAt(const CThing& c, long* weight) { *weight = 1; return c.Theme; }
static bool IsThingAbleToDig(const CThing& creature)
{
    if (!creature.StandingOn) {
        long weight;
        CDefPointer<CDef> themeDef;
        if (GetDef(GetStrongestEngineThemeAt(creature, &weight), &themeDef)) {
            CDefPointer<CDef> materialDef;
            GetDef(themeDef->MaterialDef, &materialDef);
            if (materialDef->IsDiggable) return true;
        }
    }
    return false;
}
int main()
{
    for (int i = 0; i < 4; ++i) { g_defs[i].RefCount = 0; g_defs[i].IsDiggable = false; g_defs[i].MaterialDef = 0; }
    g_defs[0].MaterialDef = 2; g_defs[1].MaterialDef = 3; g_defs[2].IsDiggable = true;
    CThing ground; ground.StandingOn = 0; ground.Theme = 0;
    CHECK(IsThingAbleToDig(ground) && g_live == 0)
    ground.Theme = 1; CHECK(!IsThingAbleToDig(ground) && g_live == 0)
    ground.Theme = 9; CHECK(!IsThingAbleToDig(ground) && g_live == 0)
    CThing box; CThing onBox; onBox.StandingOn = &box; onBox.Theme = 0; CHECK(!IsThingAbleToDig(onBox))
    std::printf("007f0300_TEST PASS\n");
    return 0;
}
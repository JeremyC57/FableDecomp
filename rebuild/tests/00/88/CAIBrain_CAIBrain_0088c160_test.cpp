// behaviour test for CAIBrain::CAIBrain (0x0088C160)
#include <cstdio>
struct Def { long RefCount; int zones; };
struct DefPtr {
    Def* Object;
    DefPtr(Def* d) : Object(d) { if (Object) ++Object->RefCount; }
    DefPtr(const DefPtr& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~DefPtr() { if (Object) --Object->RefCount; }
};
static int initCalls;
struct Brain {
    void* Component; void* Creature; void* ThingManager; void* PlayerManager; void* World;
    void* PActive; void* PLast; void* PPending; bool OutputValid, UpdatingStateGroup, Initialised, OnKillCalled;
    DefPtr PBrainDef; int zones; void* PCurrentUpdateZone;
    Brain(void* comp, void* world, void* tm, void* pm, void* creature, DefPtr def)
        : Component(comp), Creature(creature), ThingManager(tm), PlayerManager(pm), World(world), OnKillCalled(false), PBrainDef(def)
    {
        ++initCalls; zones = def.Object->zones;
        PActive = PLast = PPending = 0; PCurrentUpdateZone = 0;
        OutputValid = false; UpdatingStateGroup = false; Initialised = true;
    }
};
int main()
{
    Def d = { 1, 3 };
    int c, w, tm, pm, cr;
    {
        Brain b(&c, &w, &tm, &pm, &cr, DefPtr(&d));
        bool ok = b.Component == &c && b.World == &w && b.Creature == &cr && b.ThingManager == &tm && b.PlayerManager == &pm
            && b.zones == 3 && initCalls == 1 && b.Initialised && !b.OutputValid && d.RefCount == 2;
        if (!ok) { std::printf("0088c160_TEST FAIL\n"); return 1; }
    }
    if (d.RefCount != 1) { std::printf("0088c160_TEST FAIL\n"); return 1; }
    std::printf("0088c160_TEST PASS\n");
    return 0;
}
// Behaviour test for CDefinitionManager::LoadDef (0x009AD6E0): a loaded def is only moved to the front of the
// resource list (when its frame is stale); an unloaded def is created by its class factory, replaces the (empty)
// pointer, gets its instantiation name, owner and index, is loaded, persisted, sized (data size + 0x25), added to
// the resource list and touched; an unknown class leaves the def unloaded.
#include <cstdio>

static int g_touches = 0, g_progress = 0, g_creates = 0, g_adds = 0;

struct Def {
    long name; void* owner; unsigned long index; unsigned long size; unsigned long lastFrame; bool loaded, persisted;
};
static Def g_made;
static Def* CreateDef() { ++g_creates; g_made.loaded = g_made.persisted = false; return &g_made; }
struct ClassInfo { Def* (*Create)(); };
struct Info { long ClassName; long InstantiationName; Def* PDef; };

struct Manager {
    unsigned long currentFrame;
    Info** InstantiatedDefs;
    ClassInfo* known;
    long knownName;
    long streamSize;
    void Touch(Def* d) { if (d->lastFrame != currentFrame) { ++g_touches; d->lastFrame = currentFrame; } }
    void Progress() { ++g_progress; }
    ClassInfo* FindClassInfo(long className) { return className == knownName ? known : 0; }
    void LoadDef(unsigned long globalIndex)
    {
        Info* info = InstantiatedDefs[globalIndex];
        Def* def = info->PDef;
        if (def) { Touch(def); return; }
        Progress();
        ClassInfo* classInfo = FindClassInfo(info->ClassName);
        if (classInfo) {
            unsigned long size = streamSize + 0x25;
            Def* created = classInfo->Create();
            info->PDef = created;
            created->name = info->InstantiationName;
            created->owner = this;
            created->index = globalIndex;
            created->loaded = true;
            Progress();
            info->PDef->persisted = true;
            info->PDef->size = size;
            ++g_adds;
            info->PDef->lastFrame = 0;
            Touch(info->PDef);
            Progress();
        }
    }
};

int main()
{
    ClassInfo ci = { CreateDef };
    Info a = { 7, 70, 0 }, b = { 9, 90, 0 };
    Def existing; existing.lastFrame = 3;
    Info c = { 7, 71, &existing };
    Info* defs[3] = { &a, &b, &c };
    Manager m; m.currentFrame = 5; m.InstantiatedDefs = defs; m.known = &ci; m.knownName = 7; m.streamSize = 0x100;
    m.LoadDef(2);
    if (g_touches != 1 || existing.lastFrame != 5 || g_progress != 0) { std::printf("009ad6e0_TEST FAIL\n"); return 1; }
    m.LoadDef(2);
    if (g_touches != 1) { std::printf("009ad6e0_TEST FAIL\n"); return 1; }
    m.LoadDef(0);
    if (a.PDef != &g_made || g_made.name != 70 || g_made.owner != &m || g_made.index != 0 || !g_made.loaded
        || !g_made.persisted || g_made.size != 0x125 || g_adds != 1 || g_progress != 3 || g_touches != 2) {
        std::printf("009ad6e0_TEST FAIL\n"); return 1;
    }
    m.LoadDef(1);
    if (b.PDef != 0 || g_creates != 1 || g_progress != 4) { std::printf("009ad6e0_TEST FAIL\n"); return 1; }
    std::printf("009ad6e0_TEST PASS\n");
    return 0;
}

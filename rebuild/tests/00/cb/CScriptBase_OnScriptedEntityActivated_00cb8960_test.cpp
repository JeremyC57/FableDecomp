#include <cstdio>
#include <cstring>
// Behaviour model of CScriptBase::OnScriptedEntityActivated (0x00CB8960): an entity id already in the active-script
// map is revived (a kill-flagged script only has its kill request cleared; otherwise it restarts and is loaded);
// a new id gets a script from the binding named after the entity, is filed in the id-sorted map and loaded (and the
// entity is told not to die on level unload when the script carries flag 1). Every call bumps the script counter.
struct Script { unsigned long id; unsigned long flags; int restarts; int loads; bool killRequested; };
struct Entity { const char* name; int counter; bool killOnUnload; };
struct Base {
    Script scripts[8]; int count;
    Script* find(unsigned long id) { for (int i = 0; i < count; ++i) if (scripts[i].id == id) return &scripts[i]; return 0; }
    void sortScripts()
    {
        for (int i = 1; i < count; ++i)
            for (int j = i; j > 0 && scripts[j].id < scripts[j - 1].id; --j) { Script t = scripts[j]; scripts[j] = scripts[j - 1]; scripts[j - 1] = t; }
    }
    void Activate(unsigned long id, Entity& entity)
    {
        Script* s = find(id);
        if (s) {
            if (s->flags & 1) s->killRequested = false;
            else { ++s->restarts; ++s->loads; }
        } else {
            Script n; n.id = id; n.flags = std::strcmp(entity.name, "guard") == 0 ? 1 : 0; n.restarts = 0; n.loads = 0; n.killRequested = false;
            scripts[count++] = n;
            sortScripts();
            if (n.flags & 1) entity.killOnUnload = false;
            ++find(id)->loads;
        }
        ++entity.counter;
    }
};
int main()
{
    Base b; b.count = 0;
    Entity villager; villager.name = "villager"; villager.counter = 0; villager.killOnUnload = true;
    Entity guard; guard.name = "guard"; guard.counter = 0; guard.killOnUnload = true;
    b.Activate(50, villager);
    b.Activate(20, guard);
    bool ok = b.count == 2 && b.scripts[0].id == 20 && b.scripts[1].id == 50 && b.scripts[0].loads == 1
        && !guard.killOnUnload && villager.killOnUnload && guard.counter == 1 && villager.counter == 1;
    b.Activate(50, villager);
    ok = ok && b.count == 2 && b.scripts[1].restarts == 1 && b.scripts[1].loads == 2 && villager.counter == 2;
    b.scripts[0].killRequested = true;
    b.Activate(20, guard);
    ok = ok && !b.scripts[0].killRequested && b.scripts[0].loads == 1 && b.scripts[0].restarts == 0 && guard.counter == 2;
    if (!ok) { std::printf("00cb8960_TEST FAIL\n"); return 1; }
    std::printf("00cb8960_TEST PASS\n");
    return 0;
}
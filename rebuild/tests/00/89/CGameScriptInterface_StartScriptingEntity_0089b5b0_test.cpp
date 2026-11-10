// Behaviour model of CGameScriptInterface::StartScriptingEntity (0x0089B5B0).
#include <cstdio>
struct Thing { bool alive; bool dead; int classType; int stateGroupPriority; bool locked; int unlockAfterFrames; };
struct Resource { bool isNull; Thing* thing; long scriptID; int priority; Resource() : isNull(false), thing(0), scriptID(-1), priority(-1) {} };
struct Interface { long currentScriptID; int frames; bool stopRunning; Interface() : currentScriptID(0), frames(0), stopRunning(false) {} };
static void NewScriptFrame(Interface& gsi, Thing* thing)
{
    ++gsi.frames;
    if (thing->unlockAfterFrames > 0 && --thing->unlockAfterFrames == 0) thing->locked = false;
}
static bool StartScriptingEntity(Interface& gsi, Thing* thing, Resource& resource, int priority)
{
    if (thing->alive) {
        if (thing->classType == 1 && thing->stateGroupPriority > priority)
            return false;                               // a higher-priority AI state group keeps the creature
        while (thing->locked) {
            if (!thing->alive || gsi.stopRunning)
                return false;
            NewScriptFrame(gsi, thing);
        }
        if (!thing->dead) {
            resource.isNull = false; resource.thing = thing; resource.scriptID = gsi.currentScriptID; resource.priority = priority;
            return true;
        }
    }
    resource.isNull = true; resource.thing = 0;           // the null scripted-thing handle
    return false;
}
static int fail(int n) { std::printf("0089b5b0_TEST FAIL %d\n", n); return 1; }
int main()
{
    Interface gsi; gsi.currentScriptID = 42;
    Thing t = { true, false, 1, 3, false, 0 };
    Resource r;
    if (StartScriptingEntity(gsi, &t, r, 5) != true || r.thing != &t || r.scriptID != 42 || r.priority != 5 || r.isNull) return fail(1);
    Resource r2;
    if (StartScriptingEntity(gsi, &t, r2, 2) != false || r2.thing != 0 || r2.isNull) return fail(2);   // refused, resource untouched
    Thing v = { true, false, 2, 9, false, 0 };                                                       // not an AI creature: no priority test
    Resource r3;
    if (!StartScriptingEntity(gsi, &v, r3, 0) || r3.thing != &v) return fail(3);
    Thing w = { true, false, 1, 0, true, 3 };
    Resource r4;
    if (!StartScriptingEntity(gsi, &w, r4, 1) || gsi.frames != 3 || r4.thing != &w) return fail(4);
    Thing x = { true, false, 1, 0, true, 0 }; gsi.stopRunning = true;
    Resource r5;
    if (StartScriptingEntity(gsi, &x, r5, 1) || r5.isNull) return fail(5);
    gsi.stopRunning = false;
    Thing d = { false, true, 1, 0, false, 0 };
    Resource r6;
    if (StartScriptingEntity(gsi, &d, r6, 1) || !r6.isNull) return fail(6);
    Thing z = { true, true, 2, 0, false, 0 };
    Resource r7;
    if (StartScriptingEntity(gsi, &z, r7, 1) || !r7.isNull) return fail(7);
    std::printf("0089b5b0_TEST PASS\n");
    return 0;
}
// Behaviour test for CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage (0x009810B0): a copy of the model's logic with
// stub collaborators -- search result, flee result, hand-over result, and the by-value message handle's release on every exit.
#include <cstdio>
enum EAIActionReturn { AI_ACTION_NULL=0, AI_ACTION_TERMINATE_PROCESS=1, AI_ACTION_FINISHED=2, AI_ACTION_FAILED=3, AI_ACTION_IN_PROGRESS=4 };
struct Def { long IVRefCount; int deleted; void Delete() { deleted = 1; } };
struct DefPointer {
    Def* Object;
    DefPointer(Def* d) : Object(d) { if (Object) ++Object->IVRefCount; }
    DefPointer(const DefPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~DefPointer() { if (Object) { Def* o = Object; if (--o->IVRefCount == 0) o->Delete(); Object = 0; } }
};
struct Creature { int property; float x; };
struct World {
    Creature* creatures[4]; int count; float myX;
    EAIActionReturn fleeResult, performResult;
    int fleeCalls, sendCalls; Creature* sentTo; float fleeProximity; long sentParameter; int defRefsDuringSend;
};
static bool GetPBestLeafThing(World& w, unsigned long property, float pos, Creature** out)
{
    Creature* best = 0;
    for (int i = 0; i < w.count; ++i) {
        Creature* c = w.creatures[i];
        if ((unsigned long)c->property != property) continue;
        float d = c->x - pos, bd = best ? best->x - pos : 0;
        if (!best || d * d < bd * bd) best = c;
    }
    *out = best;
    return best != 0;
}
static EAIActionReturn Flee(World& w, Creature* target, float proximity) { ++w.fleeCalls; w.fleeProximity = proximity; (void)target; return w.fleeResult; }
static EAIActionReturn SendAndWait(World& w, const DefPointer& def, Creature* target, long parameter)
{
    DefPointer copy(def);                       // the action's PDef member
    ++w.sendCalls; w.sentTo = target; w.sentParameter = parameter; w.defRefsDuringSend = def.Object->IVRefCount;
    return w.performResult;
}
static EAIActionReturn FindAGuardAndGiveMessage(World& w, DefPointer message, float position, long parameter)
{
    (void)position;
    Creature* guard = 0;
    if (!GetPBestLeafThing(w, 3, w.myX, &guard))
        return AI_ACTION_FAILED;
    EAIActionReturn result = Flee(w, guard, 2.0f);
    if (result != AI_ACTION_FINISHED)
        return result;
    bool failed = SendAndWait(w, message, guard, parameter) != AI_ACTION_FINISHED;
    if (failed)
        return AI_ACTION_FAILED;
    return AI_ACTION_FINISHED;
}
static int Run(World& w, Def& def, EAIActionReturn expect)
{
    long before = def.IVRefCount;
    EAIActionReturn r = FindAGuardAndGiveMessage(w, DefPointer(&def), 0.0f, 77);
    return r == expect && def.IVRefCount == before;
}
int main()
{
    Creature a = { 3, 10.0f }, b = { 1, 1.0f }, c = { 3, 2.0f };
    Def def = { 1, 0 };
    World w = { { &a, &b, &c }, 3, 0.0f, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 0, 0, 0, 0, 0, 0 };
    bool ok = true;
    ok = ok && Run(w, def, AI_ACTION_FINISHED) && w.sentTo == &c && w.sentParameter == 77 && w.fleeProximity == 2.0f
            && w.defRefsDuringSend == 3;
    w.performResult = AI_ACTION_IN_PROGRESS;
    ok = ok && Run(w, def, AI_ACTION_FAILED);
    w.fleeResult = AI_ACTION_TERMINATE_PROCESS; int sends = w.sendCalls;
    ok = ok && Run(w, def, AI_ACTION_TERMINATE_PROCESS) && w.sendCalls == sends;
    World none = { { &b }, 1, 0.0f, AI_ACTION_FINISHED, AI_ACTION_FINISHED, 0, 0, 0, 0, 0, 0 };
    ok = ok && Run(none, def, AI_ACTION_FAILED) && none.fleeCalls == 0;
    ok = ok && def.deleted == 0 && def.IVRefCount == 1;
    if (!ok) { std::printf("009810b0_TEST FAIL\n"); return 1; }
    std::printf("009810b0_TEST PASS\n");
    return 0;
}
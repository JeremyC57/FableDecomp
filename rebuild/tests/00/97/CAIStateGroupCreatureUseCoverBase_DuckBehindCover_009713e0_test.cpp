// Behaviour test for CAIStateGroup_CreatureUseCoverBase::DuckBehindCover (0x009713E0): dead/absent target -> false with no
// speech; live target -> the def's Hiding speech is said by the brain's creature (def reference count restored), the
// projectile-crouch action is built (creature, target, param), performed with wait=false, destroyed, and the result is
// true only for EAIActionReturn 2.
#include <cstdio>

struct Thing { bool dead; };
struct Def { long refCount; long deletes; long hiding; };
struct Log { int speeches; Thing* speaker; unsigned long speech; int ctors; int dtors; Thing* actor; Thing* target;
             bool param; bool wait; int performResult; long defRefDuringCall; };
static Log g;
static Def* g_def;

struct DefPointer {
    Def* object;
    DefPointer(const DefPointer& o) : object(o.object) { if (object) ++object->refCount; }
    explicit DefPointer(Def* d) : object(d) { if (object) ++object->refCount; }
    ~DefPointer() { if (object) { if (--object->refCount == 0) ++object->deletes; object = 0; } }
    Def* operator->() const { return object; }
};
static DefPointer GetPDialogueDef() { return DefPointer(*&g_def); }

struct Action {
    Action(Thing& creature, Thing& target, bool param) { ++g.ctors; g.actor = &creature; g.target = &target; g.param = param; }
    ~Action() { ++g.dtors; }
};
static bool SaySpeech(Thing& creature, unsigned long speech)
{
    ++g.speeches; g.speaker = &creature; g.speech = speech; g.defRefDuringCall = g_def->refCount; return true;
}
static int PerformActionAndWait(const Action&, bool wait) { g.wait = wait; return g.performResult; }

struct Group { Thing* target; Thing* creature; };

static bool IsThingAlive(Thing* t) { if (t != 0) { if (!t->dead) return true; } return false; }

static bool DuckBehindCover(Group& self, bool param)
{
    if (!IsThingAlive(self.target))
        return false;
    unsigned long speech = GetPDialogueDef()->hiding;
    SaySpeech(*self.creature, speech);
    int result = PerformActionAndWait(Action(*self.creature, *self.target, param), false);
    return result == 2 ? true : false;
}

static int fail() { std::printf("009713e0_TEST FAIL\n"); return 1; }

int main()
{
    Def def = { 1, 0, 0x1234 };
    g_def = &def;
    Thing creature = { false }, target = { false }, deadTarget = { true };
    Group self = { 0, &creature };
    Log zero = { 0 };
    g = zero;
    if (DuckBehindCover(self, true) || g.speeches || g.ctors) return fail();
    self.target = &deadTarget;
    if (DuckBehindCover(self, true) || g.speeches || g.ctors) return fail();
    self.target = &target;
    g = zero; g.performResult = 2;
    if (!DuckBehindCover(self, true)) return fail();
    if (g.speeches != 1 || g.speaker != &creature || g.speech != 0x1234) return fail();
    if (g.defRefDuringCall != 1 || def.refCount != 1 || def.deletes != 0) return fail();
    if (g.ctors != 1 || g.dtors != 1 || g.actor != &creature || g.target != &target || !g.param || g.wait) return fail();
    g = zero; g.performResult = 4;
    if (DuckBehindCover(self, false) || g.param || g.dtors != 1) return fail();
    g = zero; g.performResult = 1;
    if (DuckBehindCover(self, false)) return fail();
    std::printf("009713e0_TEST PASS\n");
    return 0;
}
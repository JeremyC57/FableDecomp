// Behaviour test for CCreatureAction_PerformExpressionExtended::CCreatureAction_PerformExpressionExtended (0x00843310): the base
// action starts on the def's Animation (speed -1, blend 0, zero sound pair, the interruption group, 0); the def pointer is copied with
// an add-ref; the members take the parameters (FrameCount = frames_running, SoundEffectID = sound_id, ReplacedAction false); then the
// animation is replaced by AnimationToOtherLoop when performing to a thing and that name is set, else by AnimationLoop when set.
#include <cstdio>
#include <cstring>

struct Def { long refs; const char* Animation; const char* AnimationLoop; const char* AnimationToOtherLoop; };
struct Creature { int id; };
struct Thing { int id; };

static const char* anim_of(const Creature&, const char* name) { return name; }   // GetRandomAnimFromName stand-in: the name itself

struct Action {
    Creature* PCreature; float speed, blend; const char* anim; long sound0, sound1; long group, zero;
    int set_calls; float set_speed, set_blend;
    Def* PExpressionDef; Thing* PPerformToThing; long ExpressionItemDefIndex; bool EmoteIconAdded; long FrameCount;
    bool ReplacedAction; unsigned long SoundEffectID;
    void SetAnimation(const char* a, float s, float b) { anim = a; set_speed = s; set_blend = b; ++set_calls; }
    Action(Creature& creature, Thing* pperform_to, Def* pdef, long index, bool emote, unsigned long sound_id, long frames_running,
           long interruption_group_id)
    {
        PCreature = &creature; speed = -1.0f; blend = 0.0f; anim = anim_of(creature, pdef->Animation); sound0 = sound1 = 0;
        group = interruption_group_id; zero = 0; set_calls = 0;
        PExpressionDef = pdef; if (pdef) ++pdef->refs;
        PPerformToThing = pperform_to; ExpressionItemDefIndex = index; EmoteIconAdded = emote; FrameCount = frames_running;
        ReplacedAction = false; SoundEffectID = sound_id;
        if (PPerformToThing && std::strcmp(PExpressionDef->AnimationToOtherLoop, "") != 0)
            SetAnimation(anim_of(*PCreature, PExpressionDef->AnimationToOtherLoop), -1.0f, 0.0f);
        else if (std::strcmp(PExpressionDef->AnimationLoop, "") != 0)
            SetAnimation(anim_of(*PCreature, PExpressionDef->AnimationLoop), -1.0f, 0.0f);
    }
};

static int fail(int n) { std::printf("00843310_TEST FAIL %d\n", n); return 1; }

int main()
{
    Creature c = { 1 }; Thing t = { 2 };
    Def d = { 1, "base", "loop", "toother" };
    Action a(c, &t, &d, 7, true, 99, 30, 5);
    if (std::strcmp(a.anim, "toother") != 0 || a.set_calls != 1 || a.set_speed != -1.0f || a.set_blend != 0.0f) return fail(1);
    if (d.refs != 2 || a.ExpressionItemDefIndex != 7 || !a.EmoteIconAdded || a.FrameCount != 30 || a.SoundEffectID != 99) return fail(2);
    if (a.ReplacedAction || a.group != 5 || a.zero != 0 || a.speed != -1.0f || a.blend != 0.0f || a.sound0 || a.sound1) return fail(3);
    Action b(c, 0, &d, 0, false, 0, 0, 0);                       // no target: the plain loop
    if (std::strcmp(b.anim, "loop") != 0 || b.set_calls != 1) return fail(4);
    Def e = { 1, "base", "loop", "" };
    Action f(c, &t, &e, 0, false, 0, 0, 0);                      // target but no to-other loop: the plain loop
    if (std::strcmp(f.anim, "loop") != 0 || f.set_calls != 1) return fail(5);
    Def g = { 1, "base", "", "" };
    Action h(c, &t, &g, 0, false, 0, 0, 0);                      // neither set: the base animation stays
    if (std::strcmp(h.anim, "base") != 0 || h.set_calls != 0) return fail(6);
    Def i = { 1, "base", "", "toother" };
    Action j(c, 0, &i, 0, false, 0, 0, 0);                       // to-other loop but no target, no loop: base stays
    if (std::strcmp(j.anim, "base") != 0 || j.set_calls != 0) return fail(7);
    std::printf("00843310_TEST PASS\n");
    return 0;
}

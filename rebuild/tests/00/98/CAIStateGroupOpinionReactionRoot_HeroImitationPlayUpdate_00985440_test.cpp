// Behaviour test for CAIStateGroup_OpinionReactionRoot::HeroImitationPlayUpdate (0x00985440) and the helpers it relies on (a standalone
// model): the first creature found by GetPFirstThing<Bind3<IsNot(self), IsCreatureType(HUMAN_CHILD), In2DRange(pos, sight)>> (type mask
// and scan-inclusion tests first), kids_nearby = found, AddMode(0x16), TurnToFace, the speech line chosen by kids_nearby and its FINISHED
// test, frame_to_stop_walking = (long)(frame + fps * (random(3) + 2)), the walk loop while IN_PROGRESS and frame < stop, RemoveMode only when
// the mode was pushed, and the FINISHED / IN_PROGRESS result. Audit e12: the walk's arguments (thing, flags, speed; case 19), a truncated
// fractional stop frame (case 20), TurnToFace tested directly (Ego_d's ret_bool is the walk's out flag).
#include <cstdio>
#include <cstring>
#include <cmath>

enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3, AI_ACTION_IN_PROGRESS = 4 };
enum ECreatureType { NOT_HUMAN = 0, HUMAN_CHILD = 1, HUMAN_ADULT = 2, HUMAN_ELDERLY = 3 };
struct Thing { unsigned long type_flags; bool scan; ECreatureType ctype; float x, y; };

// CThingFilter_IsCreatureType::operator() (header inline defined by this row's overlay)
struct IsCreatureType { ECreatureType Type; bool operator()(const Thing& t) const { return t.ctype == Type; } };
struct IsNot { const Thing* Self; bool operator()(const Thing& t) const { return &t != Self; } };
struct In2DRange { float X, Y, Range; bool operator()(const Thing& t) const { float dx = t.x - X, dy = t.y - Y; return dx * dx + dy * dy <= Range * Range; } };
// GetPFirstThing body: mask test (0x1f for CThingAICreature), scan inclusion, then the bound filters in order
static bool GetPFirstThing(Thing* things, int n, const IsNot& a, const IsCreatureType& b, const In2DRange& c, const Thing** out)
{
    for (int i = 0; i < n; ++i) {
        const unsigned long mask = 0x1f;
        if ((things[i].type_flags & mask) == mask && things[i].scan && a(things[i]) && b(things[i]) && c(things[i])) { *out = &things[i]; return true; }
    }
    return false;
}

struct Model {
    Thing things[4]; int nthings; float sight;   // things[0] is the creature itself
    bool add_mode_ok, turn_ok; EAIActionReturn speech_res; long fps; float random3; long frame; int frame_step;
    EAIActionReturn move_res[8]; int moves;
    int add_calls, remove_calls, turn_calls, speech_calls; long mode_arg, remove_arg; char line[64]; int layer;
    float prox; long move_type, move_repeat; const void* move_thing; const void* reacting_to; bool move_flags[4]; float move_speed;

    long GetFrame() { long f = frame; frame += frame_step; return f; }
    bool Run()
    {
        const Thing* self = &things[0];
        const Thing* ret_pcreature = 0;
        IsNot a = { self }; IsCreatureType b = { HUMAN_CHILD }; In2DRange c = { self->x, self->y, sight };
        GetPFirstThing(things, nthings, a, b, c, &ret_pcreature);
        bool kids_nearby = ret_pcreature != 0;
        ++add_calls; mode_arg = 0x16; bool pushed_mode = add_mode_ok;
        ++turn_calls;
        if (!turn_ok) return false;
        ++speech_calls; layer = 1;
        std::strcpy(line, kids_nearby ? "TEXT_AI_OPINION_ACT_HERO_IMITATION_PLAY_ABOUT_HERO" : "TEXT_AI_OPINION_ACT_HERO_IMITATION_PLAY_TO_SELF");
        if (speech_res != AI_ACTION_FINISHED) return false;
        long frame_to_stop_walking = (long)(GetFrame() + fps * (random3 + 2.0f));
        EAIActionReturn ret;
        do { move_repeat = 2; move_thing = reacting_to; prox = 1.0f; move_type = 3; move_flags[0] = true; move_flags[1] = false; move_flags[2] = true;
             move_flags[3] = true; move_speed = 0.0f; ret = move_res[moves++]; } while (ret == AI_ACTION_IN_PROGRESS && GetFrame() < frame_to_stop_walking);
        if (pushed_mode) { ++remove_calls; remove_arg = 0x16; }
        return ret == AI_ACTION_FINISHED || ret == AI_ACTION_IN_PROGRESS;
    }
};

static Model Make()
{
    Model m; std::memset(&m, 0, sizeof m);
    Thing self = { 0x1f, true, HUMAN_ADULT, 0.0f, 0.0f };
    m.things[0] = self;
    Thing kid = { 0x1f, true, HUMAN_CHILD, 3.0f, 4.0f };     // distance 5
    m.things[1] = kid; m.nthings = 2; m.sight = 6.0f;
    m.add_mode_ok = true; m.turn_ok = true; m.speech_res = AI_ACTION_FINISHED; m.fps = 10; m.random3 = 1.5f; m.frame = 100; m.frame_step = 1;
    m.move_res[0] = AI_ACTION_FINISHED; m.reacting_to = &m.things[1]; m.move_speed = -1.0f;
    return m;
}
static int fails = 0;
static void Check(bool ok, int id) { if (!ok) { std::printf("case %d failed\n", id); ++fails; } }

int main()
{
    // helpers directly
    { Thing t = { 0x1f, true, HUMAN_CHILD, 0, 0 }; IsCreatureType f1 = { HUMAN_CHILD }, f2 = { HUMAN_ADULT }; Check(f1(t) && !f2(t), 1); }
    { Model m = Make(); bool r = m.Run();
      Check(r && std::strcmp(m.line, "TEXT_AI_OPINION_ACT_HERO_IMITATION_PLAY_ABOUT_HERO") == 0 && m.layer == 1 && m.mode_arg == 0x16, 2);
      Check(m.remove_calls == 1 && m.remove_arg == 0x16 && m.moves == 1 && m.move_repeat == 2 && m.prox == 1.0f && m.move_type == 3, 3);
      Check(m.move_thing == m.reacting_to && m.move_flags[0] && !m.move_flags[1] && m.move_flags[2] && m.move_flags[3] && m.move_speed == 0.0f, 19); }
    { Model m = Make(); m.sight = 5.0f; m.Run(); Check(std::strstr(m.line, "ABOUT_HERO") != 0, 4); }             // exactly on the range
    { Model m = Make(); m.sight = 4.99f; m.Run(); Check(std::strstr(m.line, "TO_SELF") != 0, 5); }               // just outside
    { Model m = Make(); m.things[1].ctype = HUMAN_ADULT; m.Run(); Check(std::strstr(m.line, "TO_SELF") != 0, 6); } // not a child
    { Model m = Make(); m.things[1].type_flags = 0x0f; m.Run(); Check(std::strstr(m.line, "TO_SELF") != 0, 7); }  // not an AI creature
    { Model m = Make(); m.things[1].scan = false; m.Run(); Check(std::strstr(m.line, "TO_SELF") != 0, 8); }       // excluded from scans
    { Model m = Make(); m.things[0].ctype = HUMAN_CHILD; m.things[1].ctype = HUMAN_ADULT; m.Run(); Check(std::strstr(m.line, "TO_SELF") != 0, 9); } // self skipped
    { Model m = Make(); m.turn_ok = false; Check(!m.Run() && m.speech_calls == 0 && m.remove_calls == 0, 10); }
    { Model m = Make(); m.speech_res = AI_ACTION_FAILED; Check(!m.Run() && m.moves == 0 && m.remove_calls == 0, 11); }
    { Model m = Make(); m.speech_res = AI_ACTION_IN_PROGRESS; Check(!m.Run() && m.moves == 0, 12); }
    { Model m = Make(); m.add_mode_ok = false; Check(m.Run() && m.remove_calls == 0, 13); }
    // frame budget: stop = (long)(100 + 10 * 3.5) = 135; frames advance 10 per call after the first
    { Model m = Make(); for (int i = 0; i < 8; ++i) m.move_res[i] = AI_ACTION_IN_PROGRESS; m.frame_step = 10;
      bool r = m.Run(); Check(r && m.moves == 4, 14); }   // checks at 110, 120, 130 continue; 140 stops
    { Model m = Make(); for (int i = 0; i < 8; ++i) m.move_res[i] = AI_ACTION_IN_PROGRESS; m.frame_step = 5; // stop = 135
      m.Run(); Check(m.moves == 7, 15); }                 // 105..130 continue, the check at exactly 135 stops (7 moves)
    // fractional stop frame truncates: stop = (long)(100 + 10 * 3.58) = (long)135.8 = 135 (rounding would give 136: an 8th move)
    { Model m = Make(); for (int i = 0; i < 8; ++i) m.move_res[i] = AI_ACTION_IN_PROGRESS; m.frame_step = 5; m.random3 = 1.58f;
      m.Run(); Check(m.moves == 7, 20); }
    { Model m = Make(); m.move_res[0] = AI_ACTION_IN_PROGRESS; m.move_res[1] = AI_ACTION_FAILED; Check(!m.Run() && m.moves == 2 && m.remove_calls == 1, 16); }
    { Model m = Make(); m.move_res[0] = AI_ACTION_TERMINATE_PROCESS; Check(!m.Run() && m.moves == 1, 17); }
    { Model m = Make(); m.move_res[0] = AI_ACTION_NULL; Check(!m.Run(), 18); }
    if (fails) { std::printf("00985440_TEST FAIL\n"); return 1; }
    std::printf("00985440_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x008643A0 NCreatureAI::Talk(CThingAICreature&, CThingCreatureBase&, const CCharString&, bool, bool)
// The model mirrors the source: the sound needs the creature's sound player and entity-sound def and the listener's
// event component; with tell_listener the sound is skipped while the listener already holds an event of type 0x12, and
// a "hero has been spoken to" event is posted after playing; an unknown sound name plays nothing; the def is released.
#include <cstdio>
#include <cstring>

static int g_def_refs = 0;
static int g_plays = 0;
static int g_events_posted = 0;
static bool g_last_play_args_ok = true;

struct SoundDef { const char* names[3]; };
struct Creature { bool has_sound_player; bool has_def; SoundDef def; };
struct Listener { bool has_events; bool has_type_0x12_event; };

struct DefRef { const SoundDef* p; DefRef() : p(0) { } ~DefRef() { if (p) --g_def_refs; } };

static const char* GetPSoundPair(const SoundDef& def, const char* name)
{
    for (int i = 0; i < 3; ++i)
        if (def.names[i] && std::strcmp(def.names[i], name) == 0) return def.names[i];
    return 0;
}
static void PlayAttachedSample(const char*, bool a, bool b, bool c) { ++g_plays; g_last_play_args_ok = a && !b && !c; }

static void Talk(Creature& creature, Listener& listener, const char* sound_name, bool, bool tell_listener)
{
    const char* name = sound_name;
    DefRef def;
    if (creature.has_sound_player) {
        if (creature.has_def) { def.p = &creature.def; ++g_def_refs; }
        if (def.p != 0 && listener.has_events) {
            if (!tell_listener || !listener.has_type_0x12_event) {
                const char* pair = GetPSoundPair(*def.p, name);
                if (pair) {
                    PlayAttachedSample(pair, true, false, false);
                    if (tell_listener) { ++g_events_posted; listener.has_type_0x12_event = true; }
                }
            }
        }
    }
}

int main()
{
    int fail = 0;
    Creature c = { true, true, { { "GREET", "INSULT", 0 } } };
    Listener l = { true, false };
    Talk(c, l, "GREET", true, false);
    if (g_plays != 1 || g_events_posted != 0 || !g_last_play_args_ok) fail |= 1;
    Talk(c, l, "INSULT", false, true);                     // posts the event
    if (g_plays != 2 || g_events_posted != 1) fail |= 2;
    Talk(c, l, "GREET", false, true);                      // listener already told: skipped
    if (g_plays != 2 || g_events_posted != 1) fail |= 4;
    Talk(c, l, "GREET", false, false);                     // without tell_listener it still plays
    if (g_plays != 3) fail |= 8;
    Talk(c, l, "UNKNOWN", false, false);
    if (g_plays != 3) fail |= 16;
    Listener none = { false, false };
    Talk(c, none, "GREET", false, true);
    if (g_plays != 3 || g_events_posted != 1) fail |= 32;
    Creature mute = c; mute.has_sound_player = false;
    Talk(mute, l, "GREET", false, false);
    if (g_plays != 3) fail |= 64;
    Creature nodef = c; nodef.has_def = false;
    Talk(nodef, l, "GREET", false, false);
    if (g_plays != 3) fail |= 128;
    if (g_def_refs != 0) fail |= 256;
    if (fail) { std::printf("008643a0_TEST FAIL\n"); return 1; }
    std::printf("008643a0_TEST PASS\n");
    return 0;
}
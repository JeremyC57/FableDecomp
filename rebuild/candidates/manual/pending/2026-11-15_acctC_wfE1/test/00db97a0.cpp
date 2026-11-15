// Behaviour test for NScript::CQ_NewOakValeIntroScript::CNOVI_Theresa::Main (0x00DB97A0): before the intro is done she skips about
// until the hero comes within 5 units, then plays the meeting cut-scene and (hero holding the chocolate box) asks the gift question;
// "yes" kills the guards, plays the yes cut-scene, sets GivenTheresaChocs, takes the box and sets the objective. Afterwards, per frame:
// talk with the box -> the question (yes: hello + gift, no: "really get present"); presenting the box -> hello + gift; presenting
// anything else -> "better present"; plain talk -> hello (gift given) / "get present" once / "really get present"; a hit (not a heal
// spell) -> allies, a bad deed, "don't hit"; otherwise skip about while idle. Once the gift is given and the hero reaches the outro
// trigger: quest info off, the raid cut-scene, the video, fade out, prison music, AttackOver, end. Speech only while she has health.
// A state model of the script's decisions (the row's own code needs the game).
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum Event { NONE, TALK, PRESENT_CHOC, PRESENT_OTHER, HIT, HEAL_HIT };
struct Theresa {
    bool DoneIntro, SpokenAlready, HasBox, GivenChocs, AttackOver, Terminate;
    int Frame, NearAt, OutroAt, Answer, BadDeeds;
    float Health;
    const Event* Events;
    void NewScriptFrame() { ++Frame; if (Frame > 40) Terminate = true; }
    bool T() const { return Terminate; }
    void Say(const char* line) { if (Health > 0.0f) Log(line); }
    void Gift() { GivenChocs = true; HasBox = false; Log("objective05"); }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        bool leaving_sister_switch = false;
        for (; true; NewScriptFrame()) {
            if (T()) return;
            if (!DoneIntro) {
                while (Frame < NearAt) { Log("skip"); NewScriptFrame(); if (T()) return; }
                Log("CS_MEET");
                if (HasBox) {
                    Log("question");
                    if (Answer == 1) { Log("killguards"); Log("CS_MEET_YES"); Gift(); leaving_sister_switch = true; }
                }
                DoneIntro = true;
                continue;
            }
            Event ev = Events[Frame];
            if (ev == TALK && HasBox) {
                Log("question");
                if (Answer == 1) { Log("killguards"); Say("HELLO"); Gift(); leaving_sister_switch = true; }
                else Say("REALLY_GET_PRESENT");
            } else if (ev == PRESENT_CHOC) {
                Log("killguards"); Say("HELLO"); Gift(); leaving_sister_switch = true;
            } else if (ev == PRESENT_OTHER) {
                Say("BETTER_PRESENT");
            } else if (ev == TALK) {
                if (leaving_sister_switch) Say("HELLO");
                else if (!SpokenAlready) { SpokenAlready = true; Say("GET_PRESENT"); }
                else Say("REALLY_GET_PRESENT");
            } else if (ev == HIT) {
                Log("allies"); ++BadDeeds; Say("DONT_HIT");
            } else if (!leaving_sister_switch) {
                Log("skip");
            }
            if (leaving_sister_switch && Frame >= OutroAt) {
                Log("CS_OUTRO"); Log("video"); Log("fade"); Log("prison");
                AttackOver = true;
                break;
            }
        }
    }
};
int main()
{
    // a: no box at the intro; talks twice, a hit, a wrong present, then the box presented; outro at frame 9.
    static const Event ea[] = { NONE, NONE, NONE, TALK, TALK, HIT, PRESENT_OTHER, HEAL_HIT, PRESENT_CHOC, NONE, NONE, NONE };
    Theresa a = { false, false, false, false, false, false, 0, 2, 9, 0, 0, 10.0f, ea };
    a.Main();
    bool ok = !std::strcmp(g_log, "skip;CS_MEET;GET_PRESENT;REALLY_GET_PRESENT;allies;DONT_HIT;BETTER_PRESENT;skip;killguards;HELLO;"
                                  "objective05;CS_OUTRO;video;fade;prison;") && a.AttackOver && a.GivenChocs && a.BadDeeds == 1;
    g_log[0] = 0;
    // b: box at the intro, answer yes there; the outro waits for the trigger (frame 5), talk in between says hello.
    static const Event eb[] = { NONE, NONE, NONE, TALK, NONE, NONE, NONE };
    Theresa b = { false, false, true, false, false, false, 0, 1, 5, 1, 0, 10.0f, eb };
    b.Main();
    ok = ok && !std::strcmp(g_log, "CS_MEET;question;killguards;CS_MEET_YES;objective05;HELLO;CS_OUTRO;video;fade;prison;") && b.AttackOver;
    g_log[0] = 0;
    // c: box kept (answer no at the intro and at a talk), no health: no speech; never given -> terminates without the outro.
    static const Event ec[50] = { NONE, NONE, NONE, TALK, HIT };
    Theresa c = { false, false, true, false, false, false, 0, 1, 3, 0, 0, 0.0f, ec };
    c.Main();
    ok = ok && !std::strncmp(g_log, "CS_MEET;question;skip;question;allies;skip;", 43) && !c.AttackOver && !c.GivenChocs && c.HasBox;
    std::printf(ok ? "00db97a0_TEST PASS\n" : "00db97a0_TEST FAIL\n");
    return ok ? 0 : 1;
}

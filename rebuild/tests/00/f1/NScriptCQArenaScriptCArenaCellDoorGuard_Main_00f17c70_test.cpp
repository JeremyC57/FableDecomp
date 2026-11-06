// Behaviour test for CArenaCellDoorGuard::Main (0x00F17C70): the per-frame step of the arena cell door guard -- talk by arena
// state (early first / second chat, the late question once, then the "second question" with one ROUND line pair per arena
// round and extra lines in rounds 1 and 7; a yes starts the arena), the hit cut-scene with its re-entry guard, and the walk back
// to / facing of GuardingDoorMarkerRight.
#include <cstdio>
#include <cstring>

enum { MAS_CELLS_PRE_ARENA_NORMAL = 0, MAS_CELLS_PRE_ARENA_NORMAL_GO = 1, MAS_IN_ARENA = 2, MAS_OTHER = 3 };

struct Parent { long ArenaState, ArenaRound; bool InHitCutsceneAlready, NeedBertForSpeech; };

struct DoorGuard {
    Parent* ParentClass;
    bool EarlyTalk, ChamTalk, Turned;
    long Lines, Heals, Walks, Faces, Fades;
    char Last[64];
    void Say(const char* text) { ++Lines; std::strcpy(Last, text); }
    void YesSequence(long round)
    {
        ParentClass->NeedBertForSpeech = true;
        char line[64];
        std::sprintf(line, "ROUND%ld", round); Say(line);   // seh_me line
        Say(line);                                         // seh_guard2 reply
        if (round == 1) Say("ROUND1_SECOND");
        if (round == 7) { Say("ROUND7_SECOND"); Say("GUARD2_ROUND7_SECOND"); }
        ++Fades;
        ParentClass->ArenaState = MAS_IN_ARENA;
        ParentClass->NeedBertForSpeech = false;
    }
    void Step(bool talked, bool hit, float distToMarker, bool busy, long answer)
    {
        if (talked) {
            if (ParentClass->ArenaState == MAS_CELLS_PRE_ARENA_NORMAL) {
                if (!EarlyTalk) { Say("EARLY_FIRST"); EarlyTalk = true; }
                else Say("EARLY_SECOND");
            } else if (ParentClass->ArenaState == MAS_CELLS_PRE_ARENA_NORMAL_GO && !ChamTalk) {
                Say("LATE_QUESTION");
                if (answer == 1) YesSequence(0); else Say("NO");
                ChamTalk = true;
            } else {
                Say("SECOND_QUESTION");
                if (answer == 1) YesSequence(ParentClass->ArenaRound); else Say("ANOTHER_REFUSAL");
            }
        } else if (hit) {
            if (!ParentClass->InHitCutsceneAlready) {
                ParentClass->InHitCutsceneAlready = true;
                Say("ATTACKED"); ++Heals;
                ParentClass->InHitCutsceneAlready = false;
            }
        } else if (distToMarker > 1.0f && !busy) {
            ++Walks; Turned = false;
        } else if (!Turned) {
            if (!busy) { Turned = true; ++Faces; }
        }
    }
};

int main()
{
    Parent p = { MAS_CELLS_PRE_ARENA_NORMAL, 0, false, false };
    DoorGuard g; std::memset(&g, 0, sizeof g); g.ParentClass = &p;
    bool ok = true;
    g.Step(true, false, 0.0f, false, -1);
    ok = ok && std::strcmp(g.Last, "EARLY_FIRST") == 0 && g.EarlyTalk;
    g.Step(true, false, 0.0f, false, -1);
    ok = ok && std::strcmp(g.Last, "EARLY_SECOND") == 0;
    p.ArenaState = MAS_CELLS_PRE_ARENA_NORMAL_GO;
    g.Step(true, false, 0.0f, false, 0);
    ok = ok && std::strcmp(g.Last, "NO") == 0 && g.ChamTalk && p.ArenaState == MAS_CELLS_PRE_ARENA_NORMAL_GO;
    g.Step(true, false, 0.0f, false, 0);                     // ChamTalk set: the second-question branch
    ok = ok && std::strcmp(g.Last, "ANOTHER_REFUSAL") == 0;
    p.ArenaRound = 7;
    long before = g.Lines;
    g.Step(true, false, 0.0f, false, 1);
    ok = ok && g.Lines == before + 5 && p.ArenaState == MAS_IN_ARENA && g.Fades == 1 && !p.NeedBertForSpeech;
    g.Step(false, true, 0.0f, false, -1);
    ok = ok && g.Heals == 1 && !p.InHitCutsceneAlready;
    p.InHitCutsceneAlready = true;
    g.Step(false, true, 0.0f, false, -1);
    ok = ok && g.Heals == 1;
    g.Step(false, false, 5.0f, false, -1);
    ok = ok && g.Walks == 1 && !g.Turned;
    g.Step(false, false, 0.5f, true, -1);
    ok = ok && g.Faces == 0;
    g.Step(false, false, 0.5f, false, -1);
    g.Step(false, false, 0.5f, false, -1);
    ok = ok && g.Faces == 1 && g.Turned;
    if (!ok) { std::printf("00f17c70_TEST FAIL\n"); return 1; }
    std::printf("00f17c70_TEST PASS\n");
    return 0;
}
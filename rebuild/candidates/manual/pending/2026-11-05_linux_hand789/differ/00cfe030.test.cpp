// Behaviour test for NScript::CQ_AwakeningTheOracleScript::COracleTablet::Main (0x00CFE030): a tablet spot waits for the
// digging-spot rewards, gets a minimap marker when it holds a tablet (BAD_ONE / GOOD_ONE), keeps its sparkles alive while the
// spot is enabled (re-created when dead and the 4 s timer ran out), retires the spot once more than two tablets are collected,
// and once dug gives the Y/M/C tablet the hero carries (first missing letter wins) or, for BAD_ONE, spawns creatures at the
// spawn points (data string unless NULL/empty, else CREATURE_MINION_DREADWINGS) until the alive cap is passed.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[1024]; int frame, termFrame, rewardsFrame, enabledUntil, tabletsCollected, spawnedAlive, maxSpawned;
    const char* data; bool gotY, gotM, gotC, hasY, hasM, hasC, alreadyDug, displayedBad, sparklesAlive; int timer;
    const char* spawn[4]; int spawnCount;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void Frame() { ++frame; if (timer > 0) --timer; }
    bool Give(bool& got, bool has, const char* ev)
    {
        if (got || !has) return false;
        Ev(ev); got = true; ++tabletsCollected; return true;
    }
    void Main()
    {
        while (frame < rewardsFrame) { Frame(); if (Term()) return; }
        if (Term()) return;
        bool marked = std::strcmp(data, "BAD_ONE") == 0 || std::strcmp(data, "GOOD_ONE") == 0;
        if (marked) Ev("MARK");
        sparklesAlive = true; timer = 0; bool removed = false;
        while (frame < enabledUntil) {
            Frame(); if (Term()) return;
            if (timer == 0 && !sparklesAlive) { Ev("SPARKLE"); sparklesAlive = true; timer = 4; }
            if (!removed && tabletsCollected > 2) { Ev("RETIRE"); removed = true; enabledUntil = frame; }
            if (frame % 3 == 0) sparklesAlive = false;
        }
        if (Term()) return;
        Ev("UNMARK");
        if (!alreadyDug && std::strcmp(data, "BAD_ONE") != 0) {
            if (!Give(gotY, hasY, "Y") && !Give(gotM, hasM, "M")) Give(gotC, hasC, "C");
        } else if (!alreadyDug) {
            if (!displayedBad) { Ev("CURSED"); displayedBad = true; }
            for (int i = 0; i < spawnCount; ++i) {
                if (spawnedAlive > maxSpawned) break;
                const char* s = spawn[i];
                Ev(std::strcmp(s, "NULL") != 0 && std::strcmp(s, "") != 0 ? s : "DREADWINGS");
                ++spawnedAlive;
            }
            Ev("REMOVE_CURSED");
        }
        alreadyDug = true; Ev("FADE");
    }
};
static void Make(Sim& s, const char* data)
{
    std::memset(&s, 0, sizeof s); s.termFrame = 100; s.rewardsFrame = 2; s.enabledUntil = 10; s.data = data; s.maxSpawned = 1;
}
int main()
{
    Sim a; Make(a, "GOOD_ONE"); a.hasM = true; a.hasC = true; a.Main();
    bool ok = std::strcmp(a.log, "MARK;SPARKLE;SPARKLE;UNMARK;M;FADE;") == 0 && a.tabletsCollected == 1 && a.alreadyDug;
    Sim b; Make(b, "BAD_ONE"); b.spawn[0] = "CREATURE_BANSHEE"; b.spawn[1] = "NULL"; b.spawn[2] = "X"; b.spawnCount = 3; b.Main();
    ok = ok && std::strcmp(b.log, "MARK;SPARKLE;SPARKLE;UNMARK;CURSED;CREATURE_BANSHEE;DREADWINGS;REMOVE_CURSED;FADE;") == 0;
    Sim c; Make(c, "PLAIN"); c.tabletsCollected = 3; c.Main();
    ok = ok && std::strcmp(c.log, "RETIRE;UNMARK;FADE;") == 0;
    Sim d; Make(d, "PLAIN"); d.termFrame = 1; d.Main();
    ok = ok && d.log[0] == 0;
    std::printf(ok ? "00cfe030_TEST PASS\n" : "00cfe030_TEST FAIL\n");
    return ok ? 0 : 1;
}

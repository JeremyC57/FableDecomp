// Behaviour test for NScript::CQ_AmbushTradersScript::CAT_BanditContact::Main (0x00F0A960): the bandit contact wanders; each frame,
// when the hero attacked her she says CONTACT_ATTACK (while alive) and clears the flag; else once the intro is done she leaves when
// BowerstonePosh unloads (other regions are ignored); else when the hero first talks to her she plays CS_GIBBETHILL_AMBUSH_PREINTRO,
// marks the intro done, teleports the hero to the jail and removes herself. A frame-stepped model.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Contact {
    int Frame, TerminateAt, AttackAt, UnloadAt, TalkAt;
    bool Attacked, IntroDone, Alive, Removed;
    const char* Region;
    void NSF() { ++Frame; if (Frame == AttackAt) Attacked = true; }
    bool T() const { return Frame >= TerminateAt || Removed; }
    void Main()
    {
        NSF(); if (T()) return;
        Log("wander");
        for (;; NSF()) {
            if (T()) return;
            if (Attacked) {
                if (Alive) Log("ATTACK");
                Log("wander");
                Attacked = false;
            }
            else if (IntroDone && Frame == UnloadAt) {
                if (!std::strcmp(Region, "BowerstonePosh")) { Log("remove"); Removed = true; }
            }
            else if (Frame == TalkAt) {
                if (!IntroDone) {
                    Log("PREINTRO");
                    IntroDone = true;
                    Log("jail");
                    Log("remove");
                    Removed = true;
                }
            }
        }
    }
};
int main()
{
    Contact a = { 0, 100, 3, 50, 5, false, false, true, false, "BowerstonePosh" };
    a.Main();
    bool ok = !std::strcmp(g_log, "wander;ATTACK;wander;PREINTRO;jail;remove;") && a.IntroDone && a.Frame == 6;
    g_log[0] = 0;
    Contact b = { 0, 100, 2, 4, 50, false, true, false, false, "BowerstonePosh" };
    b.Main();
    ok = ok && !std::strcmp(g_log, "wander;wander;remove;") && b.Frame == 5;
    g_log[0] = 0;
    Contact c = { 0, 6, 50, 3, 4, false, true, true, false, "Oakvale" };
    c.Main();
    ok = ok && !std::strcmp(g_log, "wander;") && !c.Removed && c.Frame == 6;
    std::printf(ok ? "00f0a960_TEST PASS\n" : "00f0a960_TEST FAIL\n");
    return ok ? 0 : 1;
}

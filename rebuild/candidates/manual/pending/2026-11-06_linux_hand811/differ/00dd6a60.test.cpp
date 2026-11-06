// Behaviour test for NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Main (0x00DD6A60): in brain state 1 the mother
// nags every 10 seconds (GET_WEAPONS until the hero has his weapons, then USE_LEVER until the cell door is open), answers a talk the
// same way, and a lever use without weapons gives NOSTUFF_01/02 + objective 03 while with weapons it moves to state 2 and saves; in
// the torture chamber the door opens (GOT_WEAPONS); in state 2 she follows (HURRY) with hit-by-hero / hit-by-enemy comments on a
// 15 second timer until the kraken; after the kraken and once the graveyard loads the hero is teleported to its entrance.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Mother {
    int Frames, TerminateAt, TalkMask, LeverMask, HeroHitMask, EnemyHitMask, WeaponsAt, KrakenFrom, KrakenTo, GraveyardAt;
    long BrainState, NagTimer, CommentTimer;
    bool HasWeapons, DoorOpen, Following, Saved, TortureChamber, EntranceAlive, Teleported;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; if (NagTimer > 0) --NagTimer; if (CommentTimer > 0) --CommentTimer; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    bool Kraken() const { return Frames >= KrakenFrom && Frames < KrakenTo; }
    void Comment() {
        if (Bit(HeroHitMask)) { Log("HIT_BY_HERO"); CommentTimer = 15; }
        else if (CommentTimer == 0 && Bit(EnemyHitMask)) { Log("HIT_BY_ENEMY"); CommentTimer = 15; }
    }
    void Main() {
        NagTimer = 10;
        while (BrainState == 1) {
            HasWeapons = Frames >= WeaponsAt;
            if (NagTimer == 0) {
                if (!HasWeapons) { NagTimer = 10; Log("nag_GET_WEAPONS"); }
                else if (!DoorOpen) { NagTimer = 10; Log("nag_USE_LEVER"); }
            }
            if (Bit(TalkMask)) {
                if (!HasWeapons) { NagTimer = 10; Log("GET_WEAPONS"); }
                else if (!DoorOpen) { NagTimer = 10; Log("USE_LEVER"); }
            }
            if (Bit(LeverMask)) {
                if (HasWeapons) { BrainState = 2; Saved = true; }
                else Log("NOSTUFF_01;NOSTUFF_02;OBJECTIVE_03");
            }
            if (BrainState != 1) break;
            NSF();
            if (T()) return;
        }
        if (TortureChamber) { DoorOpen = true; Log("GOT_WEAPONS"); }
        if (BrainState == 2) {
            Following = true; Log("HURRY");
            CommentTimer = 0;
            while (BrainState == 2 && !Kraken()) { Comment(); NSF(); if (T()) return; }
        }
        while (Kraken()) { NSF(); if (T()) return; }
        CommentTimer = 0;
        while (Frames < GraveyardAt) { Comment(); NSF(); if (T()) return; }
        if (EntranceAlive) { Teleported = true; Log("teleport"); }
        for (;;) { NSF(); if (T()) return; }
    }
};
int main()
{
    Mother a = { 0, 60, (1 << 3), (1 << 5) | (1 << 14), (1 << 17), (1 << 18) | (1 << 25) | (1 << 30), 12, 24, 27, 33,
                 1, 0, 0, false, false, false, false, true, true, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "GET_WEAPONS;NOSTUFF_01;NOSTUFF_02;OBJECTIVE_03;nag_USE_LEVER;GOT_WEAPONS;HURRY;HIT_BY_HERO;"
                                  "HIT_BY_ENEMY;teleport;")
              && a.BrainState == 2 && a.Saved && a.DoorOpen && a.Following && a.Teleported && a.Frames == 60;
    g_log[0] = 0;
    Mother b = { 0, 25, 0, 0, 0, 0, 5, 99, 99, 99, 1, 0, 0, false, false, false, false, false, true, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "nag_USE_LEVER;nag_USE_LEVER;") && b.BrainState == 1 && !b.Teleported;
    std::printf(ok ? "00dd6a60_TEST PASS\n" : "00dd6a60_TEST FAIL\n");
    return ok ? 0 : 1;
}

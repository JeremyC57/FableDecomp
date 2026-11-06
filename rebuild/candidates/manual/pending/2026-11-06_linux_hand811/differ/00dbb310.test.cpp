// Behaviour test for NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::Main (0x00DBB310): the bully walks home, then each frame:
// after the intro a talk while the hero carries the teddy offers it (yes: FOUND_TEDDY_TWO + GivenTeddy), presenting the teddy gives it,
// presenting anything else gets DONT_WANT; a talk before the intro is GET_LOST (DoneIntro), afterwards IN_COMMON once / NASTY_STREAK
// (< 3 hits) / DONT_HIT_ME when the hero attacked the victim, else BADGERING; a hit adds the health bar once and counts hits until
// InitialHealth, then the bully is subdued, runs off in BULLYRUN1 (+ BULLYRUN2 and the teddy for the hero if not yet given) and is
// removed; with no hits taken the intimidation line cycles 10, 20, 30, 40, 10 when the hero is near and the talk timer is out.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Bully {
    int Frames, TerminateAt, HomeDistance, TalkMask, TeddyMask, OtherItemMask, HitMask, NearMask, AnswerYes;
    long InitialHealth, HitsTaken, IntimidateSpeechLoop, TalkTimer, HealthBar;
    bool DoneIntro, HasTeddy, HeroAttackedVictim, SaidPiece, Spoken, Subdued, RanOff, GivenHeroTeddy, GaveTeddy, Removed;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; if (TalkTimer > 0) --TalkTimer; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    void Main() {
        if (T()) return;
        while (HomeDistance > 2) { --HomeDistance; NSF(); if (T()) return; }
        for (; !T(); NSF()) {
            if (DoneIntro) {
                if (Bit(TalkMask) && HasTeddy) {
                    Log("FOUND_TEDDY_ONE");
                    if (AnswerYes) { Log("FOUND_TEDDY_TWO"); GaveTeddy = true; HasTeddy = false; }
                } else if (Bit(TeddyMask) && HasTeddy) { Log("FOUND_TEDDY_TWO"); GaveTeddy = true; HasTeddy = false; }
                else if (Bit(OtherItemMask)) Log("DONT_WANT");
            }
            if (Bit(TalkMask)) {
                if (!DoneIntro) { Log("GET_LOST"); DoneIntro = true; }
                else if (HeroAttackedVictim) {
                    if (!SaidPiece) { Log("IN_COMMON"); SaidPiece = true; }
                    else if (HitsTaken < 3) Log("NASTY_STREAK");
                    else Log("DONT_HIT_ME");
                } else Log("BADGERING");
            }
            if (Bit(HitMask)) {
                if (HealthBar == -999) { HealthBar = 1; Log("health_bar"); }
                if (++HitsTaken < InitialHealth) Log("GET_OFF");
                else {
                    Subdued = true; Log("BULLYRUN1");
                    if (!GivenHeroTeddy) { Log("BULLYRUN2"); GivenHeroTeddy = true; }
                    RanOff = true; Removed = true;
                    return;
                }
            }
            if (TalkTimer == 0 && Bit(NearMask) && HitsTaken == 0) {
                Spoken = true; TalkTimer = 3;
                char line[16]; std::sprintf(line, "INTIMIDATE_%ld", IntimidateSpeechLoop); Log(line);
                IntimidateSpeechLoop += 10;
                if (IntimidateSpeechLoop > 40) IntimidateSpeechLoop = 10;
            }
        }
    }
};
int main()
{
    Bully a = { 0, 30, 4, (1 << 3) | (1 << 9) | (1 << 12), (1 << 10), (1 << 11), (1 << 14) | (1 << 15) | (1 << 16), 0x3F, 1,
                3, 0, 10, 0, -999, false, true, false, false, false, false, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "INTIMIDATE_10;GET_LOST;INTIMIDATE_20;FOUND_TEDDY_ONE;FOUND_TEDDY_TWO;BADGERING;DONT_WANT;"
                                  "BADGERING;health_bar;GET_OFF;GET_OFF;BULLYRUN1;BULLYRUN2;")
              && a.GaveTeddy && a.Subdued && a.RanOff && a.Removed && a.Frames == 16 && a.IntimidateSpeechLoop == 30;
    g_log[0] = 0;
    Bully b = { 0, 6, 0, (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4), 0, 0, 0, 0, 0,
                5, 3, 40, 0, 1, true, false, true, false, false, false, false, true, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "IN_COMMON;DONT_HIT_ME;DONT_HIT_ME;DONT_HIT_ME;") && !b.Removed;
    std::printf(ok ? "00dbb310_TEST PASS\n" : "00dbb310_TEST FAIL\n");
    return ok ? 0 : 1;
}

// Behaviour test for NScript::CV_BordelloScript::CBordelloLady::Main (0x00E3EB10): every frame a hero talk (no crime known in the
// village) picks one line: nunnery = HAPPY; hero tricking dressed as a lady (not owned) = MAGICIAN_HINT; player-owned = PARTY_FREE
// + question (yes = follow me, quickie cut-scene, sex count +1, walk downstairs; no = declined); otherwise introduction / reminder /
// party again + the paid question (yes and enough gold = pay, full cut-scene first time then quickie; else too expensive / declined).
// A nunnery retires her once; a hero hit (not a heal spell) makes her cower.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Lady {
    int Frames, TerminateAt, Gold, GoldRequired, SexCount, TalkMask, HitMask, HealMask, Answer;
    bool Nunnery, Tricking, Dressed, Owned, CrimeKnown, Hedwig, HaveTalked, Partied, HadSex, Walking, Retired, AwayFromHome;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    void Sex(bool quickie) {
        Log(quickie ? (Hedwig ? "CS_QUICKIE_HEDWIG" : "CS_QUICKIE") : (Hedwig ? "CS_FULL_HEDWIG" : "CS_FULL"));
        Partied = true; ++SexCount; Walking = true;
    }
    void Talk() {
        if (Nunnery) Log("HAPPY");
        else if (Tricking && Dressed && !Owned) Log("MAGICIAN_HINT");
        else if (Owned) {
            Log("PARTY_FREE");
            if (Answer == 1) { Log("PARTY_FREE_FOLLOW_ME"); Sex(true); } else Log("PARTY_FREE_DECLINED");
        } else {
            if (HaveTalked) Log(Partied ? "PARTY_AGAIN" : "PARTY_PAID_REMINDER");
            else { Log("PARTY_PAID_INTRODUCTION"); HaveTalked = true; }
            if (Answer == 1) {
                if (Gold >= GoldRequired) { Log("PARTY_PAID_FOLLOW_ME"); Gold -= GoldRequired; Sex(HadSex); HadSex = true; }
                else Log("PARTY_PAID_TOO_EXPENSIVE");
            } else Log("PARTY_PAID_DECLINED");
        }
        if (Walking && AwayFromHome) Log("walk_home");
        else Walking = false;
    }
    void Main() {
        NSF(); if (T()) return;
        for (; !T(); NSF()) {
            if (Bit(TalkMask) && !CrimeKnown) Talk();
            if (!Retired && Nunnery) { Retired = true; Log("RETIRED"); }
            if (Bit(HitMask) && !Bit(HealMask)) Log("COWER");
        }
    }
};
int main()
{
    Lady a = { 0, 10, 300, 200, 0, (1 << 2) | (1 << 4) | (1 << 6), 0, 0, 1,
               false, false, false, false, false, false, false, false, false, false, false, true };
    a.Main();
    bool ok = !std::strcmp(g_log, "PARTY_PAID_INTRODUCTION;PARTY_PAID_FOLLOW_ME;CS_FULL;walk_home;"
                                  "PARTY_AGAIN;PARTY_PAID_TOO_EXPENSIVE;walk_home;PARTY_AGAIN;PARTY_PAID_TOO_EXPENSIVE;walk_home;")
              && a.Gold == 100 && a.SexCount == 1 && a.HadSex;
    g_log[0] = 0;
    Lady b = { 0, 6, 0, 200, 4, (1 << 3), 0, 0, 1,
               false, false, false, true, false, true, false, false, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "PARTY_FREE;PARTY_FREE_FOLLOW_ME;CS_QUICKIE_HEDWIG;") && b.SexCount == 5 && !b.Walking;
    g_log[0] = 0;
    Lady c = { 0, 6, 0, 200, 0, (1 << 2), (1 << 3) | (1 << 4), (1 << 4), 0,
               true, false, false, false, false, false, false, false, false, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "RETIRED;HAPPY;COWER;");
    g_log[0] = 0;
    Lady d = { 0, 6, 0, 200, 0, (1 << 2), 0, 0, 0,
               false, true, true, false, true, false, false, false, false, false, false, false };
    d.Main();
    ok = ok && g_log[0] == 0;
    std::printf(ok ? "00e3eb10_TEST PASS\n" : "00e3eb10_TEST FAIL\n");
    return ok ? 0 : 1;
}
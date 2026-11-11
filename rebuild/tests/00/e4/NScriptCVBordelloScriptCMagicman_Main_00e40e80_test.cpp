// Behaviour test for NScript::CV_BordelloScript::CMagicman::Main (0x00E40E80): until the hero owns the bordello the magician
// answers a talk (not yet tricking: dressed as a lady = PIMPING, first time = TOUTING, pimp hat once = PIMP_HAT, else CHATTER_01..04
// cycling; tricking: not dressed = TOUTING, beard = BEARD_LADY, moustache = BEARD_LADY_MOUSTACHE, else LOVELY_SKIN then whore wig =
// LOVELY_HAIR + party question (yes = HORNY + 1000 gold + sex cut-scene, quickie once the deeds location is known; no = REJECTED),
// no wig = BAD_HAIR), complains when hit (ATTACKED_FEMALE / _MALE, not for the heal spell) and drinks presented beers (BEER1..BEER6,
// the sixth plays the drunk cut-scene, then BEERTOOMUCH and the beer goes back); once the hero owns it he removes himself.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Magicman {
    int Frames, TerminateAt, OwnAt, TrickAt, TalkMask, HitMask, HealMask, BeerMask, Answer, Chat, Beers, Gold, Sex;
    bool Dressed, PimpHat, Beard, Tash, Wig, DoneToutIntro, MentionedPimpHat, FoundDeeds, Partying, Female, Removed;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; }
    bool Owned() const { return Frames >= OwnAt; }
    bool Tricking() const { return Frames >= TrickAt; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    void Hit() { if (Bit(HitMask) && !Bit(HealMask)) Log(Female ? "ATTACKED_FEMALE" : "ATTACKED_MALE"); }
    void Beer(bool party) {
        static const char* lines[6] = { "BEER1", "BEER2", "BEER3", "BEER4", "BEER5", "BEER6" };
        if (Beers < 6) {
            Log(lines[Beers]);
            if (Beers == 5) {
                Log(FoundDeeds ? "CS_DRUNK_QUICKIE" : "CS_DRUNK");
                FoundDeeds = true;
                if (party) Partying = true;
            }
        } else { Log("BEERTOOMUCH"); Log("beer_back"); }
        ++Beers;
    }
    void TalkBefore() {
        if (Dressed) Log("PIMPING");
        else if (!DoneToutIntro) { DoneToutIntro = true; Log("TOUTING"); }
        else if (!MentionedPimpHat && PimpHat) { Log("PIMP_HAT"); MentionedPimpHat = true; }
        else {
            if (Chat > 4) Chat = 1;
            static const char* chatter[5] = { "", "CHATTER_01", "CHATTER_02", "CHATTER_03", "CHATTER_04" };
            Log(chatter[Chat]);
            ++Chat;
        }
    }
    void TalkTricking() {
        if (!Dressed) Log("TOUTING");
        else if (Beard) Log("BEARD_LADY");
        else if (Tash) Log("BEARD_LADY_MOUSTACHE");
        else {
            Log("LOVELY_SKIN");
            if (Wig) {
                Log("LOVELY_HAIR");
                if (Answer == 1) {
                    Log("HORNY"); Gold += 1000; ++Sex;
                    Log(FoundDeeds ? "CS_SEX_QUICKIE" : "CS_SEX");
                    Partying = true; FoundDeeds = true;
                } else Log("REJECTED");
            } else Log("BAD_HAIR");
        }
    }
    void Main() {
        NSF(); if (T()) return;
        while (!Owned()) {
            NSF(); if (T()) return;
            while (!Tricking() && !Owned()) {
                NSF(); if (T()) return;
                if (Bit(TalkMask)) TalkBefore();
                Hit();
                if (Bit(BeerMask)) Beer(true);
            }
            if (T()) return;
            while (!Owned()) {
                NSF(); if (T()) return;
                if (Bit(TalkMask)) TalkTricking();
                Hit();
                if (Bit(BeerMask)) Beer(false);
            }
            if (T()) return;
        }
        if (T()) return;
        Removed = true;
    }
};
int main()
{
    Magicman a = { 0, 30, 20, 12, (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 13), (1 << 8) | (1 << 9), (1 << 9), (1 << 10), 1, 1, 5, 0, 0,
                   false, true, false, false, false, false, false, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "TOUTING;PIMP_HAT;CHATTER_01;CHATTER_02;CHATTER_03;ATTACKED_MALE;BEER6;CS_DRUNK;TOUTING;")
              && a.Removed && a.Partying && a.FoundDeeds && a.Beers == 6;
    g_log[0] = 0;
    Magicman b = { 0, 30, 10, 0, (1 << 2) | (1 << 4), (1 << 5), 0, (1 << 6), 1, 1, 7, 0, 0,
                   true, false, false, false, true, false, false, true, false, true, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "LOVELY_SKIN;LOVELY_HAIR;HORNY;CS_SEX_QUICKIE;ATTACKED_FEMALE;BEERTOOMUCH;beer_back;")
              && b.Gold == 1000 && b.Sex == 1 && b.Removed;
    g_log[0] = 0;
    Magicman c = { 0, 6, 20, 0, (1 << 2) | (1 << 3), 0, 0, 0, 0, 1, 0, 0, 0,
                   true, false, false, true, false, false, false, false, false, true, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "BEARD_LADY_MOUSTACHE;") && !c.Removed;
    std::printf(ok ? "00e40e80_TEST PASS\n" : "00e40e80_TEST FAIL\n");
    return ok ? 0 : 1;
}
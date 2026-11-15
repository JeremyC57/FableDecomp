// Behaviour test for NScript::CV_BordelloScript::CMadame::Main (0x00E3BB70): a model of the row's decisions.
// Intro once (lady line when the hero is dressed as a lady). Until the hero owns the bordello, every frame: face the hero when the
// turn timer has run out and he is within 8 units (re-arm to 4); a presented beer tankard is thanked for and handed back; a talk
// picks the work offer (dressed as a lady, not yet tricking: yes = joined + tricking, no = declined), the magician-leaves cut-scene
// when the hero carries the deeds (-> owned), "earn some" while tricking and dressed, else chit-chat 1..5 wrapping to 1; a hit
// (not a heal spell) plays the attacked line. Once owned: talk = nunnery thanks / work offer / earn some / become-a-nunnery
// question (first time BECOME_NUNNERY then REPEAT; yes + sure = nunnery, deeds taken), hit = attacked-pimp line.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Madame {
    int Frames, TerminateAt, TalkMask, BeerMask, HitMask, HealMask, NearMask, DeedsMask, Answers, AnswerIdx;
    long Timer, Chat;
    bool DoneIntro, Dressed, Tricking, Owned, Nunnery, Mentioned;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; if (Timer > 0) --Timer; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    long Ask() { long a = (Answers >> AnswerIdx) & 1; ++AnswerIdx; return a; }
    void Face() { if (Timer == 0 && Bit(NearMask)) { Timer = 4; Log("FACE"); } }
    bool Hit() const { return Bit(HitMask) && !Bit(HealMask); }
    void WorkOffer(bool ownedLine) {
        Log("WORK_QUESTION");
        if (Ask() == 1) { Log(ownedLine && Owned ? "HERO_JOINED_OWNED" : "HERO_JOINED"); Tricking = true; }
        else Log("HERO_DECLINED");
    }
    void Main() {
        NSF(); if (T()) return;
        Timer = 2;
        if (!DoneIntro) { Log(Dressed ? "INTRO_HEROLADY" : "INTRO"); DoneIntro = true; }
        long chat = 1;
        while (!Owned) {
            NSF(); if (T()) return;
            Face();
            if (Bit(BeerMask)) Log("GIVEN_BEER");
            if (Bit(TalkMask)) {
                if (!Tricking && Dressed) WorkOffer(false);
                else if (Bit(DeedsMask)) { Log("CS_MAGICIAN_LEAVES"); Owned = true; }
                else if (Tricking && Dressed) Log("HERO_EARN_SOME");
                else { if (chat > 5) chat = 1; char b[32]; std::sprintf(b, "CHIT_CHAT_0%ld", chat); Log(b); chat++; }
            }
            if (Hit()) Log("ATTACKED");
        }
        if (T()) return;
        while (!T()) {
            Face();
            if (Bit(TalkMask)) {
                if (Nunnery) Log("NUNNERY_THANKYOU");
                else if (Dressed) { if (!Tricking) WorkOffer(true); else Log("HERO_EARN_SOME"); }
                else {
                    Log(Mentioned ? "BECOME_NUNNERY_REPEAT" : "BECOME_NUNNERY"); Mentioned = true;
                    if (Ask() == 1) {
                        Log("BECOME_NUNNERY_SURE");
                        if (Ask() == 1) { Log("BECOME_NUNNERY_THANKS"); Nunnery = true; }
                        else Log("BECOME_NUNNERY_RECONSIDER");
                    } else Log("BECOME_NUNNERY_DECLINED");
                }
            }
            if (Hit()) Log("ATTACKED_PIMP");
            NSF();
        }
    }
};
int main()
{
    // frames: 2 talk dressed (yes -> tricking), 3 face + beer, 4 hit, 5 talk (earn some), 6 talk with deeds -> owned (the owned loop
    // sees the same talk: earn some), 7 face again (timer ran out), 8 talk (earn some), 9 hit = pimp line.
    Madame m = { 0, 11, (1 << 2) | (1 << 5) | (1 << 6) | (1 << 8), 1 << 3, (1 << 4) | (1 << 9), 0, (1 << 3) | (1 << 7), 1 << 6, 1, 0,
                 0, 1, false, true, false, false, false, false };
    m.Main();
    const char* want1 = "INTRO_HEROLADY;WORK_QUESTION;HERO_JOINED;FACE;GIVEN_BEER;ATTACKED;HERO_EARN_SOME;CS_MAGICIAN_LEAVES;HERO_EARN_SOME;FACE;HERO_EARN_SOME;ATTACKED_PIMP;";
    bool ok1 = std::strcmp(g_log, want1) == 0;
    if (!ok1) std::printf("got1 %s\n", g_log);
    // second run: not dressed, owned at once by the deeds, then the nunnery question twice (no, then yes + yes), then thanks.
    g_log[0] = 0;
    Madame n = { 0, 12, (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8), 0, 0, 0, 0, 1 << 2, (1 << 1) | (1 << 2), 0,
                 0, 1, true, false, false, false, false, false };
    n.Main();
    const char* want2 = "CS_MAGICIAN_LEAVES;BECOME_NUNNERY;BECOME_NUNNERY_DECLINED;BECOME_NUNNERY_REPEAT;BECOME_NUNNERY_SURE;BECOME_NUNNERY_THANKS;NUNNERY_THANKYOU;NUNNERY_THANKYOU;";
    bool ok2 = std::strcmp(g_log, want2) == 0;
    if (!ok2) std::printf("got2 %s\n", g_log);
    // chit-chat wraps 1..5 -> 1
    g_log[0] = 0;
    Madame c = { 0, 9, 0xfe, 0, 0, 0, 0, 0, 0, 0, 0, 1, true, false, false, false, false, false };
    c.Main();
    bool ok3 = std::strcmp(g_log, "CHIT_CHAT_01;CHIT_CHAT_02;CHIT_CHAT_03;CHIT_CHAT_04;CHIT_CHAT_05;CHIT_CHAT_01;") == 0;
    if (!ok3) std::printf("got3 %s\n", g_log);
    if (ok1 && ok2 && ok3) std::printf("00e3bb70_TEST PASS\n");
    return 0;
}

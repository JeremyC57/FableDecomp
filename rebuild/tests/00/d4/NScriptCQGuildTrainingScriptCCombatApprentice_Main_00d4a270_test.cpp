// Behaviour test for NScript::CQ_GuildTrainingScript::CCombatApprentice::Main (0x00D4A270): a state model of the combat apprentice.
// Each frame: while Q_GuildTrainingDeparture is active she shows information while all three global guild grades are below 4 (and
// clears it otherwise); she runs back to her marker when more than 4 from it and idle, else when idle, the hero is within 10 and the
// chat timer has run out she turns to him, restarts the timer (20) and says the never-graded / not-A+ / A+ comment for the global
// melee grade. When waiting for a fight and talked to: busy -> EARLY; a guild test running -> OTHER_MELEE_GRADE; Whisper dead ->
// NO_WHISPER; otherwise HELLO + the yes/no question: no -> RETURN; yes -> START (+ START_APLUS below A+), the arena fight. In the
// fight: a guild warning, a bow hit (NO_BOW) or a lightning hit (NO_LIGHTNING) abort it; a fighter below the minimum health ends
// it; Whisper insults the hero on hits (melee hit when the insult timer <= 8, block / battle hits when it is <= 0; timer 15). An
// aborted fight resets both fighters; a finished one is graded (first GUI_MeleeGrades entry <= health score), plays END + the grade
// cut scene (APLUS_PRIZE the first time) + OVER and raises the global melee grade to 7 - grade. Lines need the speaker's health.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct World {
    int Frame, TerminateAt;
    bool QuestActive; long Melee, Skill, Will; bool HeroTakingGuildTest, GuildWarningAt7, WhisperAlive, Busy, Far, HeroNear;
    long Answer; int TalkFrame; bool WaitingForFight, HasInformation;
    long ChatTimer, InsultTimer; int BowAt, MeleeHitAt, BlockHitAt; bool Blocking; int EndAt;
    float HeroHealth, WhisperHealth; float Grades[7]; bool FightFinished, StartedMeleeTesting;
    void NSF() { ++Frame; if (ChatTimer > 0) --ChatTimer; if (InsultTimer > 0) --InsultTimer; if (Frame == EndAt) WhisperHealth = 0.0f; }
    bool T() const { return Frame >= TerminateAt; }
    void Speak(const char* key) { Log(key); }
    void Main()
    {
        ChatTimer = 10;
        HasInformation = false;
        for (;; NSF()) {
            if (T()) return;
            if (QuestActive) {
                if (Melee < 4 && Skill < 4 && Will < 4) { if (!HasInformation) { Log("info"); HasInformation = true; } }
                else if (HasInformation) { Log("noinfo"); HasInformation = false; }
            }
            if (Far && !Busy) { Log("run"); Far = false; }
            else if (!Busy && HeroNear && ChatTimer <= 0) {
                ChatTimer = 20;
                Log(Melee == 0 ? "COMMENT" : Melee != 7 ? "NOT_APLUS_COMMENT" : "APLUS_COMMENT");
            }
            if (!(WaitingForFight && Frame == TalkFrame)) continue;
            if (Busy) { Speak("EARLY"); continue; }
            if (HeroTakingGuildTest) { Speak("OTHER_MELEE_GRADE"); continue; }
            if (!WhisperAlive) { Speak("NO_WHISPER"); continue; }
            Speak("HELLO");
            if (Answer == 0) { Speak("RETURN"); continue; }
            Speak("START");
            if (Melee != 7) Speak("START_APLUS");
            HeroTakingGuildTest = true; StartedMeleeTesting = true; WaitingForFight = false; FightFinished = false;
            InsultTimer = 15;
            float heroStart = HeroHealth, whisperStart = WhisperHealth;
            bool aborted = false, warning = false;
            while (!FightFinished) {
                NSF();
                if (T()) return;
                if (GuildWarningAt7 && Frame == 7) { warning = true; aborted = true; FightFinished = true; }
                if (Frame == BowAt) { Speak("NO_BOW"); FightFinished = true; aborted = true; }
                if (HeroHealth < 1.0f || WhisperHealth < 1.0f) FightFinished = true;
                if (Frame == MeleeHitAt) { if (InsultTimer <= 8) { Log("HIT_INSULT"); InsultTimer = 15; } }
                else if (Blocking && Frame == BlockHitAt) { if (InsultTimer <= 0) { Log("BLOCK_INSULT"); InsultTimer = 15; } }
                else if (Frame == BlockHitAt) { if (InsultTimer <= 0) { Log("BATTLE_INSULT"); InsultTimer = 15; } }
            }
            if (aborted) { Log(warning ? "reset_sheathe" : "reset"); WaitingForFight = true; }
            else {
                float score = (whisperStart - WhisperHealth) - (heroStart - HeroHealth);
                long grade = 0;
                for (long i = 0; i < 7; ++i) { grade = i; if (Grades[i] <= score) break; }
                Log("END");
                static const char* g[7] = { "APLUS", "A", "B", "C", "D", "E", "F" };
                if (grade == 0 && Melee != 7) Log("APLUS_PRIZE"); else Log(g[grade]);
                if (Melee < 7 - grade) Melee = 7 - grade;
                Log("OVER"); WaitingForFight = true;
            }
            StartedMeleeTesting = false; HeroTakingGuildTest = false;
        }
    }
};
static World Make()
{
    World w; std::memset(&w, 0, sizeof w);
    w.TerminateAt = 30; w.QuestActive = true; w.WhisperAlive = true; w.Answer = 1; w.TalkFrame = 3; w.WaitingForFight = true;
    w.BowAt = -1; w.MeleeHitAt = -1; w.BlockHitAt = -1; w.EndAt = 9; w.HeroHealth = 100.0f; w.WhisperHealth = 100.0f;
    static const float gr[7] = { 90.0f, 80.0f, 70.0f, 60.0f, 50.0f, 40.0f, -1000.0f };
    std::memcpy(w.Grades, gr, sizeof gr);
    return w;
}
int main()
{
    bool ok = true;
    World a = Make(); a.Far = true; a.HeroNear = true; a.MeleeHitAt = 11; a.BlockHitAt = 12; a.EndAt = 12; a.Blocking = true;
    a.Main();   // info, run, the fight from frame 3 (melee-hit insult at timer 7, block insult too soon), A+ prize, then noinfo + A+ chat
    ok = ok && !std::strcmp(g_log, "info;run;HELLO;START;START_APLUS;HIT_INSULT;END;APLUS_PRIZE;OVER;noinfo;APLUS_COMMENT;") && a.Melee == 7 && a.WaitingForFight;
    g_log[0] = 0;
    World b = Make(); b.Melee = 7; b.HeroNear = true; b.TalkFrame = 12; b.BowAt = 14; b.EndAt = -1;
    b.Main();   // grade 7: no information, A+ chat at frame 10, then the bow abort
    ok = ok && !std::strcmp(g_log, "APLUS_COMMENT;HELLO;START;NO_BOW;reset;") && b.Melee == 7 && !b.HeroTakingGuildTest;
    g_log[0] = 0;
    World c = Make(); c.Melee = 5; c.Answer = 0; c.TalkFrame = 2;
    c.Main();   // a grade of 5 hides the information; a "no" answer
    ok = ok && !std::strcmp(g_log, "HELLO;RETURN;");
    g_log[0] = 0;
    World d = Make(); d.HeroTakingGuildTest = true; d.Skill = 4;
    d.Main();
    ok = ok && !std::strcmp(g_log, "OTHER_MELEE_GRADE;");
    g_log[0] = 0;
    World e = Make(); e.WhisperAlive = false; e.Busy = true; e.TalkFrame = 4;
    e.Main();   // busy: EARLY (Whisper's state never checked)
    ok = ok && !std::strcmp(g_log, "info;EARLY;");
    g_log[0] = 0;
    World f = Make(); f.GuildWarningAt7 = true; f.Melee = 2;
    f.Main();   // guild warning abort
    ok = ok && !std::strcmp(g_log, "info;HELLO;START;START_APLUS;reset_sheathe;") && f.Melee == 2;
    g_log[0] = 0;
    World h = Make(); h.Melee = 1; h.HeroHealth = 100.0f; h.EndAt = 9; h.Grades[0] = 200.0f; h.Grades[1] = 150.0f; h.Grades[2] = 95.0f;
    h.Main();   // score 100 -> grade 2 (B), global grade raised 1 -> 5
    ok = ok && !std::strcmp(g_log, "info;HELLO;START;START_APLUS;END;B;OVER;noinfo;") && h.Melee == 5;
    std::printf("00d4a270_TEST %s\n", ok ? "PASS" : "FAIL");
    return 0;
}
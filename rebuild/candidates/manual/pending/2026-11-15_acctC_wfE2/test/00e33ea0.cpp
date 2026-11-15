// Behaviour test for NScript::CV_ArcheryCompetitionScript::CAC_Owner::Main (0x00E33EA0): a state model of the generated script.
// Start: the owner gets the really-high-score prize and the first-high-score prize unless given / pickpocketed. Per frame (stop on
// termination): a random line when the speech timer is 0 while waiting and not attacked; when attacked (and alive) the attack line and
// the owner leaves; then by contest state: WAITING + talk -> intro (first time, quest card) / behave-yourself (after a warning, which
// clears it) / welcome back, rules question (yes -> rules), start question (yes -> pay the fee and COUNTDOWN, or cash-poor); COUNTDOWN ->
// teleport, countdown, STARTED with the time limit; STARTED -> stepped out / magic (once) / shot the owner (warning) end the game;
// FINISHED -> times-up unless disqualified, the disqualification line or the score prizes (new high score: first-prize, really-cool
// prize or a random new-high prize, then renown; match; three fractions of the high score; low), back to WAITING. Lines need health.
#include <cstdio>
#include <cstring>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

enum { WAITING = 0, COUNTDOWN = 1, FINISHED = 2, STARTED = 3 };

struct Owner {
    int Frame, TerminateAt;
    int State; bool Attacked, Welcome, Warning, SteppedOut, UsedMagic, ShotOwner, GivenFirst, GivenReally, Pickpocketed;
    long Score, HighScore, Gold, Cost; float Health; long TimeLeft; int SpeechTimer;
    int TalkAt, OutsideAt, MagicAt, ShotAt; int RulesAnswer, StartAnswer;
    bool HeroHasReward, OwnerHasReward, HeroHasReally, OwnerHasReally, Left;
    bool T() const { return Frame >= TerminateAt; }
    void NSF() { ++Frame; if (SpeechTimer > 0) --SpeechTimer; if (State == STARTED && TimeLeft > 0 && --TimeLeft == 0) State = FINISHED; }   // the parent's game timer ends the game
    void Say(const char* s) { if (Health > 0.0f) Log(s); }
    void Main()
    {
        NSF();
        if (T()) return;
        Log("spawn");
        if (!GivenReally && !Pickpocketed) OwnerHasReally = true;
        if (!GivenFirst && !Pickpocketed) OwnerHasReward = true;
        for (; !T(); NSF()) {
            if (SpeechTimer == 0 && State == WAITING && !Attacked) { Log("speech"); SpeechTimer = 20; }
            if (Attacked && Health > 0.0001f) { Say("ATTACK"); Left = true; Log("leave"); }
            if (State == WAITING) {
                if (Frame == TalkAt) {
                    if (!Welcome) { Say("INTRO"); Log("card"); Welcome = true; }
                    else if (Warning) { Say("BEHAVE"); Warning = false; }
                    else Say("WELCOME_BACK");
                    SpeechTimer = 20;
                    if (RulesAnswer == 1) Say("RULES");
                    if (StartAnswer == 1) {
                        if (Gold >= -Cost) { Gold += Cost; State = COUNTDOWN; }
                        else Say("CASH_POOR");
                    }
                }
            } else if (State == COUNTDOWN) {
                Log("countdown"); TimeLeft = 3; State = STARTED;
            } else if (State == STARTED) {
                if (Frame == OutsideAt) { State = FINISHED; SteppedOut = true; TimeLeft = 0; }
                else if (Frame == MagicAt && !UsedMagic) { State = FINISHED; UsedMagic = true; TimeLeft = 0; }
                else if (Frame == ShotAt && Health > 0.0001f) { State = FINISHED; ShotOwner = true; TimeLeft = 0; Warning = true; }
            } else if (State == FINISHED) {
                if (!SteppedOut && !UsedMagic && !ShotOwner) Log("timesup");
                if (SteppedOut) Say("DQ_OUT");
                else if (UsedMagic) Say("DQ_MAGIC");
                else if (ShotOwner) Say("DQ_SHOT");
                else {
                    long score = Score, high = HighScore;
                    if (score > high) {
                        HighScore = Score; Log("update");
                        if (!HeroHasReward && OwnerHasReward && !GivenFirst) { Say("HIGH_NEW"); OwnerHasReward = false; HeroHasReward = true; GivenFirst = true; }
                        else if (!HeroHasReally && OwnerHasReally && !GivenReally) { Say("HIGH_REALLY"); OwnerHasReally = false; HeroHasReally = true; GivenReally = true; }
                        else { Say("HIGH_NEW"); Log("prize_new"); }
                        Log("renown");
                    } else if (score == high) { Say("MATCH"); Log("prize_match"); }
                    else if ((float)score >= (float)high * 0.9f) { Say("LOW_PRIZE"); Log("prize_high"); }
                    else if ((float)score >= (float)high * 0.6f) { Say("LOW_PRIZE"); Log("prize_medium"); }
                    else if ((float)score >= (float)high * 0.3f) { Say("LOW_PRIZE"); Log("prize_low"); }
                    else Say("LOW");
                }
                State = WAITING;
            }
        }
    }
};

static Owner Make(int term)
{
    Owner o; std::memset(&o, 0, sizeof o);
    o.TerminateAt = term; o.Health = 10.0f; o.Cost = -50; o.Gold = 100; o.HighScore = 100; o.SpeechTimer = 3;
    o.TalkAt = o.OutsideAt = o.MagicAt = o.ShotAt = 1000; o.RulesAnswer = o.StartAnswer = 0;
    g_log[0] = 0;
    return o;
}

int main()
{
    bool ok = true;
    Owner a = Make(10); a.TalkAt = 2; a.RulesAnswer = 1; a.StartAnswer = 1; a.Score = 150; a.Main();   // full game, new high score
    ok = ok && std::strcmp(g_log, "spawn,INTRO,card,RULES,countdown,timesup,update,HIGH_NEW,renown,") == 0 && a.Gold == 50 && a.GivenFirst && a.HighScore == 150 && a.State == WAITING;
    Owner b = Make(10); b.Welcome = true; b.Warning = true; b.TalkAt = 2; b.StartAnswer = 1; b.Gold = 10; b.Main(); // warned, too poor
    ok = ok && std::strcmp(g_log, "spawn,BEHAVE,CASH_POOR,") == 0 && !b.Warning && b.State == WAITING && b.Gold == 10;
    Owner c = Make(9); c.Welcome = true; c.TalkAt = 2; c.StartAnswer = 1; c.MagicAt = 4; c.Main();      // magic disqualifies
    ok = ok && std::strcmp(g_log, "spawn,WELCOME_BACK,countdown,DQ_MAGIC,") == 0 && c.UsedMagic;
    Owner d = Make(9); d.Welcome = true; d.TalkAt = 2; d.StartAnswer = 1; d.ShotAt = 4; d.Main();       // shooting the owner gives a warning
    ok = ok && std::strcmp(g_log, "spawn,WELCOME_BACK,countdown,DQ_SHOT,") == 0 && d.Warning;
    Owner e = Make(9); e.Welcome = true; e.TalkAt = 2; e.StartAnswer = 1; e.Score = 70; e.Main();       // 70 of 100: medium prize
    ok = ok && std::strcmp(g_log, "spawn,WELCOME_BACK,countdown,timesup,LOW_PRIZE,prize_medium,") == 0;
    Owner f = Make(9); f.Welcome = true; f.TalkAt = 2; f.StartAnswer = 1; f.Score = 100; f.GivenFirst = true; f.Main(); // match
    ok = ok && std::strcmp(g_log, "spawn,WELCOME_BACK,countdown,timesup,MATCH,prize_match,") == 0;
    Owner g = Make(9); g.Welcome = true; g.TalkAt = 2; g.StartAnswer = 1; g.Score = 120; g.GivenFirst = true; g.Main(); // really-cool prize
    ok = ok && std::strcmp(g_log, "spawn,WELCOME_BACK,countdown,timesup,update,HIGH_REALLY,renown,") == 0 && g.GivenReally;
    Owner h = Make(7); h.Pickpocketed = true; h.SpeechTimer = 0; h.Main();                              // random speech, no prizes stocked
    ok = ok && std::strcmp(g_log, "spawn,speech,") == 0 && !h.OwnerHasReward && !h.OwnerHasReally;
    Owner i = Make(4); i.Attacked = true; i.Main();                                                    // attacked: leaves, no speech
    ok = ok && std::strcmp(g_log, "spawn,ATTACK,leave,ATTACK,leave,ATTACK,leave,") == 0 && i.Left;
    if (!ok) { std::printf("%s 00e33ea0_TEST FAIL\n", g_log); return 1; }
    std::printf("00e33ea0_TEST PASS\n");
    return 0;
}

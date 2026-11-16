// Behaviour test for NScript::CV_BanditTollScript::CTollBandit::Main (0x00E4B5B0): a model of the row's decisions.
// Start: angered bandits force state 6; state 5 without the toll paid, or state 6 without anger, restart at state 2.
// State 1: hero inside the kill distance with the trader alive -> the trader scene, trader killed, state 2 (a dead trader also
// gives state 2). State 2: toll already paid -> 4; hero within 6 -> fee by followers (with / without), HALT + movie, fee speech,
// then no money -> state 3, else state 4 and the pay question (yes + enough gold = paid; yes + short = not enough; no + gold =
// has money; else far / no money); else a run-out call timer inside the call distance -> wave + call line. State 3: near warning
// on a run-out warn timer, far -> state 4, talked to -> movie + no-money line, gold >= fee -> 4. State 4: paid -> 5 (+ the paid
// conversation when this bandit is the nearest); the left / came-back / impatient lines. State 5: paid-not-moved line on a
// run-out timer. Then anger -> 6: bandit fights until told to flee, then runs to the escape point and fades out.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct TB {
    int TollState, Frame, TerminateAt;
    bool TollPaid, Angered, TraderDead, Flee, Followers, Nearest, Killed, Assassinate;
    long Fee, FeeNo, FeeWith, Gold, Answer;
    const char* Dist;   // per frame: 'k' kill range, 'h' halt (< 6), 'c' call range, 'f' far, 'n' near (warn), 't' talked to
    long CallTimer, WarnTimer;
    char D() const { return Dist[Frame] ? Dist[Frame] : 'f'; }
    bool T() const { return Frame >= TerminateAt; }
    bool NSF() { ++Frame; if (CallTimer) --CallTimer; if (WarnTimer) --WarnTimer; return T(); }
    void Pay() {
        long answer = 0;
        if (Fee == FeeNo) { Log("Q_NO_FOLLOWER"); answer = Answer; }
        else if (Fee == FeeWith) { Log("Q_WITH_FOLLOWER"); answer = Answer; }
        if (answer == 1 && Gold >= Fee) { TollPaid = true; Gold -= Fee; Log("PAID"); }
        else if (answer == 1) Log("NOT_ENOUGH_MONEY");
        else if (answer == 0 && Gold >= Fee) Log("HAS_MONEY");
        else Log("FAR_NO_MONEY");
    }
    void Main() {
        if (Angered) TollState = 6;
        if ((TollState == 5 && !TollPaid) || (!Angered && TollState == 6)) TollState = 2;
        while (TollState == 1) {
            if (NSF()) return;
            if (D() == 'k' && !TraderDead) { Log("TRADER_REFUSES"); Assassinate = true; TollState = 2; Log("TRADER_DIES"); Killed = true; Log("CALL_HERO"); }
            if (TraderDead) TollState = 2;
        }
        CallTimer = 0;
        while (TollState == 2 && !Angered) {
            if (NSF()) return;
            if (TollPaid) TollState = 4;
            if (D() == 'h') {
                Fee = Followers ? FeeWith : FeeNo;
                Log("HALT");
                if (Fee == FeeNo) Log("TALK_5"); else if (Fee == FeeWith) Log("WITH_FOLLOWER_TALK_10");
                Log("TALK_10");
                if (Gold < Fee) { Log("TALK_NO_MONEY"); TollState = 3; }
                else { Log("TALK_MONEY"); TollState = 4; Pay(); }
            } else if (CallTimer == 0 && D() == 'c') { CallTimer = 3; Log("CALL_HERO"); }
        }
        WarnTimer = 0;
        while (TollState == 3 && !Angered) {
            if (NSF()) return;
            if (WarnTimer == 0 && D() == 'n') { WarnTimer = 3; Log("NEAR_NO_MONEY"); }
            else if (D() == 'f') { Log("FAR_NO_MONEY_LINE"); TollState = 4; }
            if (D() == 't') Log("TALK_NO_MONEY");
            if (Gold >= Fee) TollState = 4;
        }
        WarnTimer = 0;
        while (TollState == 4 && !Angered) {
            if (NSF()) return;
            if (TollPaid) { TollState = 5; if (Nearest) Log("TOLLPAID_10_20_30"); }
            if (D() == 't') { Fee = Followers ? FeeWith : FeeNo; Log("IMPATIENT"); Pay(); }
            if (D() == 'g') Gold += 100;
        }
        WarnTimer = 0;
        while (TollState == 5 && !Angered) {
            if (NSF()) return;
            if (WarnTimer == 0 && D() == 'n') { WarnTimer = 3; Log("PAID_NOT_MOVED"); }
            if (D() == 'a') Angered = true;
        }
        if (Angered) TollState = 6;
        while (TollState == 6) {
            if (NSF()) return;
            if (Flee) { Log("FLEE"); Log("FADE"); return; }
            Log("FIGHT"); Flee = true;
        }
    }
};
int main()
{
    // run 1: trader killed in state 1, halted with a follower and short of gold (state 3), walks off (state 4), gets gold, talks:
    // pays, state 5, paid-not-moved warning, bandits angered, fight then flee.
    TB a = { 1, 0, 60, false, false, false, false, true, true, false, false, 0, 10, 20, 5, 1, "fkchff" "gt" "fnfa" };
    a.Main();
    const char* wa = "TRADER_REFUSES;TRADER_DIES;CALL_HERO;CALL_HERO;HALT;WITH_FOLLOWER_TALK_10;TALK_10;TALK_NO_MONEY;FAR_NO_MONEY_LINE;"
                     "IMPATIENT;Q_WITH_FOLLOWER;PAID;TOLLPAID_10_20_30;PAID_NOT_MOVED;FIGHT;FLEE;FADE;";
    bool ok = std::strcmp(g_log, wa) == 0 && a.TollPaid && a.Gold == 85 && a.Killed;
    if (!ok) std::printf("run1 got %s\n", g_log);
    // run 2: restart state 6 without anger -> state 2; halted without followers, gold exactly the fee, answers no -> has money; terminate.
    g_log[0] = 0;
    TB b = { 6, 0, 3, false, false, true, false, false, false, false, false, 0, 10, 20, 10, 0, "fh" };
    b.Main();
    bool ok2 = std::strcmp(g_log, "HALT;TALK_5;TALK_10;TALK_MONEY;Q_NO_FOLLOWER;HAS_MONEY;") == 0 && !b.TollPaid && b.TollState == 4;
    if (!ok2) std::printf("run2 got %s\n", g_log);
    // run 3: start in state 4 short of gold, talked to, answers no -> far / no money (not "has money").
    g_log[0] = 0;
    TB c = { 4, 0, 2, false, false, true, false, false, false, false, false, 0, 10, 20, 3, 0, "ft" };
    c.Main();
    bool ok3 = std::strcmp(g_log, "IMPATIENT;Q_NO_FOLLOWER;FAR_NO_MONEY;") == 0;
    if (!ok3) std::printf("run3 got %s\n", g_log);
    if (ok && ok2 && ok3) std::printf("00e4b5b0_TEST PASS\n");
    return 0;
}
// behaviour test for 00e01760 CQ_TraderEscortScript::CDarkwoodTrader::OnPredicateFail
#include <cstdio>
#include <cstring>
struct World { long timerValue; bool killedByHero; long setTimerId, setTimerValue; const char* comment; int commentKind; };
struct Script { long traderTimer; long tradersLeft; };
struct State { bool flag; };
static void OnPredicateFail(World& w, Script& s, State& st, long timer)
{
    (void)timer;
    if (w.timerValue > 0) {
        --s.tradersLeft;
        w.setTimerId = s.traderTimer; w.setTimerValue = 0;
        if (w.killedByHero) { w.comment = "HERO_KILLED_TRADER"; w.commentKind = 0; }
        else if (s.tradersLeft == 2) { w.comment = "TRADER_DIED"; w.commentKind = 1; }
        else if (s.tradersLeft == 1) { w.comment = "LAST_TRADER"; w.commentKind = 0; }
        st.flag = false;
    }
}
int main()
{
    int bad = 0;
    World w; Script s; State st;
    w.timerValue = 0; w.comment = 0; w.setTimerId = -1; s.tradersLeft = 3; s.traderTimer = 7; st.flag = true;
    OnPredicateFail(w, s, st, 1);
    if (s.tradersLeft != 3 || w.comment || !st.flag || w.setTimerId != -1) bad |= 1;      // timer expired: nothing
    w.timerValue = 5; w.killedByHero = true;
    OnPredicateFail(w, s, st, 1);
    if (s.tradersLeft != 2 || std::strcmp(w.comment, "HERO_KILLED_TRADER") || w.commentKind || st.flag || w.setTimerId != 7) bad |= 2;
    w.killedByHero = false; s.tradersLeft = 3; w.comment = 0;
    OnPredicateFail(w, s, st, 1);
    if (!w.comment || std::strcmp(w.comment, "TRADER_DIED") || w.commentKind != 1) bad |= 4;
    w.comment = 0;
    OnPredicateFail(w, s, st, 1);
    if (!w.comment || std::strcmp(w.comment, "LAST_TRADER")) bad |= 8;
    w.comment = 0;
    OnPredicateFail(w, s, st, 1);
    if (w.comment || s.tradersLeft != 0) bad |= 16;                                         // none left: no comment
    if (bad) { std::printf("00e01760_TEST FAIL\n"); return 1; }
    std::printf("00e01760_TEST PASS\n");
    return 0;
}
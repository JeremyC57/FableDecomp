// Test for NScript::CV_LostTraderScript::CLostTraderTrader::TraderYellAtHero() -- the first six calls use lines 0..5 in order (5 and up = HELP), later calls pick rand() % 5 unless the thread is terminating; each call opens a conversation with the hero, adds one line and bumps the counter.
#include <cstdio>
#include <cstring>
static int g_rand = 0;
static int fake_rand() { return g_rand++; }
struct CTrader {
    unsigned long YellAtHero;
    bool terminating;
    const char* lastLine;
    int conversations;
    bool IsActiveThreadTerminating() const { return terminating; }
    void AddLine(const char* text) { lastLine = text; }
    void TraderYellAtHero()
    {
        unsigned long index = YellAtHero;
        if (index > 5) {
            if (IsActiveThreadTerminating())
                return;
            index = fake_rand() % 5;
        }
        ++conversations;
        switch (index) {
        case 0: AddLine("TEXT_QST_042_LOSTTRADER_HEAR"); break;
        case 1: AddLine("TEXT_QST_042_LOSTTRADER_HERE"); break;
        case 2: AddLine("TEXT_QST_042_LOSTTRADER_CAN_HEAR"); break;
        case 3: AddLine("TEXT_QST_042_LOSTTRADER_THISWAY"); break;
        case 4: AddLine("TEXT_QST_042_LOSTTRADER_WHO"); break;
        default: AddLine("TEXT_QST_042_LOSTTRADER_HELP"); break;
        }
        ++YellAtHero;
    }
};
int main()
{
    CTrader t; t.YellAtHero = 0; t.terminating = false; t.lastLine = 0; t.conversations = 0;
    static const char* order[6] = { "_HEAR", "_HERE", "CAN_HEAR", "THISWAY", "_WHO", "_HELP" };
    int ok = 1;
    for (int i = 0; i < 6; ++i) {
        t.TraderYellAtHero();
        if (!std::strstr(t.lastLine, order[i])) ok = 0;
    }
    g_rand = 7;                                   // 7 % 5 = 2 -> CAN_HEAR
    t.TraderYellAtHero();
    if (!std::strstr(t.lastLine, "CAN_HEAR") || t.YellAtHero != 7) ok = 0;
    t.terminating = true; t.lastLine = 0;
    t.TraderYellAtHero();
    if (t.lastLine != 0 || t.YellAtHero != 7 || t.conversations != 7) ok = 0;
    if (!ok) { std::printf("00ea4bb0_TEST FAIL\n"); return 1; }
    std::printf("00ea4bb0_TEST PASS\n");
    return 0;
}
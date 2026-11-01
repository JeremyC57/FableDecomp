// Test for NScript::CV_LostTraderScript::CLostTraderTrader::TraderYellForHelp() -- the first six calls use lines 0..5 in order (5 and up = WHCIHWAY), later calls pick rand() % 5 unless the thread is terminating; each call opens a conversation with the hero, adds one line and bumps the counter.
#include <cstdio>
#include <cstring>
static int g_rand = 0;
static int fake_rand() { return g_rand++; }
struct CTrader {
    unsigned long YellForHelp;
    bool terminating;
    const char* lastLine;
    int conversations;
    bool IsActiveThreadTerminating() const { return terminating; }
    void AddLine(const char* text) { lastLine = text; }
    void TraderYellForHelp()
    {
        unsigned long index = YellForHelp;
        if (index > 5) {
            if (IsActiveThreadTerminating())
                return;
            index = fake_rand() % 5;
        }
        ++conversations;
        switch (index) {
        case 0: AddLine("TEXT_QST_042_LOSTTRADER_LOST"); break;
        case 1: AddLine("TEXT_QST_042_LOSTTRADER_ANYONE"); break;
        case 2: AddLine("TEXT_QST_042_LOSTTRADER_ANYBODY"); break;
        case 3: AddLine("TEXT_QST_042_LOSTTRADER_STUPID"); break;
        case 4: AddLine("TEXT_QST_042_LOSTTRADER_BROTHER"); break;
        default: AddLine("TEXT_QST_042_LOSTTRADER_WHCIHWAY"); break;
        }
        ++YellForHelp;
    }
};
int main()
{
    CTrader t; t.YellForHelp = 0; t.terminating = false; t.lastLine = 0; t.conversations = 0;
    static const char* order[6] = { "LOST", "ANYONE", "ANYBODY", "STUPID", "BROTHER", "WHCIHWAY" };
    int ok = 1;
    for (int i = 0; i < 6; ++i) {
        t.TraderYellForHelp();
        if (!std::strstr(t.lastLine, order[i])) ok = 0;
    }
    g_rand = 7;                                   // 7 % 5 = 2 -> ANYBODY
    t.TraderYellForHelp();
    if (!std::strstr(t.lastLine, "ANYBODY") || t.YellForHelp != 7) ok = 0;
    t.terminating = true; t.lastLine = 0;
    t.TraderYellForHelp();
    if (t.lastLine != 0 || t.YellForHelp != 7 || t.conversations != 7) ok = 0;
    if (!ok) { std::printf("00ea4980_TEST FAIL\n"); return 1; }
    std::printf("00ea4980_TEST PASS\n");
    return 0;
}
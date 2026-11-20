// Behaviour test for 0x00E0C350 (secret-passage quest ender): wait for success/failure, then report and deactivate.
#include <cstdio>
#include <string>
struct Game {
    std::string log; int frames; int succeedAt; int failAt; bool* ok; bool* bad;
    Game() : frames(0), succeedAt(1000), failAt(1000), ok(0), bad(0) {}
    void NewScriptFrame() { log += "F"; ++frames; if (frames == succeedAt) *ok = true; if (frames == failAt) *bad = true; }
    std::string GetActiveQuestName() { return "Q_SecretPassage"; }
    void SetQuestAsCompleted(const std::string& q, bool a, bool b, bool c) { log += (q == "Q_SecretPassage" && !a && !b && !c) ? "C" : "?"; }
    void SetQuestAsFailed(const std::string& q, bool a, const std::string& reason, bool b) { log += (q == "Q_SecretPassage" && !a && reason.empty() && b) ? "X" : "?"; }
    void GiveHeroQuestCardDirectly(const std::string& card, const std::string& q, bool add)
    { log += (card == "OBJECT_QUEST_CARD_PRISON_SECRET_PASSAGE" && q == "Q_SecretPassage" && !add) ? "K" : "?"; }
    void DeactivateQuestLater(const std::string& q, unsigned long d) { log += (q == "Q_SecretPassage" && d == 0) ? "D" : "?"; }
};
struct Script {
    Game* GSI; bool Succeeded; bool Failed; int termAfter; int checks;
    Script(Game* g, int after) : GSI(g), Succeeded(false), Failed(false), termAfter(after), checks(0) { g->ok = &Succeeded; g->bad = &Failed; }
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void Run()
    {
        while (!Failed && !Succeeded) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (Succeeded) {
            if (IsActiveThreadTerminating()) return;
            GSI->SetQuestAsCompleted(GSI->GetActiveQuestName(), false, false, false);
            GSI->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_PRISON_SECRET_PASSAGE", "Q_SecretPassage", false);
        } else {
            if (IsActiveThreadTerminating()) return;
            GSI->SetQuestAsFailed(GSI->GetActiveQuestName(), false, "", true);
        }
        GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; g.succeedAt = 2; Script s(&g, 9); s.Run(); CHECK(g.log == "FFCKD"); }
    { Game g; g.failAt = 1; Script s(&g, 9); s.Run(); CHECK(g.log == "FXD"); }
    { Game g; Script s(&g, 2); s.Run(); CHECK(g.log == "FFF"); }
    { Game g; g.succeedAt = 1; Script s(&g, 2); s.Run(); CHECK(g.log == "F"); }
    if (g_fail) { std::printf("00e0c350_TEST FAIL\n"); return 1; }
    std::printf("00e0c350_TEST PASS\n");
    return 0;
}
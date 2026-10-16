#include <cstdio>
#include <cstring>
// Behaviour model of CV_TrophyDealerScript::Init: the active quest's card objective becomes the trophy-dealer text in
// Witchwood2 with an empty extra string.
struct Str { const char* p; Str(const char* t) : p(t) {} };
struct Game {
    const char* q; const char* t; const char* r; const char* e;
    Str GetActiveQuestName() const { return Str("Q_Trophy"); }
    void SetQuestCardObjective(const Str& a, const Str& b, const Str& c, const Str& d) { q = a.p; t = b.p; r = c.p; e = d.p; }
};
struct Script {
    Game* G;
    void Init()
    {
        Str extra("");
        Str region("Witchwood2");
        Str text("TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_");
        G->SetQuestCardObjective(G->GetActiveQuestName(), text, region, extra);
    }
};
int main()
{
    Game g; Script s; s.G = &g; s.Init();
    if (std::strcmp(g.q, "Q_Trophy") || std::strcmp(g.r, "Witchwood2") || g.e[0] || std::strncmp(g.t, "TEXT_QUEST_FIND", 15)) {
        std::printf("00ee6d60_TEST FAIL\n"); return 1;
    }
    std::printf("00ee6d60_TEST PASS\n");
    return 0;
}
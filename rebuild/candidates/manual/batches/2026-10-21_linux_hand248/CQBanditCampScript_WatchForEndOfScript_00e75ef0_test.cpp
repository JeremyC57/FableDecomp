// Behaviour test for 00e75ef0 (CQ_BanditCampScript::WatchForEndOfScript): while Q_TraderConflictEvil is active,
// V_ExposeMayorRhodri is deactivated (delay 0) and re-activated once Q_TraderConflictEvil ends; frames pass until the
// thread terminates.
#include <cstdio>
#include <cstring>

struct Game { int frames, termAt, deactivations, activations; bool conflict[64]; unsigned long lastDelay; char last[32]; };
struct Script {
    Game* g;
    bool Term() { return g->frames >= g->termAt; }
    bool IsQuestActive(const char* q) { return std::strcmp(q, "Q_TraderConflictEvil") == 0 && g->conflict[g->frames]; }
    void Run()
    {
        while (!Term()) {
            if (IsQuestActive("Q_TraderConflictEvil")) {
                if (Term()) return;
                ++g->deactivations; g->lastDelay = 0; std::strcpy(g->last, "V_ExposeMayorRhodri");
                while (!(!IsQuestActive("Q_TraderConflictEvil"))) { ++g->frames; if (Term()) return; }
                if (Term()) return;
                ++g->activations;
            }
            ++g->frames;
        }
    }
};
int main()
{
    Game g; std::memset(&g, 0, sizeof g); g.termAt = 30;
    for (int i = 3; i < 8; ++i) g.conflict[i] = true;
    for (int i = 15; i < 17; ++i) g.conflict[i] = true;
    Script s; s.g = &g; s.Run();
    int ok = g.deactivations == 2 && g.activations == 2 && std::strcmp(g.last, "V_ExposeMayorRhodri") == 0 && g.frames == 30;
    if (!ok) { std::printf("00e75ef0_TEST FAIL\n"); return 1; }
    std::printf("00e75ef0_TEST PASS\n");
    return 0;
}

#include <cstdio>
// behaviour test: quitting a non-follower does nothing; quitting an AI follower drops it from both lists, notifies the follow handler,
// posts deed 0x31 and clears the emote icon; persistence is dropped only when neither home nor work building is alive
class CThing { public: bool Dead; };
struct CCreature { CThing* Home; CThing* Work; bool Persisted; bool LevelPersistent; };
static bool IsAlive(const CThing* t) { return t && !t->Dead; }
class CTCFollowed { public:
    CCreature* followers[4]; int nFollowers; CCreature* ai[4]; int nAI; int handled, deeds, icons;
    static void Remove(CCreature** v, int& n, CCreature* c) { int o = 0; for (int i = 0; i < n; ++i) if (v[i] != c) v[o++] = v[i]; n = o; }
    bool IsAIFollower(CCreature* c) { for (int i = 0; i < nAI; ++i) if (ai[i] == c) return true; return false; }
    void QuitAIFollower(CCreature* creature)
    {
        if (IsAIFollower(creature)) {
            Remove(ai, nAI, creature);
            Remove(followers, nFollowers, creature);
            ++handled; ++deeds; ++icons;
            if (!IsAlive(creature->Home) && !IsAlive(creature->Work)) {
                creature->Persisted = false;
                creature->LevelPersistent = false;
            }
        }
    }
};
int main()
{
    CThing house = { false }, ruin = { true };
    CCreature a = { &ruin, 0, true, true }, b = { 0, &house, true, true }, c = { 0, 0, true, true };
    CTCFollowed f = { { &a, &b }, 2, { &a, &b }, 2, 0, 0, 0 };
    f.QuitAIFollower(&c);
    bool ok = f.nAI == 2 && f.handled == 0;
    f.QuitAIFollower(&a);
    ok = ok && f.nAI == 1 && f.nFollowers == 1 && f.ai[0] == &b && f.handled == 1 && f.deeds == 1 && !a.Persisted && !a.LevelPersistent;
    f.QuitAIFollower(&b);
    ok = ok && f.nAI == 0 && f.nFollowers == 0 && b.Persisted && b.LevelPersistent;
    if (!ok) { std::printf("006d56e0_TEST FAIL\n"); return 1; }
    std::printf("006d56e0_TEST PASS\n");
    return 0;
}
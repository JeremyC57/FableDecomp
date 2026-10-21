// Behaviour test for CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable (0x00986510): same logic over mocks.
#include <cstdio>
struct CTCVillage { int id; };
struct CTCVillager { CTCVillage* village; };
CTCVillage* GFGetPVillageInterface(CTCVillager* v) { return v->village; }
struct CThing;
struct CHeroMarriageInfo { long a, b, c, d; long State; bool Flag; CHeroMarriageInfo() : a(0), b(0), c(0), d(0), State(0), Flag(false) { } };
struct CTCHeroStats {
    CThing* wives[4]; int nWives; long states[4]; bool known[4];
    bool empty() const { return nWives == 0; }
    bool PeekMarriageInfo(int idx, CHeroMarriageInfo* info) const { if (!known[idx]) return false; info->State = states[idx]; return true; }
};
struct CThing {
    bool dead; CTCVillager* villager; CTCHeroStats* stats; int uid;
    CThing() : dead(false), villager(0), stats(0), uid(0) { }
};
struct CState {
    CThing* creature; CThing* reactingTo; bool comment;
    bool MarriageCommentIsRunnable()
    {
        if (creature->villager) {
            CTCVillage* village = GFGetPVillageInterface(creature->villager);
            if (village != 0 && comment) {
                CTCHeroStats* stats = reactingTo->stats;
                if (stats && !stats->empty()) {
                    for (int i = 0; i < stats->nWives; ++i) {
                        CThing* w = stats->wives[i];
                        if (w != 0 && !w->dead && w->villager && GFGetPVillageInterface(w->villager) == village) {
                            CHeroMarriageInfo info;
                            if (stats->PeekMarriageInfo(w->uid, &info)) {
                                bool married = false;
                                if (info.State == 1 || info.State == 3 || info.State == 2 || info.State == 4 || info.State == 5)
                                    married = true;
                                return married;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
};
int main()
{
    CTCVillage v1, v2; CTCVillager cv = { &v1 }, wv1 = { &v2 }, wv2 = { &v1 };
    CThing creature, hero, w1, w2; creature.villager = &cv;
    CTCHeroStats stats; stats.nWives = 2; stats.wives[0] = &w1; stats.wives[1] = &w2;
    w1.villager = &wv1; w1.uid = 0; w2.villager = &wv2; w2.uid = 1;
    stats.known[0] = true; stats.known[1] = true; stats.states[0] = 3; stats.states[1] = 4;
    hero.stats = &stats;
    CState s; s.creature = &creature; s.reactingTo = &hero; s.comment = true;
    bool ok = s.MarriageCommentIsRunnable();             // w2 same village, state 4
    stats.states[1] = 6; ok = ok && !s.MarriageCommentIsRunnable();
    stats.states[1] = 1; w2.dead = true; ok = ok && !s.MarriageCommentIsRunnable();
    w2.dead = false; s.comment = false; ok = ok && !s.MarriageCommentIsRunnable();
    s.comment = true; stats.nWives = 0; ok = ok && !s.MarriageCommentIsRunnable();
    if (!ok) { std::printf("00986510_TEST FAIL\n"); return 1; }
    std::printf("00986510_TEST PASS\n");
    return 0;
}
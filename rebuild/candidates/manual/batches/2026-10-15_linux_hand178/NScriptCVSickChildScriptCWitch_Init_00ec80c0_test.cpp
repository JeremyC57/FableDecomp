#include <cstdio>
struct Thing { int id; };
struct Game {
    char log[16]; int n;
    void RemoveThing(const Thing&, bool a, bool b) { if (!a && b) log[n++] = 'R'; }
    void SetIsPushableByHero(Thing, bool p) { if (!p) log[n++] = 'P'; }
    void SetThingHasInformation(const Thing&, bool a, bool b, bool c) { if (!a && b && !c) log[n++] = 'I'; }
    void EntitySetAsKillable(const Thing&, bool a, bool b) { if (!a && !b) log[n++] = 'K'; }
    void EntitySetAsToAddToComboMultiplierWhenHit(const Thing&, bool a) { if (!a) log[n++] = 'C'; }
};
struct Parent { bool flag; };
struct Witch {
    Game* G; Thing Me; Parent* ParentClass; bool IntroDone; int ThingsForPotionGot;
    void Init()
    {
        IntroDone = false; ThingsForPotionGot = 0;
        if (!ParentClass->flag) { G->RemoveThing(Me, false, true); return; }
        G->SetIsPushableByHero(Me, false); G->SetThingHasInformation(Me, false, true, false);
        G->EntitySetAsKillable(Me, false, false); G->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    }
};
int main()
{
    Game g; g.n = 0; Parent p; p.flag = false;
    Witch w; w.G = &g; w.Me.id = 1; w.ParentClass = &p; w.IntroDone = true; w.ThingsForPotionGot = 5;
    w.Init();
    if (g.n != 1 || g.log[0] != 'R' || w.IntroDone || w.ThingsForPotionGot) { std::printf("00ec80c0_TEST FAIL\n"); return 1; }
    p.flag = true; g.n = 0; w.Init();
    if (g.n != 4 || g.log[0] != 'P' || g.log[1] != 'I' || g.log[2] != 'K' || g.log[3] != 'C') { std::printf("00ec80c0_TEST FAIL\n"); return 1; }
    std::printf("00ec80c0_TEST PASS\n");
    return 0;
}

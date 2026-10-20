#include <cstdio>
#include <cstring>
struct Info { long RefCount; int deleted; };
struct Ptr {
    void* Data; Info* I;
    Ptr() : Data(0), I(0) {}
    Ptr& operator=(const Ptr& o) { void* d = o.Data; Info* i = o.I; if (I != i) { if (I) { if (--I->RefCount == 0) I->deleted++; } Data = d; I = i; if (i) ++i->RefCount; } return *this; }
};
struct Game {
    char faction[32]; char marker[32]; char narrator[32]; int damageable, respond, pushable;
    void EntitySetInFaction(const char* f) { std::strcpy(faction, f); }
    void EntitySetAsDamageable(bool b) { damageable = b; }
    void EntitySetAsRespondToHit(bool b) { respond = b; }
    void SetIsThingForcePushable(bool b) { pushable = b; }
    void MiniMapAddMarker(const char* m) { std::strcpy(marker, m); }
    void EntitySetWillBeUsingNarrator(const char* n) { std::strcpy(narrator, n); }
};
struct Keeper {
    Game* G; bool DoneIntro; Ptr ImprisonedFX;
    void Init()
    {
        DoneIntro = false;
        G->EntitySetInFaction("FACTION_MAP_KEEPER");
        ImprisonedFX = Ptr();
        G->EntitySetAsDamageable(false); G->EntitySetAsRespondToHit(false); G->SetIsThingForcePushable(false);
        G->MiniMapAddMarker("HUD_ORB_QUEST_CORE"); G->EntitySetWillBeUsingNarrator("ARCHAEOLOGIST");
    }
};
int main()
{
    Game g; g.damageable = g.respond = g.pushable = 1;
    Info inf = { 1, 0 }; int fx = 0;
    Keeper k; k.G = &g; k.DoneIntro = true; k.ImprisonedFX.Data = &fx; k.ImprisonedFX.I = &inf;
    k.Init();
    bool ok = !k.DoneIntro && k.ImprisonedFX.Data == 0 && k.ImprisonedFX.I == 0 && inf.deleted == 1
        && !std::strcmp(g.faction, "FACTION_MAP_KEEPER") && !std::strcmp(g.narrator, "ARCHAEOLOGIST") && !g.damageable && !g.respond && !g.pushable;
    if (!ok) { std::printf("00da4350_TEST FAIL\n"); return 1; }
    std::printf("00da4350_TEST PASS\n");
    return 0;
}
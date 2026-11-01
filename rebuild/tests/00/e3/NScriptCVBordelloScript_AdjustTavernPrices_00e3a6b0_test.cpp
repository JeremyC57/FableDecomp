// Test for CV_BordelloScript::AdjustTavernPrices() -- per Bordello visit: wait for the region, set the barmaid/barman opinion masks,
// apply the hero price multiplier once the hero owns the bordello, wait for the region to unload, reset prices to 1.0 and the beer count.
#include <cstdio>
class CGame { public:
    int frame; int loadFrom, loadTo, ownFrom, stopAt; int masks; float prices[8]; int nPrices;
    bool IsRegionLoaded() const { return frame >= loadFrom && frame < loadTo; }
    void NewScriptFrame() { ++frame; }
    void EntitySetOpinionReactionMask() { ++masks; }
    void SetTradingPriceMult(float m) { prices[nPrices++] = m; }
};
struct CV_BordelloScript {
    CGame* Game; bool PlayerOwned; long BeersDrunk;
    bool IsActiveThreadTerminating() const { return Game->frame >= Game->stopAt; }
    void AdjustTavernPrices()
    {
        while (!IsActiveThreadTerminating()) {
            while (!Game->IsRegionLoaded()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetOpinionReactionMask();
            Game->EntitySetOpinionReactionMask();
            while (Game->IsRegionLoaded()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                PlayerOwned = Game->frame >= Game->ownFrom;
                if (PlayerOwned) { if (IsActiveThreadTerminating()) return; Game->SetTradingPriceMult(1.5f); break; }
            }
            if (IsActiveThreadTerminating()) return;
            while (!(!Game->IsRegionLoaded())) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Game->SetTradingPriceMult(1.0f);
            BeersDrunk = 0;
            Game->NewScriptFrame();
            Game->loadFrom = 1000;
        }
    }
};
int main()
{
    CGame g = { 0, 3, 10, 5, 20, 0, {0}, 0 };
    CV_BordelloScript s = { &g, false, 7 };
    s.AdjustTavernPrices();
    if (g.masks != 2 || g.nPrices != 2 || g.prices[0] != 1.5f || g.prices[1] != 1.0f || s.BeersDrunk != 0)
    { std::printf("00e3a6b0_TEST FAIL\n"); return 1; }
    std::printf("00e3a6b0_TEST PASS\n");
    return 0;
}
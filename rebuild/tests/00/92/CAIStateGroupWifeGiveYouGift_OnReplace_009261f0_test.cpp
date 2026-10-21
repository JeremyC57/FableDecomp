// Behaviour test for CAIStateGroup_WifeGiveYouGift::OnReplace (0x009261F0): same control flow over mocks.
#include <cstdio>
#include <cstring>
struct CHero { bool exists; bool dead; int inventoryItems; int playerNumber; };
struct CWorldMock {
    int created; int createdDef; int createdPlayer; char subtitle[96]; unsigned long colour; int wifeGiftState; bool wifeHasComponent;
};
struct CGift {
    int def; bool delivered;
    void OnReplace(CHero& hero, CWorldMock& w)
    {
        if (!delivered)
            return;
        if (!hero.exists || hero.dead)
            return;
        ++w.created; w.createdDef = def; w.createdPlayer = hero.playerNumber;
        ++hero.inventoryItems;
        std::strcpy(w.subtitle, "TEXT_GUI_WIFE_GIVEN_YOU_GIFT");
        std::strcat(w.subtitle, ":Ring");
        w.colour = 0x122;
        if (w.wifeHasComponent)
            w.wifeGiftState = 0;
    }
};
static CWorldMock Fresh()
{
    CWorldMock w; std::memset(&w, 0, sizeof(w)); w.wifeGiftState = 3; w.wifeHasComponent = true; return w;
}
int main()
{
    bool ok = true;
    CGift g; g.def = 77; g.delivered = true;
    CHero h = { true, false, 0, 1 };
    CWorldMock w = Fresh();
    g.OnReplace(h, w);
    ok = ok && w.created == 1 && w.createdDef == 77 && w.createdPlayer == 1 && h.inventoryItems == 1
        && std::strcmp(w.subtitle, "TEXT_GUI_WIFE_GIVEN_YOU_GIFT:Ring") == 0 && w.colour == 0x122 && w.wifeGiftState == 0;
    w = Fresh(); h.dead = true; g.OnReplace(h, w);
    ok = ok && w.created == 0 && w.wifeGiftState == 3;
    w = Fresh(); h.dead = false; h.exists = false; g.OnReplace(h, w);
    ok = ok && w.created == 0;
    w = Fresh(); h.exists = true; g.delivered = false; g.OnReplace(h, w);
    ok = ok && w.created == 0 && h.inventoryItems == 1;
    w = Fresh(); w.wifeHasComponent = false; g.delivered = true; g.OnReplace(h, w);
    ok = ok && w.created == 1 && w.wifeGiftState == 3 && h.inventoryItems == 2;
    if (!ok) { std::printf("009261f0_TEST FAIL\n"); return 1; }
    std::printf("009261f0_TEST PASS\n");
    return 0;
}
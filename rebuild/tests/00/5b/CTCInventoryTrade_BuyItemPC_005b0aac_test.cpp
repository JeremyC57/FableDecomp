// Behaviour test for CTCInventoryTrade::BuyItemPC @ 0x005B0AAC: stock bookkeeping, payment by purchase kind, the normal-item path
// (shop sells `count`, the hero pays, the inventory gets the items at the unit price, the sell totals grow), and the special-item
// re-enable pass.
#include <cstdio>
#include <vector>
enum Kind { TATTOO, HAIR, TITLE, NORMAL };
struct Buy { long def, stock; bool special; bool available; bool owned; long price; };
struct State { long money; long sold; long invCount; long invUnitPrice; long totClothes, totWeapons, totItems; int applied; bool warning; };
static void BuyItemPC(std::vector<Buy>& buys, unsigned int which, long count, Kind kind, int menuType, bool weaponUsable,
                      bool buyable, bool room, State& st)
{
    Buy& rec = buys[which];
    if (!rec.special)
        rec.stock -= count;
    if (rec.stock == 0 || (!room && !rec.special))
        rec.available = false;
    long price = rec.price * count;
    if (kind != NORMAL) {
        st.money -= price;
        st.applied = kind;
    } else {
        for (long i = 0; i < count; ++i)
            ++st.sold;
        st.money -= price;
        if (!weaponUsable)
            st.warning = true;
        st.invCount += count;
        st.invUnitPrice = price / count;
        if (buyable) {
            switch (menuType) {
            case 0: st.totClothes += count; break;
            case 1: st.totWeapons += count; break;
            case 2: st.totItems += count; break;
            }
        }
    }
    if (rec.special) {
        for (unsigned int i = 0; i < buys.size(); ++i)
            if (buys[i].stock > 0 && buys[i].price <= st.money && !buys[i].owned)
                buys[i].available = true;
    }
}
int main()
{
    int ok = 1;
    State st = { 1000, 0, 0, 0, 0, 0, 0, -1, false };
    Buy a = { 10, 5, false, true, false, 20 };
    Buy sp1 = { 11, 1, true, true, false, 100 };
    Buy sp2 = { 12, 1, true, false, false, 300 };
    Buy sp3 = { 13, 1, true, false, false, 5000 };
    std::vector<Buy> v; v.push_back(a); v.push_back(sp1); v.push_back(sp2); v.push_back(sp3);
    BuyItemPC(v, 0, 3, NORMAL, 1, false, true, true, st);
    if (v[0].stock != 2 || !v[0].available || st.money != 940 || st.sold != 3 || st.invCount != 3 || st.invUnitPrice != 20) ok = 0;
    if (st.totWeapons != 3 || st.totClothes != 0 || !st.warning) ok = 0;
    BuyItemPC(v, 0, 2, NORMAL, 0, true, true, true, st);
    if (v[0].stock != 0 || v[0].available || st.totClothes != 2) ok = 0;   // sold out: unavailable
    BuyItemPC(v, 1, 1, TATTOO, 0, true, false, false, st);
    if (v[1].stock != 1 || st.money != 800 || st.applied != TATTOO) ok = 0; // specials never run out
    if (!v[2].available || v[3].available) ok = 0;                          // affordable special back, the dear one not
    if (!ok) { std::printf("005b0aac_TEST FAIL\n"); return 1; }
    std::printf("005b0aac_TEST PASS\n");
    return 0;
}
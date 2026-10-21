#include <cstdio>
// behaviour test: CanBeRecreated -- false unless respawnable; false when the village is flagged; a home house not available
// for residency blocks; a home or work house the player owns or that starts empty (and is not rented) blocks and pushes the
// recreation day up to today (never down)
struct House { bool Present, Available, Owned, InitiallyEmpty, Rented; };
struct Dummy {
    bool Respawnable, VillageFlag; House Home, Work; long Day;
    static bool Blocks(const House& h) { return (h.Owned || h.InitiallyEmpty) && !h.Rented; }
    bool CanBeRecreated(long today)
    {
        if (!Respawnable) return false;
        if (VillageFlag) return false;
        if (Home.Present) {
            if (!Home.Available) return false;
            if (Blocks(Home)) { Day = Day < today ? today : Day; return false; }
        }
        if (Work.Present && Blocks(Work)) { Day = Day < today ? today : Day; return false; }
        return true;
    }
};
int main()
{
    House none = { false, false, false, false, false };
    House free_ = { true, true, false, false, false };
    House owned = { true, true, true, false, false };
    House rented = { true, true, true, false, true };
    Dummy d = { true, false, none, none, 3 };
    bool ok = d.CanBeRecreated(10);
    d.Respawnable = false; ok = ok && !d.CanBeRecreated(10); d.Respawnable = true;
    d.VillageFlag = true; ok = ok && !d.CanBeRecreated(10); d.VillageFlag = false;
    d.Home = free_; ok = ok && d.CanBeRecreated(10);
    d.Home = owned; ok = ok && !d.CanBeRecreated(10) && d.Day == 10;
    d.Day = 20; ok = ok && !d.CanBeRecreated(10) && d.Day == 20;
    d.Home = rented; ok = ok && d.CanBeRecreated(10);
    d.Work = owned; d.Day = 1; ok = ok && !d.CanBeRecreated(7) && d.Day == 7;
    d.Home.Available = false; d.Work = none; ok = ok && !d.CanBeRecreated(7);
    if (!ok) { std::printf("0072f9e0_TEST FAIL\n"); return 1; }
    std::printf("0072f9e0_TEST PASS\n");
    return 0;
}
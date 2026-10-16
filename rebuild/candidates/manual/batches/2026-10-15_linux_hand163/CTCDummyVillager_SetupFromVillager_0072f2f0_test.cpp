// behaviour test for 0x0072F2F0 CTCDummyVillager::SetupFromVillager
#include <cstdio>
struct Building { int id; };
struct Villager { unsigned short DefIndex; Building* home; Building* work; long role; long GetVillagerRole() const { return role; } };
struct Dummy {
    Villager* Creature; unsigned int DefType; Building* Home; Building* Work; bool Respawnable; bool Guard;
    void SetupFromData(unsigned int t, Building* h, Building* w, bool r) { DefType = t; Home = h; Work = w; Respawnable = r; }
    void SetupFromVillager(Villager& v, bool r)
    {
        Creature = &v;
        Building* home = v.home;
        unsigned int defType = v.DefIndex;
        SetupFromData(defType, home, v.work, r);
        if (v.GetVillagerRole() == 3) Guard = true;
    }
};
int main()
{
    Building h = { 1 }, w = { 2 };
    Villager v = { 42, &h, &w, 3 };
    Dummy d = { 0, 0, 0, 0, false, false };
    d.SetupFromVillager(v, true);
    bool ok = d.Creature == &v && d.DefType == 42 && d.Home == &h && d.Work == &w && d.Respawnable && d.Guard;
    Dummy e = { 0, 0, 0, 0, true, false };
    v.role = 1;
    e.SetupFromVillager(v, false);
    ok = ok && !e.Guard && !e.Respawnable;
    if (!ok) { std::printf("0072f2f0_TEST FAIL\n"); return 1; }
    std::printf("0072f2f0_TEST PASS\n");
    return 0;
}

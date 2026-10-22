#include <cstdio>

struct CRegionDef { bool PreventHeroWeaponUse; bool PreventHeroWillUse; };
struct CRegion { CRegionDef* def; };
struct CTCHero {
    bool WeaponsUsable, WillUsable; int setCalls;
    CTCHero() : WeaponsUsable(false), WillUsable(false), setCalls(0) {}
    void SetWeaponsAsUsable(bool b) { ++setCalls; WeaponsUsable = b; }
    void OnCreate(CRegion& region)
    {
        if (region.def) {
            SetWeaponsAsUsable(!region.def->PreventHeroWeaponUse);
            WillUsable = !region.def->PreventHeroWillUse;
        } else {
            WeaponsUsable = true;
            WillUsable = true;
        }
    }
};
static bool Run()
{
    CRegion none; none.def = 0;
    CTCHero a; a.OnCreate(none);
    bool ok = a.WeaponsUsable && a.WillUsable && a.setCalls == 0;
    CRegionDef d; d.PreventHeroWeaponUse = true; d.PreventHeroWillUse = false; CRegion r; r.def = &d;
    CTCHero b; b.WeaponsUsable = true; b.OnCreate(r);
    ok = ok && !b.WeaponsUsable && b.WillUsable && b.setCalls == 1;
    d.PreventHeroWeaponUse = false; d.PreventHeroWillUse = true; CTCHero c; c.OnCreate(r);
    return ok && c.WeaponsUsable && !c.WillUsable;
}

int main()
{
    if (!Run()) { std::printf("006ca5a0_TEST FAIL\n"); return 1; }
    std::printf("006ca5a0_TEST PASS\n");
    return 0;
}
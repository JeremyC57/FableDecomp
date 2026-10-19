// behaviour copy of CTCCreatureModeManager::CModeBase::OnActivate: the weapon filter follows the primary-slot weapon's group, else "STAND"
#include <cstdio>
#include <cstring>
struct CCharString { const char* p; CCharString(const char* t = 0) : p(t) {} };
static int g_outOfLine = 0;
struct CAnimFilterOfGroup {
    CCharString Group; unsigned long GroupCRC;
    void SetGroup(const CCharString& g) { ++g_outOfLine; Group = g; GroupCRC = (unsigned long)std::strlen(g.p); }
    void SetGroupInline(const CCharString& g) { Group = g; GroupCRC = (unsigned long)std::strlen(g.p); }
};
struct CTCWeapon { CCharString Name; const CCharString& GetAnimationGroupName() const { return Name; } };
struct CThing;
struct CTCCarrying { CThing* item; CThing* GetThingInPrimarySlot() { return item; } };
struct CThing {
    bool hasCarrying, hasWeapon, dead; CTCCarrying* carrying; CTCWeapon* weapon;
    bool IsDead() const { return dead; }
    bool GetInterface(CTCCarrying** out, long type) { if (type == 0x46 && hasCarrying) { *out = carrying; return true; } return false; }
    bool GetInterface(const CTCWeapon** out, long type) const { if (type == 0x48 && hasWeapon) { *out = weapon; return true; } return false; }
};
struct CModeBase {
    CThing* ParentThing; CAnimFilterOfGroup WeaponBasedFilter; bool Active;
    void OnActivate()
    {
        Active = true;
        CTCCarrying* carrying;
        if (ParentThing->GetInterface(&carrying, 0x46)) {
            const CThing* item = carrying->GetThingInPrimarySlot();
            if (item && !item->IsDead()) {
                const CTCWeapon* weapon;
                if (item->GetInterface(&weapon, 0x48)) {
                    WeaponBasedFilter.SetGroup(weapon->GetAnimationGroupName());
                    return;
                }
            }
        }
        WeaponBasedFilter.SetGroupInline("STAND");
    }
};
int main()
{
    CTCWeapon w; w.Name = CCharString("SWORD");
    CThing sword = { false, true, false, 0, &w };
    CTCCarrying carry; carry.item = &sword;
    CThing hero = { true, false, false, &carry, 0 };
    CModeBase m; m.ParentThing = &hero; m.Active = false;
    m.OnActivate();
    if (!m.Active || std::strcmp(m.WeaponBasedFilter.Group.p, "SWORD") != 0 || g_outOfLine != 1) { std::printf("0073aac0_TEST FAIL\n"); return 1; }
    sword.dead = true; m.OnActivate();
    if (std::strcmp(m.WeaponBasedFilter.Group.p, "STAND") != 0 || m.WeaponBasedFilter.GroupCRC != 5 || g_outOfLine != 1) { std::printf("0073aac0_TEST FAIL\n"); return 1; }
    hero.hasCarrying = false; sword.dead = false; m.WeaponBasedFilter.Group = CCharString("X"); m.OnActivate();
    if (std::strcmp(m.WeaponBasedFilter.Group.p, "STAND") != 0) { std::printf("0073aac0_TEST FAIL\n"); return 1; }
    std::printf("0073aac0_TEST PASS\n");
    return 0;
}

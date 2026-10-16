#include <cstdio>
// Behaviour test for CCombatManager::SetAttackerAsReadyToAttack @ 0x006EA450 (decision logic).
struct Weapon { bool projectile; int readied; };
struct Item { bool dead; bool hasWeapon; Weapon* weapon; };
struct Attacker { bool attacking; bool hasCarrying; Item* primary; };
static int g_resets = 0;
static void Ready(Attacker& a, bool keepSequence)
{
    if (!a.attacking) return;
    if (a.hasCarrying) {
        Item* item = a.primary;
        if (item && !item->dead && item->hasWeapon && item->weapon->projectile) ++item->weapon->readied;
    }
    if (!keepSequence) ++g_resets;
}
static int Fail() { std::printf("006ea450_TEST FAIL\n"); return 1; }
int main()
{
    Weapon bow = {true, 0}, sword = {false, 0};
    Item it = {false, true, &bow};
    Attacker a = {true, true, &it};
    Ready(a, false); if (bow.readied != 1 || g_resets != 1) return Fail();
    it.dead = true; Ready(a, true); if (bow.readied != 1 || g_resets != 1) return Fail();
    it.dead = false; it.weapon = &sword; Ready(a, true); if (sword.readied != 0) return Fail();
    a.primary = 0; Ready(a, false); if (g_resets != 2) return Fail();
    a.attacking = false; Ready(a, false); if (g_resets != 2) return Fail();
    std::printf("006ea450_TEST PASS\n");
    return 0;
}
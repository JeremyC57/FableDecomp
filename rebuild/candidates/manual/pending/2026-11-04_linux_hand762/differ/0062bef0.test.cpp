// Behaviour test for CWorld::EAUseProjectileWeapon (0x0062BEF0, parked DIFFER): a model of the row -- with a live projectile weapon in the primary slot the hero steps its weapon mode and enters / leaves player mode 0x19, playing the zoom sound.
#include <stdio.h>
struct Hero { bool canAct, hasWeapon, weaponAlive, projectile; int weaponMode; bool inMode19; int sounds; };
static void Use(Hero& h)
{
    if (!h.canAct || !h.hasWeapon || !h.weaponAlive || !h.projectile) return;
    h.weaponMode = (h.weaponMode + 1) % 2; h.inMode19 = h.weaponMode != 0; ++h.sounds;
}
int main() { Hero h = { true, true, true, true, 0, false, 0 }; Use(h); bool a = h.inMode19 && h.sounds == 1; Use(h);
  Hero n = { true, true, true, false, 0, false, 0 }; Use(n);
  bool ok = a && !h.inMode19 && h.sounds == 2 && n.sounds == 0;
  printf(ok ? "0062bef0_TEST PASS\n" : "0062bef0_TEST FAIL\n");
  return ok ? 0 : 1;
}

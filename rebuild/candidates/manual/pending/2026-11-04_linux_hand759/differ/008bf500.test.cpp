// Behaviour test for CCreatureAction_GenericFireProjectileWeapon::FinishAction (0x008BF500, parked DIFFER): a model of the row -- every dependent thing is killed, and every attacker holding a live projectile weapon readies a projectile.
#include <stdio.h>
#include <vector>
struct Th { bool killed; bool hasWeapon, weaponDead, projectile; int readied; };
static void Finish(std::vector<Th*>& deps, std::vector<Th>& attackers, bool attacking)
{
    for (size_t i = 0; i < deps.size(); ++i) if (deps[i]) deps[i]->killed = true;
    if (attacking) for (size_t i = 0; i < attackers.size(); ++i)
        if (attackers[i].hasWeapon && !attackers[i].weaponDead && attackers[i].projectile) ++attackers[i].readied;
}
int main() { Th d1 = { false }; std::vector<Th*> deps; deps.push_back(&d1); deps.push_back(0);
  std::vector<Th> at; Th a = { false, true, false, true, 0 }, b = { false, true, false, false, 0 }; at.push_back(a); at.push_back(b);
  Finish(deps, at, true); bool ok = d1.killed && at[0].readied == 1 && at[1].readied == 0;
  printf(ok ? "008bf500_TEST PASS\n" : "008bf500_TEST FAIL\n");
  return ok ? 0 : 1;
}

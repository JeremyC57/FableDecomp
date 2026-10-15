// Behaviour test for 006ebee0 CCombatRing::AssignAttackerToProng: the ring index is recorded, the prong
// takes the attacker, the thickness only grows (1.5 x diameter on inner rings, the combat-type distance
// on ring 0), and the weapon type is 5 for the outer ring, 4 otherwise.
#include <cstdio>

struct CCreature;
struct CManager { CCreature* who; int type; void CreatureUnsheathWeapon(CCreature* c, int t) { who = c; type = t; } };
static CManager g_mgr;
struct CTypeDef { float dist; };
struct CCreature { float radius; CTypeDef def; CTypeDef* GetCombatTypeDef() { return &def; } CManager* GetCombatManager() { return &g_mgr; } };
struct CProng { CCreature* occupier; void SetPOccupyingAttacker(CCreature* c) { occupier = c; } };
struct CInfo { long ring; };
struct CRing {
    long Index; float RingThickness;
    void Assign(CProng* prong, CCreature* a, CInfo* info, bool outer)
    {
        info->ring = Index;
        prong->SetPOccupyingAttacker(a);
        if (Index != 0) { float s = a->radius + a->radius; if (s > RingThickness) RingThickness = s * 1.5f; }
        else { float s = a->GetCombatTypeDef()->dist; if (s > RingThickness) RingThickness = s; }
        if (outer) a->GetCombatManager()->CreatureUnsheathWeapon(a, 5);
        else a->GetCombatManager()->CreatureUnsheathWeapon(a, 4);
    }
};

int main()
{
    CCreature c; c.radius = 2.0f; c.def.dist = 3.0f;
    CProng p; p.occupier = 0; CInfo info; info.ring = -1;
    CRing r; r.Index = 2; r.RingThickness = 1.0f;
    r.Assign(&p, &c, &info, true);
    bool ok = info.ring == 2 && p.occupier == &c && r.RingThickness == 6.0f && g_mgr.type == 5 && g_mgr.who == &c;
    r.RingThickness = 10.0f; r.Assign(&p, &c, &info, false);
    ok = ok && r.RingThickness == 10.0f && g_mgr.type == 4;
    r.Index = 0; r.RingThickness = 1.0f; r.Assign(&p, &c, &info, false);
    ok = ok && r.RingThickness == 3.0f && info.ring == 0;
    if (!ok) { std::printf("006ebee0_TEST FAIL\n"); return 1; }
    std::printf("006ebee0_TEST PASS\n");
    return 0;
}
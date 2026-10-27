// Behaviour test for CTCCombat::OnInitialActivate (0x00736AD0): only a type-2 parent thing updates its usable melee abilities.
#include <cstdio>
class CThing { public: unsigned int Type; unsigned int GetType() const { return Type; } };
class CTCCombat { public:
    CThing* ParentThing;
    int Updates;
    void UpdateUsableMeleeCombatAbilities() { ++Updates; }
    void OnInitialActivate() { if (ParentThing->GetType() == 2) UpdateUsableMeleeCombatAbilities(); }
};
int main()
{
    CThing t; CTCCombat c; c.ParentThing = &t; c.Updates = 0;
    t.Type = 1; c.OnInitialActivate();
    t.Type = 2; c.OnInitialActivate();
    t.Type = 3; c.OnInitialActivate();
    if (c.Updates != 1) { std::printf("00736ad0_TEST FAIL\n"); return 1; }
    std::printf("00736ad0_TEST PASS\n");
    return 0;
}
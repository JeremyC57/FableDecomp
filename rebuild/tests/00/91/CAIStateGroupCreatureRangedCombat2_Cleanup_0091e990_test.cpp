// Behaviour test for 0x0091E990: RangedCombat2::Cleanup.
#include <cstdio>
#include <cstring>
struct CTC { int triggered; void Trigger() { ++triggered; } };
struct CCreature { int cleaned; CTC tc; char lastName[64];
    CTC* AddTC(const char* name, bool, void*) { std::strcpy(lastName, name); return &tc; } };
struct CGroup { bool shuttingDown; CCreature* creature; bool AddedStrafe;
    void Cleanup()
    {
        if (shuttingDown) {
            creature->cleaned++;
        } else {
            CTC* fire = creature->AddTC("CTCFireRangedWeaponOnLeavingStateGroup", false, 0);
            if (!AddedStrafe) fire->Trigger();
        }
        AddedStrafe = false;
    }
};
int main()
{
    CCreature c; std::memset(&c, 0, sizeof c);
    CGroup g = { true, &c, true };
    g.Cleanup();
    int bad = (c.cleaned != 1 || g.AddedStrafe || c.tc.triggered != 0);
    g.shuttingDown = false; g.AddedStrafe = true; g.Cleanup();
    if (c.tc.triggered != 0 || g.AddedStrafe || std::strcmp(c.lastName, "CTCFireRangedWeaponOnLeavingStateGroup") != 0) bad = 2;
    g.Cleanup(); if (c.tc.triggered != 1) bad = 3;
    if (bad) { std::printf("0091e990_TEST FAIL\n"); return 1; }
    std::printf("0091e990_TEST PASS\n");
    return 0;
}
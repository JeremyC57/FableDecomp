#include <cstdio>
struct CTurncoatDef { float MinimumResistance; float PercentageOfHealthAsResistance; float ExtraResistance; };
struct CThingCreatureBase { float Health; };
struct CTCTurncoat {
    CThingCreatureBase* Thing; const CTurncoatDef* PDef_Object; float CreatureTurncoatResistance;
    void OnInitialActivate()
    {
        CreatureTurncoatResistance = PDef_Object->PercentageOfHealthAsResistance * Thing->Health + PDef_Object->ExtraResistance;
        if (CreatureTurncoatResistance < PDef_Object->MinimumResistance)
            CreatureTurncoatResistance = PDef_Object->MinimumResistance;
    }
};
int main()
{
    CTurncoatDef def; def.MinimumResistance = 10.0f; def.PercentageOfHealthAsResistance = 0.5f; def.ExtraResistance = 2.0f;
    CThingCreatureBase c; c.Health = 100.0f;
    CTCTurncoat t; t.Thing = &c; t.PDef_Object = &def; t.CreatureTurncoatResistance = 0.0f;
    t.OnInitialActivate();
    if (t.CreatureTurncoatResistance != 52.0f) { std::printf("007c7840_TEST FAIL\n"); return 1; }
    c.Health = 4.0f; t.OnInitialActivate();           // 4 below the minimum: clamped
    if (t.CreatureTurncoatResistance != 10.0f) { std::printf("007c7840_TEST FAIL\n"); return 1; }
    std::printf("007c7840_TEST PASS\n");
    return 0;
}
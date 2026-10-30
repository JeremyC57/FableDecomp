// Behaviour test for 0x00EC1CD0 NScript::CV_RandomPopulationSimScript::CRandomPopulationCreature::Init: clear
// AttachedToVillage, take the furthest RandomPopulationExit from Me as Exit, classify Me by script name (villager 0 with
// gold = (1 - region population scale) * (rand() % variation + minimum), guard 1, bandit 2, anything else 0) and count the
// type in the parent's CreaturesAlive.
#include <stdio.h>
#include <string.h>

static int g_rand;
static int FakeRand() { return g_rand; }
static long g_gold, g_goldCalls;
static char g_exitQuery[64];
class CScriptDef { public: long RandomPopulationTraderGoldMinimum, RandomPopulationTraderGoldVariation; };
static CScriptDef g_def;
static CScriptDef* pGlobalScriptDef = &g_def;
class CThing { public: const char* Name; int Id; const char* GetName() const { return Name ? Name : ""; } };
class CGame { public:
    CThing GetFurthestWithScriptName(const CThing&, const char* n) { strcpy(g_exitQuery, n); CThing t; t.Name = "Exit"; t.Id = 7; return t; }
    void EntityGiveGold(const CThing&, long gold) { g_gold = gold; ++g_goldCalls; }
};
struct CSim { float CurrentPopulationScale[4]; long CurrentPopulationRegion; long CreaturesAlive[3]; };
struct CCreature {
    CGame* Game; CThing Me; CSim* ParentClass; CThing Exit; long MyType; bool AttachedToVillage;
    void Init()
    {
        AttachedToVillage = false;
        Exit = Game->GetFurthestWithScriptName(Me, "RandomPopulationExit");
        if (!strcmp(Me.GetName(), "RandomPopulationVillager")) {
            MyType = 0;
            CScriptDef* def = pGlobalScriptDef;
            long gold = FakeRand() % def->RandomPopulationTraderGoldVariation + pGlobalScriptDef->RandomPopulationTraderGoldMinimum;
            Game->EntityGiveGold(Me, (long)((1.0f - ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion]) * gold));
        } else if (!strcmp(Me.GetName(), "RandomPopulationGuard"))
            MyType = 1;
        else if (!strcmp(Me.GetName(), "RandomPopulationBandit"))
            MyType = 2;
        else
            MyType = 0;
        ParentClass->CreaturesAlive[MyType]++;
    }
};

int main()
{
    CGame g; CSim sim; CCreature c; c.Game = &g; c.ParentClass = &sim;
    sim.CurrentPopulationScale[0] = 0.0f; sim.CurrentPopulationScale[1] = 0.75f; sim.CurrentPopulationRegion = 1;
    sim.CreaturesAlive[0] = sim.CreaturesAlive[1] = sim.CreaturesAlive[2] = 0;
    g_def.RandomPopulationTraderGoldMinimum = 100; g_def.RandomPopulationTraderGoldVariation = 50; g_rand = 123;
    int ok = 1;
    c.AttachedToVillage = true; c.Me.Name = "RandomPopulationVillager"; g_goldCalls = 0;
    c.Init();
    ok = ok && !c.AttachedToVillage && c.MyType == 0 && c.Exit.Id == 7 && !strcmp(g_exitQuery, "RandomPopulationExit")
        && g_goldCalls == 1 && g_gold == 30 && sim.CreaturesAlive[0] == 1;   // (1 - 0.75) * (123 % 50 + 100) = 0.25 * 123
    c.Me.Name = "RandomPopulationGuard"; c.Init();
    ok = ok && c.MyType == 1 && sim.CreaturesAlive[1] == 1 && g_goldCalls == 1;
    c.Me.Name = "RandomPopulationBandit"; c.Init();
    ok = ok && c.MyType == 2 && sim.CreaturesAlive[2] == 1;
    c.Me.Name = 0; c.Init();
    ok = ok && c.MyType == 0 && sim.CreaturesAlive[0] == 2 && g_goldCalls == 1;
    if (!ok) { printf("00ec1cd0_TEST FAIL\n"); return 1; }
    printf("00ec1cd0_TEST PASS\n");
    return 0;
}
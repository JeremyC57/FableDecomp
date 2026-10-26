#include <cstdio>
// Behaviour test for 00e5fc70: CBodyGuard::GetHenchmanGoldCost maps HenchManType 0/2 to the cheap wage, 1/3 to the
// expensive wage, everything else to 0.
class CScriptDef { public: long HenchmanPayTimeDelayInSeconds, HenchmanCheapGoldCost, HenchmanExpensiveGoldCost; };
static CScriptDef g_def;
static CScriptDef* g_pScriptDef = &g_def;
struct CBodyGuard {
    long HenchManType;
    long GetHenchmanGoldCost() const
    {
        switch (HenchManType)
        {
        case 0: case 2: return g_pScriptDef->HenchmanCheapGoldCost;
        case 1: case 3: return g_pScriptDef->HenchmanExpensiveGoldCost;
        }
        return 0;
    }
};
int main()
{
    g_def.HenchmanCheapGoldCost = 100; g_def.HenchmanExpensiveGoldCost = 250;
    long expect[6] = { 100, 250, 100, 250, 0, 0 };
    long types[6] = { 0, 1, 2, 3, 4, -1 };
    for (int i = 0; i < 6; ++i)
    {
        CBodyGuard g; g.HenchManType = types[i];
        if (g.GetHenchmanGoldCost() != expect[i]) { std::printf("00e5fc70_TEST FAIL\n"); return 1; }
    }
    std::printf("00e5fc70_TEST PASS\n");
    return 0;
}
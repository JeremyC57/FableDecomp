#include <cstdio>
class CTrophyDef { public: bool EvilTrophy; bool FakeTrophy; };
struct CDefPointerTrophyDef { const CTrophyDef* Object; };
static CTrophyDef g_def;
class CDefClassBase { public: bool GetSubDef(CDefPointerTrophyDef* out) { out->Object = &g_def; return true; } };
class CThing { public: CDefClassBase* Def; };
struct CWitnessList { int count; void clear() { count = 0; } };
class CTCTrophy { public:
    CThing* Thing; CDefPointerTrophyDef PDef; CWitnessList AvailableWitnesses, WitnessesAchieved; bool Mountable;
    void OnCreate()
    {
        Thing->Def->GetSubDef(&PDef);
        AvailableWitnesses.clear();
        Mountable = !PDef.Object->FakeTrophy;
    }
};
int main()
{
    CDefClassBase defs; CThing thing; thing.Def = &defs;
    CTCTrophy t; t.Thing = &thing; t.PDef.Object = 0; t.AvailableWitnesses.count = 3; t.WitnessesAchieved.count = 2; t.Mountable = false;
    g_def.FakeTrophy = false; t.OnCreate();
    if (t.PDef.Object != &g_def || t.AvailableWitnesses.count != 0 || t.WitnessesAchieved.count != 2 || !t.Mountable) { std::printf("006dda20_TEST FAIL\n"); return 1; }
    g_def.FakeTrophy = true; t.OnCreate();
    if (t.Mountable) { std::printf("006dda20_TEST FAIL\n"); return 1; }
    std::printf("006dda20_TEST PASS\n");
    return 0;
}
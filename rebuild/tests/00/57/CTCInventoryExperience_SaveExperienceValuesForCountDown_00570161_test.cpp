// Behaviour test for 0x00570161 CTCInventoryExperience::SaveExperienceValuesForCountDown
#include <cstdio>
struct CHeroExp { long GeneralExperience; long stats[3]; long GetStatExperience(long s) { return stats[s]; } };
class CThing { public: CHeroExp* exp; bool GetInterface(CHeroExp** out, long) { if (!exp) return false; *out = exp; return true; } };
struct CInv {
    CThing* ParentThing; long PointsGeneral, PointsStrength, PointsSkill, PointsWill;
    void Save()
    {
        CHeroExp* experience;
        if (ParentThing->GetInterface(&experience, 0x68)) {
            PointsGeneral = experience->GeneralExperience;
            PointsStrength = experience->GetStatExperience(0);
            PointsSkill = experience->GetStatExperience(1);
            PointsWill = experience->GetStatExperience(2);
        }
    }
};
int main()
{
    CHeroExp e = { 100, { 10, 20, 30 } }; CThing t = { &e };
    CInv inv = { &t, -1, -1, -1, -1 };
    inv.Save();
    bool ok = inv.PointsGeneral == 100 && inv.PointsStrength == 10 && inv.PointsSkill == 20 && inv.PointsWill == 30;
    CThing none = { 0 }; CInv inv2 = { &none, -1, -1, -1, -1 }; inv2.Save();
    if (inv2.PointsGeneral != -1) ok = false;
    if (!ok) { std::printf("00570161_TEST FAIL\n"); return 1; }
    std::printf("00570161_TEST PASS\n");
    return 0;
}
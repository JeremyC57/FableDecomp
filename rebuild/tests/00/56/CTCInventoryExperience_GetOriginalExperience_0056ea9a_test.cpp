#include <cstdio>
// Behaviour test for 0x0056EA9A: pool 0..3 -> OrigGeneral/Strength/Skill/Will, else 0.
struct CTCInventoryExperience {
    long OrigGeneral, OrigStrength, OrigSkill, OrigWill;
    long GetOriginalExperience(long pool);
};
long CTCInventoryExperience::GetOriginalExperience(long pool)
{
    if (pool == 0)
        return OrigGeneral;
    else if (pool == 1)
        return OrigStrength;
    else if (pool == 2)
        return OrigSkill;
    else if (pool == 3)
        return OrigWill;
    return 0;
}
int main()
{
    CTCInventoryExperience e; e.OrigGeneral = 11; e.OrigStrength = 22; e.OrigSkill = 33; e.OrigWill = 44;
    if (e.GetOriginalExperience(0) != 11 || e.GetOriginalExperience(1) != 22 || e.GetOriginalExperience(2) != 33 ||
        e.GetOriginalExperience(3) != 44 || e.GetOriginalExperience(4) != 0 || e.GetOriginalExperience(-1) != 0) {
        std::printf("0056ea9a_TEST FAIL\n"); return 1;
    }
    std::printf("0056ea9a_TEST PASS\n");
    return 0;
}
// behaviour test for 0x0057CAE9
#include <stdio.h>
class CTCHeroStats {
public:
    unsigned char _pad00[0x70];
    long          RenownLevel;                           // +0x70
    long          RenownMaxLevel;                        // +0x74
    long          RenownPointsInLevel;                   // +0x78
    void UpgradeRenownLevel();
    int checks; void CheckForNewExpressions() { ++checks; }
};
void CTCHeroStats::UpgradeRenownLevel()
{
    const long maxLevel = RenownMaxLevel;
    ++RenownLevel;
    if (RenownLevel > maxLevel)
        RenownLevel = maxLevel;
    RenownPointsInLevel = 0;
    CheckForNewExpressions();
}
int main()
{
    CTCHeroStats h; h.RenownLevel = 3; h.RenownMaxLevel = 5; h.RenownPointsInLevel = 77; h.checks = 0; int ok = 1;
    h.UpgradeRenownLevel(); ok &= h.RenownLevel == 4 && h.RenownPointsInLevel == 0 && h.checks == 1;
    h.UpgradeRenownLevel(); h.UpgradeRenownLevel(); ok &= h.RenownLevel == 5 && h.checks == 3;
    printf(ok ? "0057cae9_TEST PASS\n" : "0057cae9_TEST FAIL\n");
    return ok ? 0 : 1;
}
// CTCHeroStats::UpgradeRenownLevel() -- raises the hero's renown level by one (capped), clears the in-level points, refreshes expressions
// Leaf: UpgradeRenownLevel@CTCHeroStats
// Retail: Fable.exe @ 0x0057CAE9, 23 bytes; Ego_r.pdb ?UpgradeRenownLevel@CTCHeroStats@@QAEXXZ; tail jump to
// ?CheckForNewExpressions@CTCHeroStats@@QAEXXZ 0x0057BE71. Callers 0x0057D915, CGameScriptInterface 0x0089926F/0x0089927B.
// Layout: retail CTCHeroStats = Ego_r.pdb - 8 here (RenownLevel +0x70, RenownMaxLevel +0x74, RenownPointsInLevel +0x78;
// PDB +0x78/+0x7c/+0x80), as in the landed hero-stat rows; engine/CTCHeroStats.h carries the PDB offsets.
// Shape: size TU (`and dword ptr [ecx+78h],0` store); the cap is read into a local before the increment (retail loads it first).
#pragma optimize("s", on)
class CTCHeroStats {
public:
    unsigned char _pad00[0x70];
    long          RenownLevel;                           // +0x70
    long          RenownMaxLevel;                        // +0x74
    long          RenownPointsInLevel;                   // +0x78
    void UpgradeRenownLevel();
    void CheckForNewExpressions();                       // 0x0057BE71
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
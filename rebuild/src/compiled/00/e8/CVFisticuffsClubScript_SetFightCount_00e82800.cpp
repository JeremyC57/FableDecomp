// CV_FisticuffsClubScript::SetFightCount() -- sets the number of fights for the club of the current region from the script definition (Bowerstone also for an unknown region)
// Leaf: ?SetFightCount@CV_FisticuffsClubScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E82800, length 104 bytes (jump table at 0x00E82858 inside the row), `ret`.
// Data: the script definition @ 0x0143E90C (fight counts at +0x600 / +0x604 / +0x608 / +0x60c for regions 1..4).
// Layout (retail = Ego_r.pdb CV_FisticuffsClubScript - 0x14): RelevantRegion +0x48, FightCount +0x88.
// Shape: the region-1 case is the default arm's body (merged by VC7.1, laid out last); the cases follow the source
// order 1, 4, 2, 3.
struct CScriptDef {                                // the global script definition block
    unsigned char _pad000[0x5ec];
    long FisticuffsAllowedHitsBowerstone;          // +0x5ec (region 1)
    long FisticuffsAllowedHitsOakvale;             // +0x5f0 (region 2)
    long FisticuffsAllowedHitsBanditCamp;          // +0x5f4 (region 3)
    long FisticuffsAllowedHitsKnotholeGlade;       // +0x5f8 (region 4)
    long FisticuffsAllowedHitsScale;               // +0x5fc
    long FisticuffsFightCountBowerstone;           // +0x600 (region 1)
    long FisticuffsFightCountOakvale;              // +0x604 (region 2)
    long FisticuffsFightCountBanditCamp;           // +0x608 (region 3)
    long FisticuffsFightCountKnotholeGlade;        // +0x60c (region 4)
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CV_FisticuffsClubScript {
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    long RelevantRegion;                           // +0x48 (1 Bowerstone, 2 Oakvale, 3 Bandit Camp, 4 Knothole Glade)
    unsigned char _pad4c[0x88 - 0x4c];
    long FightCount;                               // +0x88
    bool EverybodyAttack;                          // +0x8c
    bool LostWhileOutside;                         // +0x8d
    bool FisticuffsActive;                         // +0x8e
    long AllowedHits;                              // +0x90
    void SetFightCount();
};
void CV_FisticuffsClubScript::SetFightCount()
{
    switch (RelevantRegion)
    {
    case 1:
        FightCount = pGlobalScriptDef->FisticuffsFightCountBowerstone;
        return;
    case 4:
        FightCount = pGlobalScriptDef->FisticuffsFightCountKnotholeGlade;
        return;
    case 2:
        FightCount = pGlobalScriptDef->FisticuffsFightCountOakvale;
        return;
    case 3:
        FightCount = pGlobalScriptDef->FisticuffsFightCountBanditCamp;
        return;
    }
    FightCount = pGlobalScriptDef->FisticuffsFightCountBowerstone;
}
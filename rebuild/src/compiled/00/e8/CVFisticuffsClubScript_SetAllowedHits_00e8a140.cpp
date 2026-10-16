// CV_FisticuffsClubScript::SetAllowedHits(long hits) -- sets the hits allowed in the coming fight: the region's base allowance minus hits / FightCount scaled by the definition's factor, truncated; an unknown region simply takes the given hits
// Leaf: SetAllowedHits
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E8A140, length 116 bytes (jump table at 0x00E8A1B4 inside the row), `ret 4`.
// Callees: __ftol2 @ 0x00BFEA70. Data: the script definition @ 0x0143E90C (allowances +0x5ec / +0x5f0 / +0x5f4 / +0x5f8 for
// regions 1..4, the scale at +0x5fc).
// Layout (retail = Ego_r.pdb CV_FisticuffsClubScript - 0x14): RelevantRegion +0x48, FightCount +0x88, AllowedHits +0x90.
// Shape: every case is the whole statement; VC7.1 cross-jumps the common `fild hits ; fidiv ; fimul ; fsubp ; ftol`
// tail, the cases in source order 1, 4, 2, 3.
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
extern CScriptDef* g_pScriptDef;                   // 0x0143E90C
struct CV_FisticuffsClubScript {
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    long RelevantRegion;                           // +0x48 (1 Bowerstone, 2 Oakvale, 3 Bandit Camp, 4 Knothole Glade)
    unsigned char _pad4c[0x88 - 0x4c];
    long FightCount;                               // +0x88
    bool EverybodyAttack;                          // +0x8c
    bool LostWhileOutside;                         // +0x8d
    bool FisticuffsActive;                         // +0x8e
    long AllowedHits;                              // +0x90
    void SetAllowedHits(long hits);
};
void CV_FisticuffsClubScript::SetAllowedHits(long hits)
{
    switch (RelevantRegion)
    {
    case 1:
        AllowedHits = (long)(g_pScriptDef->FisticuffsAllowedHitsBowerstone - (float)hits / FightCount * g_pScriptDef->FisticuffsAllowedHitsScale);
        return;
    case 4:
        AllowedHits = (long)(g_pScriptDef->FisticuffsAllowedHitsKnotholeGlade - (float)hits / FightCount * g_pScriptDef->FisticuffsAllowedHitsScale);
        return;
    case 2:
        AllowedHits = (long)(g_pScriptDef->FisticuffsAllowedHitsOakvale - (float)hits / FightCount * g_pScriptDef->FisticuffsAllowedHitsScale);
        return;
    case 3:
        AllowedHits = (long)(g_pScriptDef->FisticuffsAllowedHitsBanditCamp - (float)hits / FightCount * g_pScriptDef->FisticuffsAllowedHitsScale);
        return;
    }
    AllowedHits = hits;
}
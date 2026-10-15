// CTCHeroStats::ApplyPenaltyForDeath() -- takes the hero-stats definition's renown and money losses on death off the hero (always as losses, whatever their sign)
// Grade: DIFFER(67v67) -- register choice only: the second def-object load lands in ecx (`mov ecx,[eax+0DCh] ; mov eax,[ecx+58h]`) where retail reuses eax
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057DE14, length 67 bytes.
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0 (twice), CTCHeroStats::AddRenown(long)
//          0x0057D8BE, CTCHeroStats::AddMoney(long, bool) 0x0057B327 (Ego_r PDB:
//          ?ApplyPenaltyForDeath@CTCHeroStats@@QAEXXZ, ?AddMoney@CTCHeroStats@@QAEXJ_N@Z).
// The game definition manager (no PDB layout) holds the hero-stats def object at +0xdc; in retail
// CHeroStatsDef the PDB's RenownLossOnDeath (+0x58) and MoneyLossOnDeath (+0x5c) sit 4 bytes lower.
// Shape: `-abs(x)` is the intrinsic `cdq ; xor ; sub ; neg`; the definition manager is fetched again
// for the second penalty (two calls, as written). The money loss magnitude is a named local: retail
// computes abs() before pushing AddMoney's `true` and negates after it (`push 1` sits between the
// `sub` and the `neg`); written inline as the argument, VC7.1 pushes the constant before the
// GetDefinitionManager call.
#include <stdlib.h>
#include "engine/CTCHeroStats.h"

struct CHeroStatsDef_Retail {
    unsigned char _pad_0x00[0x54];
    long          RenownLossOnDeath;           // +0x54 (PDB +0x58)
    long          MoneyLossOnDeath;            // +0x58 (PDB +0x5c)
};

struct CGameDefinitionManager {
    unsigned char               _pad_0x00[0xdc];
    const CHeroStatsDef_Retail* PHeroStatsDef;  // +0xdc
};

struct CTCHeroStats_Methods : CTCHeroStats {
    const CGameDefinitionManager& GetDefinitionManager() const;   // CTCBase, 0x006869D0
    void AddRenown(long renown);                                  // 0x0057D8BE
    void AddMoney(long money, bool countAsEarned);                // 0x0057B327
    void ApplyPenaltyForDeath();
};

void CTCHeroStats_Methods::ApplyPenaltyForDeath()
{
    AddRenown(-abs(GetDefinitionManager().PHeroStatsDef->RenownLossOnDeath));
    long moneyLoss = GetDefinitionManager().PHeroStatsDef->MoneyLossOnDeath;
    AddMoney(-abs(moneyLoss), true);
}

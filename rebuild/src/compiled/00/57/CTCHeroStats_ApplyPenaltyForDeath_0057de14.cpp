// CTCHeroStats::ApplyPenaltyForDeath() -- takes the hero-stats definition's renown and money losses on death off the hero (always as losses, whatever their sign)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057DE14, length 67 bytes.
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0 (twice), CTCHeroStats::AddRenown(long)
//          0x0057D8BE, CTCHeroStats::AddMoney(long, bool) 0x0057B327 (Ego_r PDB:
//          ?ApplyPenaltyForDeath@CTCHeroStats@@QAEXXZ, ?AddMoney@CTCHeroStats@@QAEXJ_N@Z).
// The game definition manager (no PDB layout) holds the hero-stats def object at +0xdc; in retail
// CHeroStatsDef the PDB's RenownLossOnDeath (+0x58) and MoneyLossOnDeath (+0x5c) sit 4 bytes lower.
// Shape: `-abs(x)` is the intrinsic `cdq ; xor ; sub ; neg`; the definition manager is fetched again
// for the second penalty (two calls, as written).
// Retry 10b: the second penalty reads the def object through a named `const CHeroStatsDef_Retail* def` local (the
// money-loss magnitude local of the parked form kept ecx for the middle load; the def local gives retail's eax chain).
#include <stdlib.h>
#define CTCHeroStats CTCHeroStatsLayout
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CGameDefinitionManager;

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
class CTCHeroStats : public CTCBase, public CTCHeroStatsLayout
{
public:
    void AddMoney(long, bool);   // 0x0057B327
    void AddRenown(long);   // 0x0057D8BE
};

struct CHeroStatsDef_Retail {
    unsigned char _pad_0x00[0x54];
    long          RenownLossOnDeath;           // +0x54 (PDB +0x58)
    long          MoneyLossOnDeath;            // +0x58 (PDB +0x5c)
};

class CGameDefinitionManager { public:
    unsigned char               _pad_0x00[0xdc];
    const CHeroStatsDef_Retail* PHeroStatsDef;  // +0xdc
};

struct CTCHeroStats_Methods : CTCHeroStats {
    void ApplyPenaltyForDeath();
};

void CTCHeroStats_Methods::ApplyPenaltyForDeath()
{
    AddRenown(-abs(GetDefinitionManager().PHeroStatsDef->RenownLossOnDeath));
    const CHeroStatsDef_Retail* def = GetDefinitionManager().PHeroStatsDef;
    AddMoney(-abs(def->MoneyLossOnDeath), true);
}
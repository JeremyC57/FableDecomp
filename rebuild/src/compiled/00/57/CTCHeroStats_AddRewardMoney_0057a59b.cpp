// CTCHeroStats::AddRewardMoney(long amount) -- add a quest reward payout to the hero's running RewardMoney total
// Leaf: ?AddRewardMoney@CTCHeroStats@@QAEXJ@Z
// Retail: Fable.exe @ 0x0057A59B, 10 bytes, thiscall `ret 4`; /Os TU (packed address, the CTCHeroStats run
// 0x0057A56C..0x0057A6B8).
// Identity (proven): the body is `[ecx+50h] += amount`, and retail CTCHeroStats::RewardMoney is +0x50 (Ego_r.pdb +0x58,
// 8 bytes behind the STLport OutstandingCrimes map and ExperienceSpentOn vector; engine/CTCHeroStats.h pins it). Ego_r.pdb
// has exactly one adder over that member, ?AddRewardMoney@CTCHeroStats@@QAEXJ@Z (the other `QAEXJ` adders AddRenown /
// AddRentMoney / AddSpentExperience touch other members). Callers 0x004B1F93 / 0x005E7D71 pass a reward record's money word.
// Label correction: the catalog's "CTCCoopSpirit::AddExperience" is wrong (CTCCoopSpirit is 0x34 bytes in Ego_r.pdb, no
// member at +0x50, and this sits inside the CTCHeroStats TU).
// Callees: none.
#pragma optimize("s", on)
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats

class CTCHeroStats : public CTCHeroStatsDonor {
public:
    void AddRewardMoney(long amount);
};

void CTCHeroStats::AddRewardMoney(long amount)
{
    RewardMoney += amount;
}
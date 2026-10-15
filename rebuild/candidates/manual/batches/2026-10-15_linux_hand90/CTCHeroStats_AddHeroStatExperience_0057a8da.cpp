// CTCHeroStats::AddHeroStatExperience(EHeroStat, long) -- banks experience in one of the four hero stats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A8DA, 49 bytes.
//
// Shape: a size-optimised TU (`push 3 ; pop edx`, `xor edx,edx ; inc edx`). The EHeroStat value is mapped to a
// vector index by an inline `switch` into a local that starts at 0 (cases 0..3 store themselves, anything else keeps
// 0; written with `return` per case the zero is not shared and case 0 tests `sub eax,0`): the zero register doubles as
// the case-0 test (`xor edx,edx ; sub eax,edx ; je`), the remaining cases are a `dec eax ; je` chain. The index then
// addresses HeroStatsExperience (+0x118) with `lea eax,[eax+edx*4]` and the amount is added in memory. GetHeroStatExperience
// (0x0057A8AF) is the read twin with the same switch.
// Layout: retail CTCHeroStats sits 8 bytes below the PDB here (map and vector ahead are 12 bytes, not 16):
// HeroStatsExperience +0x118 (PDB +0x120).
#pragma optimize("s", on)

static inline long GetHeroStatIndex(long stat)
{
    long index = 0;
    switch (stat) {
    case 0: index = 0; break;
    case 1: index = 1; break;
    case 2: index = 2; break;
    case 3: index = 3; break;
    }
    return index;
}

struct CTCHeroStats_Retail {
    unsigned char _pad_0x00[0x118];
    long*         HeroStatsExperience;         // +0x118 vector<long>::_First (PDB +0x120)

    void AddHeroStatExperience(long stat, long amount);
};

void CTCHeroStats_Retail::AddHeroStatExperience(long stat, long amount)
{
    HeroStatsExperience[GetHeroStatIndex(stat)] += amount;
}

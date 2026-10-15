// CTCHeroStats::GetNumberOfRegionsVisited() const -- how many regions the hero has spent any time in (non-zero RegionTime entries)
#pragma optimize("s", on)
// Leaf: GetNumberOfRegionsVisited
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AA1B, length 64 bytes.
// Callers: the stats screen (0x005CF7C2, 0x005D3DAF).
// Label: the manifest's "CTCInventoryAbilities::GetNumAssignedReceivedAbilities" is propagated over a byte-shape
//        twin (0x005EE146); this body sits among the CTCHeroStats getters (0x0057A8AF..0x0057AAD9) and reads the
//        vector at +0x124, which is RegionTime in the retail CTCHeroStats layout (HeroStatsExperience +0x118, see
//        the landed 0x0057A8AF; the 12-byte vectors put RegionTime 0x0C bytes lower than the PDB's +0x130).
// Layout: engine/CTCHeroStats.h is the Ego_r.pdb layout (16-byte vectors), so a local view names the member.
// Pragma: optimize("s", on) -- size TU (unaligned loop head; `first` re-read after the entry test, the
//        speed build keeps it in edi from the size computation).
// Shape: the count runs `i != size()` (`test edx,0FFFFFFFCh` entry test, `jne` back edge; the `<` form gives
//        `sar ; je` and `jb`), size() re-evaluated per iteration, the element pointer strength-reduced.

struct CLongVector {
    long* first;                                  // +0x00
    long* last;                                   // +0x04
    long* end_of_storage;                         // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    const long& operator[](unsigned long n) const { return first[n]; }
};

struct CTCHeroStats_Retail {
    unsigned char _pad_0x000[0x124];
    CLongVector   RegionTime;                     // +0x124 (PDB +0x130)

    long GetNumberOfRegionsVisited() const;
};

long CTCHeroStats_Retail::GetNumberOfRegionsVisited() const
{
    long visited = 0;
    for (unsigned long i = 0; i != RegionTime.size(); ++i) {
        if (RegionTime[i] != 0)
            ++visited;
    }
    return visited;
}

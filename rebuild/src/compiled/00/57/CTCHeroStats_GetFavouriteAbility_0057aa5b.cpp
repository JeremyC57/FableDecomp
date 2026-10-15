// CTCHeroStats::GetFavouriteAbility() const -- index of the most-used ability (ties keep the first; -1 when no ability was ever used)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AA5B, 77 bytes.
//
// Shape: two locals start at -1 (`or eax,-1 ; mov ebx,eax`: the favourite index in eax, the best count in ebx); the
// loop over NumberOfTimesUsedAbility (first +0x130, last +0x134) is rotated behind a `size() != 0` test folded into
// `test edx,0FFFFFFFCh`, walks the elements with a pointer (`add esi,4`) and re-evaluates size() (`sub ; sar edx,2`)
// for the `cmp edi,edx ; jne` exit -- the loop test is `i != size()` (written `<` VC emits `sar ; je` for the entry
// test and `jb` for the exit) with the vector's inline size() (no null test, STLport-style `last - first`). A count replaces the best only when it beats it AND is positive.
// Layout: retail CTCHeroStats sits 0x10 below the PDB here (map and two vectors ahead are 12 bytes, not 16):
// NumberOfTimesUsedAbility +0x130 (PDB +0x140).
#pragma optimize("s", on)

struct CLongVector {                           // retail std::vector<long>: first, last, end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CTCHeroStats_Retail {
    unsigned char _pad_0x00[0x130];
    CLongVector   NumberOfTimesUsedAbility;    // +0x130 (PDB +0x140)

    long GetFavouriteAbility() const;
};

long CTCHeroStats_Retail::GetFavouriteAbility() const
{
    long favourite = -1;
    long mostUses = -1;
    for (unsigned long ability = 0; ability != NumberOfTimesUsedAbility.size(); ++ability) {
        long uses = NumberOfTimesUsedAbility[ability];
        if (uses > mostUses && uses > 0) {
            mostUses = uses;
            favourite = ability;
        }
    }
    return favourite;
}
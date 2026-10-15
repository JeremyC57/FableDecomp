// CTCHeroStats::GetNumberOfAbilityCasts() const -- total number of times the hero has used any ability
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AAA8, 49 bytes.
//
// Shape: GetFavouriteAbility's (0x0057AA5B) loop shape over NumberOfTimesUsedAbility (+0x130): `i != size()` with the
// entry test folded into `test edx,0FFFFFFFCh`; here nothing is stored in the loop, so size() and the first pointer
// are hoisted after the entry test and the body is a bare `add eax,[ecx] ; inc esi ; add ecx,4`. The vector is
// addressed through one materialised pointer (`add ecx,130h`).
// Layout: NumberOfTimesUsedAbility +0x130 in retail (PDB +0x140; see GetFavouriteAbility).
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

    long GetNumberOfAbilityCasts() const;
};

long CTCHeroStats_Retail::GetNumberOfAbilityCasts() const
{
    long casts = 0;
    for (unsigned long ability = 0; ability != NumberOfTimesUsedAbility.size(); ++ability)
        casts += NumberOfTimesUsedAbility[ability];
    return casts;
}

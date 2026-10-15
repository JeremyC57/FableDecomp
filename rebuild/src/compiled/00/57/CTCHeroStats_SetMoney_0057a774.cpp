// CTCHeroStats::SetMoney(long, bool) -- sets the purse (optionally booking the difference as earned or spent), raises the high-water mark and floors the purse at 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A774, 69 bytes. Called by SetMoneyAsConfiscated (0x0057C999, landed)
// with `false`.
//
// Shape: a size-optimised TU (`push esi` shrink-wrapped into the earned arm, `and dword ptr [eax],0`). With the flag
// set, a rise is added to TotalMoneyEarned (+0x48) and a fall to TotalMoneySpent (+0x4C) (`jle` then `jge` on the
// same compare). Money (+0x3C) is then stored through a pointer (`lea eax,[ecx+3Ch]`) and every later access goes
// through it: MaxMoney (+0x40) = the Dinkumware-style `Max(MaxMoney, Money)` (`a < b ? b : a` on const references --
// the selected ADDRESS is loaded, `jl` keeps &Money) and the purse is clamped by a `long&` helper
// (`cmp [eax],0 ; jge ; and [eax],0`) -- the same two inline helpers as SetMaxStamina (0x0057A730, landed).
// Layout: retail CTCHeroStats sits 8 bytes below the PDB from here on (the OutstandingCrimes map and the
// ExperienceSpentOn vector are 12 bytes, not 16): Money +0x3C (PDB +0x44) .. TotalMoneySpent +0x4C (PDB +0x54).
#pragma optimize("s", on)

template <class T> inline const T& Max(const T& left, const T& right)
{
    return left < right ? right : left;
}

static inline void ClampMin(long& value, long minimum)
{
    if (value < minimum)
        value = minimum;
}

struct CTCHeroStats_Retail {
    unsigned char _pad_0x00[0x3C];
    long          Money;                       // +0x3C (PDB +0x44)
    long          MaxMoney;                    // +0x40 (PDB +0x48)
    long          ConfiscatedMoney;            // +0x44 (PDB +0x4C)
    long          TotalMoneyEarned;            // +0x48 (PDB +0x50)
    long          TotalMoneySpent;             // +0x4C (PDB +0x54)

    void SetMoney(long amount, bool countTransaction);
};

void CTCHeroStats_Retail::SetMoney(long amount, bool countTransaction)
{
    if (countTransaction) {
        if (amount > Money)
            TotalMoneyEarned += amount - Money;
        else if (amount < Money)
            TotalMoneySpent += Money - amount;
    }
    Money = amount;
    MaxMoney = Max(MaxMoney, Money);
    ClampMin(Money, 0);
}
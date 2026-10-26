// CTCHeroStats::AddMoney(long, bool) -- adds to the purse, clamps it to [0, def max], raises the high-water mark and books the change
// Leaf: AddMoney
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057B327, 187 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), the same TU as the landed SetMoney (0x0057A774) and
// SetSunTan (0x0057B0F7).
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (+0xDC its CHeroStatsDef; MaxMoney at def +0x5C),
//          std::bitset::test @ 0x00410DE0 (thing interface presence, +0x20), CVectorMap<ETCInterfaceType,CTCBase*>::
//          LowerBound @ 0x0040F020 (+0x44), CTCHeroOnlineScoreboard::UpdateScoreForActivity(long, float, bool)
//          @ 0x0055F3B0.
// Shape: the purse is clamped by an inline `GFLimit(long&, const long&, const long&)` (the def maximum by address:
// `add eax,5Ch` before the low test; the low bound 0 folds to `and [esi+3Ch],0`); MaxMoney takes the Dinkumware
// `Max(MaxMoney, amount)` of the ARGUMENT (`lea ecx,[ebp+8]`). Earned money is reported to the hero's online
// scoreboard (interface 0x10D) through the unchecked out-parameter GetInterface: the absent arm reads the
// never-written out pointer from the dead `countTransaction` slot the key was homed in.
// Layout: retail CTCHeroStats sits 8 bytes below the PDB (engine/CTCHeroStats.h carries PDB offsets): Money +0x3C,
// MaxMoney +0x40, TotalMoneyEarned +0x48, TotalMoneySpent +0x4C.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;

template <class T> inline const T& Max(const T& left, const T& right)
{
    return left < right ? right : left;
}

template <class T> __forceinline void GFLimit(T& value, const T& lo, const T& hi)
{
    if (value < lo)
        value = lo;
    else if (value > hi)
        value = hi;
}

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0x5C];
    long          MaxMoney;                          // +0x5C
};

class CDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC
};

class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);  // 0x0055F3B0
};

class CTCHeroStats { public:
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x34];
    long          Money;                             // +0x3C (PDB +0x44)
    long          MaxMoney;                          // +0x40 (PDB +0x48)
    long          ConfiscatedMoney;                  // +0x44 (PDB +0x4C)
    long          TotalMoneyEarned;                  // +0x48 (PDB +0x50)
    long          TotalMoneySpent;                   // +0x4C (PDB +0x54)

    CDefinitionManager* GetDefinitionManager() const;  // 0x006869D0 (CTCBase)
    void AddMoney(long amount, bool countTransaction);
};

void CTCHeroStats::AddMoney(long amount, bool countTransaction)
{
    long oldMoney = Money;
    Money = oldMoney + amount;
    GFLimit(Money, 0L, GetDefinitionManager()->PHeroStatsDef->MaxMoney);
    MaxMoney = Max(MaxMoney, amount);
    long change = Money - oldMoney;
    if (change > 0) {
        if (countTransaction) {
            TotalMoneyEarned += change;
            CTCHeroOnlineScoreboard* scoreboard;
            ParentThing->GetInterface(&scoreboard, 0x10D);
            scoreboard->UpdateScoreForActivity(5, (float)TotalMoneyEarned, false);
        }
    } else if (change < 0) {
        if (countTransaction)
            TotalMoneySpent -= change;
    }
}
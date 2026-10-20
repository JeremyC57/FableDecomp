// CTCHeroStats::AddRenown(long) -- adds renown, levels up while the current level is paid for, and shows the change overhead and in the player GUI
// Leaf: ?AddRenown@CTCHeroStats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057D8BE, 210 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CTCHeroStats::GetPercentageThroughRenownLevel() const @ 0x0057AAD9 (landed), CTCBase::
//          GetDefinitionManager() @ 0x006869D0 (+0xDC its CHeroStatsDef; RenownLevelCosts at def +0xAC),
//          CTCHeroStats::UpgradeRenownLevel() @ 0x0057CAE9, std::bitset::test @ 0x00410DE0, CVectorMap<
//          ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, the overhead display's renown entry (interface 0xB7)
//          @ 0x007B7CD0 (manifest "CTCMarketPlace_AddRenownEntry"; the class is the one built by the CTCOverheadDisplay
//          ctor 0x007B7BC0 next to it), and the player GUI's renown-change setter @ 0x00437920 (stores the value in
//          the sub-object at +0x1A8; manifest "AddMoralityChange" is propagated) through g_pPlayerGui @ 0x013B8790.
// Shape: both counters are floored at 0 through the `long&` ClampMin helper (`cmp [m],0 ; jge ; and [m],0`); the
// level-up loop is `while (GetPercentageThroughRenownLevel() >= 1.0f)` (`test ah,1 ; jne` exit, rotated), the
// carried-over points computed before UpgradeRenownLevel and stored after it. The overhead entry uses the CHECKED
// out-parameter GetInterface (the absent arm skips to the GUI call).
// Layout: retail CTCHeroStats sits 8 bytes below the PDB: RenownLevel +0x70, RenownPointsInLevel +0x78,
// RenownTotal +0x7C.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

static inline void ClampMin(long& value, long minimum)
{
    if (value < minimum)
        value = minimum;
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

struct CThing {
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

struct CLongVector {                                 // retail std::vector<long>: first, last, end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CHeroStatsDef {
    unsigned char _pad_0x00[0xAC];
    CLongVector   RenownLevelCosts;                  // +0xAC (PDB +0xB4)
};

struct CDefinitionManager {
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC
};

struct CTCOverheadDisplay {
    void AddRenownEntry(float renown);               // 0x007B7CD0
};

struct CPlayerGui {
    void SetRenownChange_00437920(long renown);      // 0x00437920
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

struct CTCHeroStats {
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x68];
    long          RenownLevel;                       // +0x70 (PDB +0x78)
    long          RenownMaxLevel;                    // +0x74 (PDB +0x7C)
    long          RenownPointsInLevel;               // +0x78 (PDB +0x80)
    long          RenownTotal;                       // +0x7C (PDB +0x84)

    CDefinitionManager* GetDefinitionManager() const;  // 0x006869D0 (CTCBase)
    float GetPercentageThroughRenownLevel() const;   // 0x0057AAD9
    void UpgradeRenownLevel();                       // 0x0057CAE9
    void AddRenown(long renown);
};

void CTCHeroStats::AddRenown(long renown)
{
    RenownPointsInLevel += renown;
    ClampMin(RenownPointsInLevel, 0);
    RenownTotal += renown;
    ClampMin(RenownTotal, 0);
    while (GetPercentageThroughRenownLevel() >= 1.0f) {
        const CHeroStatsDef* def = GetDefinitionManager()->PHeroStatsDef;
        long carried = RenownPointsInLevel - def->RenownLevelCosts.m_pFirst[RenownLevel];
        UpgradeRenownLevel();
        RenownPointsInLevel = carried;
    }
    if (renown != 0) {
        CTCOverheadDisplay* display;
        if (ParentThing->GetInterface(&display, 0xB7))
            display->AddRenownEntry((float)renown);
        g_pPlayerGui->SetRenownChange_00437920(renown);
    }
}
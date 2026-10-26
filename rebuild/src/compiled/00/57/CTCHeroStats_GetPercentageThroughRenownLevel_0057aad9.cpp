// CTCHeroStats::GetPercentageThroughRenownLevel() const -- renown points earned in the current level as a fraction of that level's cost (0 outside the cost table)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AAD9, 59 bytes.
// Callee: CTCBase::GetDefinitionManager() @ 0x006869D0 (ParentThing's definition holder; +0xDC its CDefPointer<CHeroStatsDef>).
// Globals: 0.0f @ 0x0122DEDC.
//
// Shape: the level is range-checked signed (`test ecx,ecx ; jl`) and then unsigned against the cost vector's size
// (`cmp ; jae`, vector addressed through one materialised pointer `add eax,0ACh`); the in-range path is
// `fild points ; fidiv cost` = `(float)RenownPointsInLevel / cost` with the long divisor used straight from memory;
// the fall-back `return 0.0f` loads the pool constant.
// Layout: retail CTCHeroStats sits 8 bytes below the PDB here: RenownLevel +0x70 (PDB +0x78), RenownPointsInLevel
// +0x78 (PDB +0x80); CHeroStatsDef::RenownLevelCosts +0xAC (PDB +0xB4, a 12-byte vector).
#pragma optimize("s", on)

struct CLongVector {                           // retail std::vector<long>: first, last, end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0xAC];
    CLongVector   RenownLevelCosts;            // +0xAC (PDB +0xB4)
};

class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;              // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

struct CTCHeroStats_Retail : public CTCBase {
    void*         __vftable;                   // +0x00
    void*         ParentThing;                 // +0x04 CTCBase
    unsigned char _pad_0x08[0x68];
    long          RenownLevel;                 // +0x70 (PDB +0x78)
    long          RenownMaxLevel;              // +0x74 (PDB +0x7C)
    long          RenownPointsInLevel;         // +0x78 (PDB +0x80)

    float GetPercentageThroughRenownLevel() const;
};

float CTCHeroStats_Retail::GetPercentageThroughRenownLevel() const
{
    const CHeroStatsDef* def = GetDefinitionManager().PHeroStatsDef;
    if (RenownLevel >= 0 && (unsigned long)RenownLevel < def->RenownLevelCosts.size())
        return (float)RenownPointsInLevel / def->RenownLevelCosts[RenownLevel];
    return 0.0f;
}
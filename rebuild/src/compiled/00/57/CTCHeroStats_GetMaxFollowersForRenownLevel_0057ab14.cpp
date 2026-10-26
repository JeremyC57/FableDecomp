// CTCHeroStats::GetMaxFollowersForRenownLevel(long) const -- the most followers allowed at the hero's renown level (the table's last entry past its end, 0 for an empty table)
// Leaf: GetMaxFollowersForRenownLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AB14, 86 bytes. Manifest label "GetMaxBoastingCrowdForRenownLevel" is
// propagated from the byte-twin at 0x0057AB6A: this body reads the definition vector at +0xB8, which is
// CHeroStatsDef::RenownLevelMaxFollowers (PDB +0xC4), not RenownLevelMaxBoastingCrowd (+0xC4 in retail). The stack argument is not read (`ret 4`); the level
// used is the RenownLevel member.
// Callee: CTCBase::GetDefinitions() @ 0x006869D0 (ParentThing's definition holder; +0xDC its CDefPointer<CHeroStatsDef>).
//
// Shape: the twin of GetMaxBoastingCrowdForRenownLevel (0x0057AB6A) on another table. Three separate size() evaluations of CHeroStatsDef::RenownLevelMaxFollowers (`sub ; sar 2` each): the
// guard (`sar ; je` to a trailing `xor eax,eax`), the unsigned clamp test (`cmp ; jb`), and `size() - 1` (`dec ecx`) in
// the clamp arm -- a ternary `level < size() ? level : size() - 1` indexing the first pointer. The guard is
// `if (size() > 0) { ... return ...; } return 0;` -- the early-out `if (size() == 0) return 0;` emits
// `test ecx,0FFFFFFFCh ; jne` with the zero arm first.
// Layout: RenownLevel +0x70 in retail (PDB +0x78); CHeroStatsDef::RenownLevelMaxFollowers +0xB8 (PDB +0xC4: the
// definition head is 4 bytes smaller and two 12-byte vectors precede it).
#pragma optimize("s", on)

struct CLongVector {                           // retail std::vector<long>: first, last, end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};

class CHeroStatsDef { public:
    unsigned char _pad_0x00[0xB8];
    CLongVector   RenownLevelMaxFollowers;     // +0xB8 (PDB +0xC4)
};

struct CDefinitionHolder {
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;              // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

struct CTCHeroStats_Retail {
    void*         __vftable;                   // +0x00
    void*         ParentThing;                 // +0x04 CTCBase
    unsigned char _pad_0x08[0x68];
    long          RenownLevel;                 // +0x70 (PDB +0x78)

    CDefinitionHolder* GetDefinitions() const; // 0x006869D0
    long GetMaxFollowersForRenownLevel(long unused) const;
};

long CTCHeroStats_Retail::GetMaxFollowersForRenownLevel(long unused) const
{
    const CHeroStatsDef* def = GetDefinitions()->PHeroStatsDef;
    if (def->RenownLevelMaxFollowers.size() > 0) {
        unsigned long level = (unsigned long)RenownLevel < def->RenownLevelMaxFollowers.size()
            ? RenownLevel : def->RenownLevelMaxFollowers.size() - 1;
        return def->RenownLevelMaxFollowers[level];
    }
    return 0;
}
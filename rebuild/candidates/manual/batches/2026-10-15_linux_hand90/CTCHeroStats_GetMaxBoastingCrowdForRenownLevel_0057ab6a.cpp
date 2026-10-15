// CTCHeroStats::GetMaxBoastingCrowdForRenownLevel(long) const -- the largest boasting crowd allowed at the hero's renown level (the table's last entry past its end, 0 for an empty table)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AB6A, 86 bytes. The stack argument is not read (`ret 4`); the level
// used is the RenownLevel member.
// Callee: CTCBase::GetDefinitions() @ 0x006869D0 (ParentThing's definition holder; +0xDC its CDefPointer<CHeroStatsDef>).
//
// Shape: three separate size() evaluations of CHeroStatsDef::RenownLevelMaxBoastingCrowd (`sub ; sar 2` each): the
// guard (`sar ; je` to a trailing `xor eax,eax`), the unsigned clamp test (`cmp ; jb`), and `size() - 1` (`dec ecx`) in
// the clamp arm -- a ternary `level < size() ? level : size() - 1` indexing the first pointer. The guard is
// `if (size() > 0) { ... return ...; } return 0;` -- the early-out `if (size() == 0) return 0;` emits
// `test ecx,0FFFFFFFCh ; jne` with the zero arm first.
// Layout: RenownLevel +0x70 in retail (PDB +0x78); CHeroStatsDef::RenownLevelMaxBoastingCrowd +0xC4 (PDB +0xD4: the
// definition head is 4 bytes smaller and three 12-byte vectors precede it).
#pragma optimize("s", on)

struct CLongVector {                           // retail std::vector<long>: first, last, end
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CHeroStatsDef {
    unsigned char _pad_0x00[0xC4];
    CLongVector   RenownLevelMaxBoastingCrowd; // +0xC4 (PDB +0xD4)
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
    long GetMaxBoastingCrowdForRenownLevel(long unused) const;
};

long CTCHeroStats_Retail::GetMaxBoastingCrowdForRenownLevel(long unused) const
{
    const CHeroStatsDef* def = GetDefinitions()->PHeroStatsDef;
    if (def->RenownLevelMaxBoastingCrowd.size() > 0) {
        unsigned long level = (unsigned long)RenownLevel < def->RenownLevelMaxBoastingCrowd.size()
            ? RenownLevel : def->RenownLevelMaxBoastingCrowd.size() - 1;
        return def->RenownLevelMaxBoastingCrowd[level];
    }
    return 0;
}

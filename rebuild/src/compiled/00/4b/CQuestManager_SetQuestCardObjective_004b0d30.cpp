// CQuestManager::SetQuestCardObjective(const CCharString& scriptName, const CCharString& objective, const CCharString& region, const CCharString& teleporterRegion) -- update the objective (and the non-empty region names) on the named quest script's active quest card; false when there is no live card or nothing changed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0D30, length 282 bytes, `ret 0x10`.
// Callees: CQuestManager::GetActiveQuestCardFromScriptName 0x004B0C80 (landed), CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound 0x0040F020 (the non-const instantiation), CCharString::operator!=(const CCharString&) 0x0099E900
// (propagated label "less<CCharString>::operator()") and operator!=(const char*) 0x0099E960, CTCQuestCard::
// GetRegionName 0x007025A0 (by value), CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0 (conditional),
// CTCQuestCard::SetRegionName 0x00702560 and SetTeleporterRegionName 0x007021E0 (propagated label "_Cons_val<...>").
// Literal "" 0x0122D70E.
// Layout: CThing UsingComponents +0x20 (TC 0x6c = word 3 bit 12), flag byte +0x91 (bit 0 dead), Components +0x44;
// Ego_r.pdb CTCQuestCard ObjectiveName +0x1c.
// Shape: `changed = a != b || (region != "" && GetRegionName() != region)` -- the by-value temporary is destroyed
// under a construction flag (cleared at entry in the `push ecx` word); the lookup key and the temporary both land in
// the dead `scriptName` argument slot; HasTC + the end/hit Find inline (the landed 0x004B0C80 CThing model); nested
// ifs with one trailing `return false` keep ebp/esi/edi pushed at entry and the false exit last.
struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const CCharString& other) const;   // 0x0099E900
    bool operator!=(const char* text) const;           // 0x0099E960
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

struct CTCBase;
struct CTCEntry {                                      // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     Type;
    CTCBase* TC;
};
struct CTCMap {                                        // CThing::Components: a sorted vector of pairs (12 bytes)
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type);   // 0x0040F020
    __forceinline CTCBase* Find(long type)             // the inlined map find: the end entry when absent
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20 which TC types the thing holds
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                            // +0x91 bit 0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) { return Components.Find(type); }
};

struct CTCQuestCard {                                  // TC type 0x6c (Ego_r.pdb)
    unsigned char _pad00[0x1c];
    CCharString   ObjectiveName;                       // +0x1c
    CCharString GetRegionName();                       // 0x007025A0
    void SetRegionName(const CCharString& region);     // 0x00702560
    void SetTeleporterRegionName(const CCharString& region);   // 0x007021E0
};

struct CQuestManager {
    CThing* GetActiveQuestCardFromScriptName(const CCharString& scriptName) const;   // 0x004B0C80
    bool SetQuestCardObjective(const CCharString& scriptName, const CCharString& objective, const CCharString& region, const CCharString& teleporterRegion);
};

bool CQuestManager::SetQuestCardObjective(const CCharString& scriptName, const CCharString& objective, const CCharString& region, const CCharString& teleporterRegion)
{
    CThing* card = GetActiveQuestCardFromScriptName(scriptName);
    if (card != 0 && !card->Dead && card->HasTC(0x6c)) {
        CTCQuestCard* tc = (CTCQuestCard*)card->GetTC(0x6c);
        if (tc != 0) {
            bool changed = tc->ObjectiveName != objective || (region != "" && tc->GetRegionName() != region);
            if (changed) {
                tc->ObjectiveName = objective;
                if (region != "")
                    tc->SetRegionName(region);
                if (teleporterRegion != "")
                    tc->SetTeleporterRegionName(teleporterRegion);
                return true;
            }
        }
    }
    return false;
}
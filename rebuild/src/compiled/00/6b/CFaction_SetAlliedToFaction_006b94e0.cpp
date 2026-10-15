// CFaction::SetAlliedToFaction(const CCharString&) -- marks the named faction as allied: removes it from the enemy set and adds it to the allied set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B94E0, length 42 bytes (the 48-byte catalog row ends in int3 padding).
// Callees: std::set<CCharString>::erase(const CCharString&) @ 0x004BB270 (returns the erased count, dropped),
//          std::set<CCharString>::insert(const CCharString&) @ 0x004AC380 (pair<iterator,bool> returned
//          through the hidden pointer into an 8-byte frame temporary, dropped).
// Callers: CGameScriptInterface::SetFactionAs*Faction (0x00890870 family) call it on both factions with the
//          other faction (its Name sits at +0, so the reference is the faction pointer itself).
// Label: the manifest's "CTCInventory::Copy" is propagated over byte-identical bodies; the callers and the
//        CFaction PDB layout (Name +0, AlliedFactions +4, EnemyFactions after it) identify the class.
// Layout: CFaction from Ego_r.pdb is 36 bytes with 16-byte sets; the shipped VC6-era set is 12 bytes,
//         so EnemyFactions sits at +0x10 (PDB +0x14). No engine header exists for CFaction.
// Shape: the pair result needs a user constructor so it comes back through the hidden pointer.

struct CCharString {
    char* m_pData;                                // +0x00
};

struct SSetIterator {
    void* m_pNode;
};

struct SSetInsertResult {                         // pair<iterator, bool>
    SSetIterator first;
    bool         second;
    SSetInsertResult() { }
};

struct CCharStringSet {                           // std::set<CCharString> (VC6-era _Tree, 12 bytes)
    unsigned char m_comp[4];                      // +0x00 less<> (empty, padded)
    void*         m_pHead;                        // +0x04
    unsigned long m_size;                         // +0x08
    unsigned long erase(const CCharString& key);              // 0x004BB270
    SSetInsertResult insert(const CCharString& value);        // 0x004AC380
};

struct CFaction {
    CCharString    Name;                          // +0x00
    CCharStringSet AlliedFactions;                // +0x04
    CCharStringSet EnemyFactions;                 // +0x10
    void SetAlliedToFaction(const CCharString& factionName);
};

void CFaction::SetAlliedToFaction(const CCharString& factionName)
{
    EnemyFactions.erase(factionName);
    AlliedFactions.insert(factionName);
}
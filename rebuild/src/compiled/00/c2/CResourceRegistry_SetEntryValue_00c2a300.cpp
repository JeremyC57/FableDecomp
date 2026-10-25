// CResourceRegistry::SetEntryValue(unsigned long key, long value) -- look the key up in the entry list (+0x68) and, when an entry is found, store the value in its +0x160 word
// Leaf: SetEntryValue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A300, length 32 bytes (31 + int3 padding), `ret 8`.
// Caller: the owner's locked forwarder @ 0x00C148C0 (landed "CPersistContext_00c148c0::Transfer": `m_pTarget->...(a, b)` under
//   the module lock). Callee: the entry list's virtual Find (slot 17).
// Layout: the registry of the landed ReleaseByKey @ 0x00C2A6D0 (same TU): an embedded polymorphic keyed list at +0x68
//   (entries by key) and a second one at +0x98 (entries by owner), both searched through virtual slot 17 (+0x44,
//   Find(const key&) -> entry or null). No PDB covers libpk: the names are descriptive.
// Shape: the key's address is its own incoming stack slot (`lea edx,[esp+4]`), the value read only on the found path.
struct CResourceEntry {                      // a registry entry (owner word +0x00)
    unsigned char _pad_0x000[0x160];
    long          m_value;                  // +0x160
};

struct CRegistryMap {                       // libpk keyed list (polymorphic)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual CResourceEntry* Find(const unsigned long& key);   // slot 17 (+0x44)
};

struct CResourceRegistry {
    unsigned char _pad_0x00[0x68];
    CRegistryMap  m_entries;                // +0x68 entries by key
    unsigned char _pad_0x6c[0x2c];
    CRegistryMap  m_byOwner;                // +0x98 entries by owner
    void ReleaseEntry(CResourceEntry* entry);                  // 0x00C2A5C0
    void SetEntryValue(unsigned long key, long value);
    bool HasEntry(unsigned long key);
    void ReleaseOwnerEntries(void* owner);
};

void CResourceRegistry::SetEntryValue(unsigned long key, long value)
{
    CResourceEntry* entry = m_entries.Find(key);
    if (entry)
        entry->m_value = value;
}
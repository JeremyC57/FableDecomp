// CBlendOwner_00c1e7d0::AddCombinationBlend(CBlendEntry*) -- files the entry into one of two member lists (+8 or +0x14) depending on whether the entry's key (+8) resolves to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E7D0, length 38 bytes.
// Callees: CBlendKey::GetIndex @ 0x00C1BA90 (on &entry->m_key, returns long); CBlendList::Add @ 0x00C1E900 (thiscall, one arg).
// The manifest labels (C3DAnimationBlendState::AddCombinationBlend; the callees' CManager::GetKeyPressed
// and EHeroMorphType_FindAndRemoveEntry) are propagated names and do not fit the bytes; the
// classes are named by what the body does. Written as if/else: VC hoists the common `push`
// of the argument above the branch and merges the two Add calls, leaving only the `lea` of
// the list selected (a conditional expression on the list reference pushes after the select).
struct CBlendKey {
    long GetIndex();                       // 0x00C1BA90
};

struct CBlendEntry {
    unsigned char _pad[0x8];
    CBlendKey m_key;                       // +8
};

struct CBlendList {
    unsigned char _body[0xc];
    void Add(CBlendEntry* entry);          // 0x00C1E900
};

struct CBlendOwner_00c1e7d0 {
    unsigned char _pad[0x8];
    CBlendList m_unkeyed;                  // +8
    CBlendList m_keyed;                    // +0x14
    void AddCombinationBlend(CBlendEntry* entry);
};

void CBlendOwner_00c1e7d0::AddCombinationBlend(CBlendEntry* entry)
{
    if (entry->m_key.GetIndex() == -1)
        m_unkeyed.Add(entry);
    else
        m_keyed.Add(entry);
}
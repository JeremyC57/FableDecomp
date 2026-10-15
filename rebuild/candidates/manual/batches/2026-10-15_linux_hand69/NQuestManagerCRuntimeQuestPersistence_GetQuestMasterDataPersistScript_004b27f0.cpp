// NQuestManager::CRuntimeQuestPersistence::GetQuestMasterDataPersistScript(const CCharString&) const -- the master-data persist script recorded for the quest, or null when the quest has none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B27F0, length 82 bytes.
// Callees: CVectorMap<CCharString, CPersistInfo>::LowerBound(const CCharString&) @ 0x004B97D0,
// CBasicString<char>::operator< @ 0x00429950 (on the string blocks, the inlined CCharString <).
// The manifest label "CQuestManager::LookupPersistentQuestInfo" is a guess and is rejected: `this` holds the
// sorted map at +0x10 -- PDB CRuntimeQuestPersistence {PersistScripts +0, MasterDataPersistScripts +0x14}, the
// shipped CVectorMap being 16 bytes, so MasterDataPersistScripts sits at +0x10 (the sibling
// SetQuestMasterDataPersistScript @ 0x004B4F30, called by CQuestManager on its member at +0xc, uses the same
// +0x10 map); FableWin.pdb names ?GetQuestMasterDataPersistScript@CRuntimeQuestPersistence@NQuestManager@@QBEPBVCCharString@@ABV3@@Z.
// The element is pair<CCharString, CPersistInfo {QuestName, PersistScript}>: the result is &second.PersistScript (+8).
// Shape: the map's inline find (`(it == last || key < it->first) ? last : it` with `last` a local read once into
// esi after the call) and the caller's own `it != end()` test re-reading +0x14, the found return first;
// CCharString < compares the blocks (equal -> false, null key -> true, null element -> false, else the block compare).
struct CCharStringData {
    bool operator<(const CCharStringData& other) const;   // 0x00429950 (CBasicString<char>::operator<)
};

struct CCharString {
    CCharStringData* PStringData;
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};

namespace NQuestManager {

struct CRuntimeQuestPersistence {
    struct CPersistInfo {
        CCharString QuestName;                     // +0x00
        CCharString PersistScript;                 // +0x04
    };
    struct SPair {
        CCharString  first;                        // +0x00
        CPersistInfo second;                       // +0x04
    };
    struct CPersistMap {                           // CVectorMap<CCharString, CPersistInfo>: a sorted vector + 1 word
        SPair* m_pFirst;
        SPair* m_pLast;
        SPair* m_pEnd;
        long   m_sorted;
        SPair* LowerBound(const CCharString& key);   // 0x004B97D0
        SPair* Find(const CCharString& key)
        {
            SPair* it = LowerBound(key);
            SPair* last = m_pLast;
            return (it == last || key < it->first) ? last : it;
        }
    };
    CPersistMap PersistScripts;                    // +0x00
    CPersistMap MasterDataPersistScripts;          // +0x10
    const CCharString* GetQuestMasterDataPersistScript(const CCharString& quest);
};

const CCharString* CRuntimeQuestPersistence::GetQuestMasterDataPersistScript(const CCharString& quest)
{
    SPair* it = MasterDataPersistScripts.Find(quest);
    if (it != MasterDataPersistScripts.m_pLast)
        return &it->second.PersistScript;
    return 0;
}

}

// NQuestManager::CRuntimeQuestPersistence::SetQuestScriptPersistScript(const CCharString& quest, const CCharString& script) -- record the persist script for the quest in the quest-script persist map: overwrite the entry when the quest is present, else add {quest, script} and re-sort the map
// Leaf: SetQuestScriptPersistScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B4E30, length 256 bytes (`ret 8`).
// FableWin.pdb: ?SetQuestScriptPersistScript@CRuntimeQuestPersistence@NQuestManager@@QAEXABVCCharString@@0@Z (the manifest gives both rows the
// SetQuestMasterDataPersistScript label; the +0 map row 0x004B4E30 is SetQuestScriptPersistScript, the +0x10 one 0x004B4F30).
// Callees: CVectorMap<CCharString, CPersistInfo>::LowerBound @ 0x004B9750, CBasicString<char>::operator< @ 0x00429950 (the
// inline CCharString <), CCharString copy ctor @ 0x0099EC30 / operator= @ 0x0099EFB0 / dtor @ 0x0099EAE0, the pair vector's
// push_back @ 0x004BB780, SGI sort<pair*, CKeyPairCompareLess> @ 0x004BBB40 (`__fastcall`, the empty comparator by value).
// Layout: the landed GetQuestMasterDataPersistScript 0x004B27F0 model (PersistScripts +0, MasterDataPersistScripts +0x10;
// each CVectorMap = 12-byte STLport vector + Compare +0xc + Dirty +0xd).
// Shape: the map's inline Find (`last` read once after LowerBound), then a CPersistInfo local built from the arguments; a hit
// assigns it through the inline memberwise operator=, a miss runs the inline Add (pair temporary in its own block, Dirty =
// true) and the inline SortIfDirty (the test folds, the true store stays before the sort call, false after).
class CCharStringData { public:
    bool operator<(const CCharStringData& other) const;   // 0x00429950 (CBasicString<char>::operator<)
};

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
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
    class CPersistInfo { public:
        CCharString QuestName;                     // +0x00
        CCharString PersistScript;                 // +0x04
        CPersistInfo(const CCharString& questName, const CCharString& persistScript)
            : QuestName(questName), PersistScript(persistScript) { }
        __forceinline CPersistInfo(const CPersistInfo& other) : QuestName(other.QuestName), PersistScript(other.PersistScript) { }
        __forceinline CPersistInfo& operator=(const CPersistInfo& other)
        {
            QuestName = other.QuestName;
            PersistScript = other.PersistScript;
            return *this;
        }
    };
    struct SPair {                                 // pair<CCharString, CPersistInfo>, 12 bytes
        CCharString  first;                        // +0x00
        CPersistInfo second;                       // +0x04
        __forceinline SPair(const CCharString& key, const CPersistInfo& value) : first(key), second(value) { }
    };
    struct CKeyPairCompareLess { };                // empty comparator (1 byte)
    struct CPersistMap {                           // CVectorMap<CCharString, CPersistInfo>: STLport vector, Compare, Dirty
        SPair*              m_pFirst;              // +0x00
        SPair*              m_pLast;               // +0x04
        SPair*              m_pEnd;                // +0x08
        CKeyPairCompareLess Compare;               // +0x0c
        bool                Dirty;                 // +0x0d
        SPair* LowerBound(const CCharString& key);   // 0x004B9750
        void   push_back(const SPair& value);        // 0x004BB780 (STLport vector::push_back)
        SPair* end() { return m_pLast; }
        SPair* Find(const CCharString& key)
        {
            SPair* it = LowerBound(key);
            SPair* last = m_pLast;
            return (it == last || key < it->first) ? last : it;
        }
        __forceinline void Add(const CCharString& key, const CPersistInfo& value)
        {
            {
                SPair pair(key, value);
                push_back(pair);
            }
            Dirty = true;
            SortIfDirty();
        }
        __forceinline void SortIfDirty();
    };
    CPersistMap PersistScripts;                    // +0x00
    CPersistMap MasterDataPersistScripts;          // +0x10
    void SetQuestScriptPersistScript(const CCharString& quest, const CCharString& script);
    void SetQuestMasterDataPersistScript(const CCharString& quest, const CCharString& script);
};

void __fastcall sort(CRuntimeQuestPersistence::SPair* first, CRuntimeQuestPersistence::SPair* last,
                     CRuntimeQuestPersistence::CKeyPairCompareLess comp);   // 0x004BBB40 (SGI sort, /Gr)

__forceinline void CRuntimeQuestPersistence::CPersistMap::SortIfDirty()
{
    if (Dirty) {
        sort(m_pFirst, m_pLast, Compare);
        Dirty = false;
    }
}

void CRuntimeQuestPersistence::SetQuestScriptPersistScript(const CCharString& quest, const CCharString& script)
{
    SPair* it = PersistScripts.Find(quest);
    CPersistInfo info(quest, script);
    if (it != PersistScripts.end())
        it->second = info;
    else
        PersistScripts.Add(quest, info);
}

}
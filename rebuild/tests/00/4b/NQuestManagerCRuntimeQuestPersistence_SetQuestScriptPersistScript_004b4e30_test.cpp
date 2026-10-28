#include <cstdio>
#include <cstring>
// Behaviour model of NQuestManager::CRuntimeQuestPersistence::SetQuestScriptPersistScript @ 0x004B4E30
// (copy of the source's logic; CCharString stand-in over a fixed buffer, sorted-vector stand-ins).
static int g_sorts = 0;
class CCharStringData { public: char text[32]; bool operator<(const CCharStringData& o) const { return std::strcmp(text, o.text) < 0; } };
static CCharStringData g_pool[64]; static int g_used = 0;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* s) { PStringData = &g_pool[g_used++]; std::strcpy(PStringData->text, s); }
    CCharString(const CCharString& o) { PStringData = &g_pool[g_used++]; *PStringData = *o.PStringData; }
    ~CCharString() {}
    const CCharString& operator=(const CCharString& o) { *PStringData = *o.PStringData; return *this; }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (PStringData == 0) return true;
        if (other.PStringData == 0) return false;
        return *PStringData < *other.PStringData;
    }
};
struct CPersistInfo {
    CCharString QuestName, PersistScript;
    CPersistInfo(const CCharString& q, const CCharString& s) : QuestName(q), PersistScript(s) {}
    CPersistInfo(const CPersistInfo& o) : QuestName(o.QuestName), PersistScript(o.PersistScript) {}
    CPersistInfo& operator=(const CPersistInfo& o) { QuestName = o.QuestName; PersistScript = o.PersistScript; return *this; }
};
struct SPair { CCharString first; CPersistInfo second; SPair(const CCharString& k, const CPersistInfo& v) : first(k), second(v) {} };
struct CPersistMap {
    SPair* items[16]; int count; bool Dirty;
    SPair** begin() { return items; }
    SPair* end() { return 0; }                        // stand-in: null = end
    SPair* LowerBound(const CCharString& key)
    { for (int i = 0; i < count; ++i) if (!(items[i]->first < key)) return items[i]; return end(); }
    void push_back(const SPair& v) { items[count++] = new SPair(v); }
    SPair* Find(const CCharString& key)
    {
        SPair* it = LowerBound(key);
        SPair* last = end();
        return (it == last || key < it->first) ? last : it;
    }
    void SortIfDirty()
    {
        if (Dirty) {
            ++g_sorts;
            for (int i = 1; i < count; ++i)
                for (int j = i; j > 0 && items[j]->first < items[j - 1]->first; --j) { SPair* t = items[j]; items[j] = items[j - 1]; items[j - 1] = t; }
            Dirty = false;
        }
    }
    void Add(const CCharString& key, const CPersistInfo& value)
    {
        { SPair pair(key, value); push_back(pair); }
        Dirty = true;
        SortIfDirty();
    }
};
struct CRuntimeQuestPersistence {
    CPersistMap PersistScripts;
    void SetQuestScriptPersistScript(const CCharString& quest, const CCharString& script)
    {
        SPair* it = PersistScripts.Find(quest);
        CPersistInfo info(quest, script);
        if (it != PersistScripts.end())
            it->second = info;
        else
            PersistScripts.Add(quest, info);
    }
};
int main()
{
    static CRuntimeQuestPersistence p; p.PersistScripts.count = 0; p.PersistScripts.Dirty = false;
    p.SetQuestScriptPersistScript(CCharString("Q_Wasp"), CCharString("wasp.lua"));
    p.SetQuestScriptPersistScript(CCharString("Q_Arena"), CCharString("arena.lua"));
    p.SetQuestScriptPersistScript(CCharString("Q_Wasp"), CCharString("wasp2.lua"));
    CPersistMap& m = p.PersistScripts;
    int ok = m.count == 2 && g_sorts == 2 && !m.Dirty
          && std::strcmp(m.items[0]->first.PStringData->text, "Q_Arena") == 0
          && std::strcmp(m.items[1]->second.PersistScript.PStringData->text, "wasp2.lua") == 0
          && std::strcmp(m.items[1]->second.QuestName.PStringData->text, "Q_Wasp") == 0;
    for (int i = 0; i < m.count; ++i) delete m.items[i];
    if (!ok) { std::printf("004b4e30_TEST FAIL\n"); return 1; }
    std::printf("004b4e30_TEST PASS\n");
    return 0;
}
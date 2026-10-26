// Behaviour test for 0x009ad2e0 CFactionDatabase::FindFactionBySymbolName
#include <cstdio>
#include <cstring>
static unsigned int Crc(const char* s, unsigned int n) { unsigned int h = 0; for (unsigned int i = 0; i < n; ++i) h = h * 31 + (unsigned char)s[i]; return h; }
class CCharStringData { public: const char* m_pChars; unsigned int m_length; };
class CCharString { public:
    CCharStringData* m_pStringData;
    unsigned int GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? "" : m_pStringData->m_pChars; }
};
struct CCountedPointer_CFaction { int faction; int count; };
struct CFactionMap {
    struct SEntry { unsigned int key; CCountedPointer_CFaction second; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const unsigned int* key) { SEntry* it = first; while (it != last && it->key < *key) ++it; return it; }
    SEntry* end() const { return last; }
    SEntry* Find(unsigned int key) { unsigned int k = key; SEntry* it = LowerBound(&k); if (it == last || key < it->key) it = last; return it; }
};
struct CFactionDatabase {
    CFactionMap m_factions;
    CCountedPointer_CFaction* FindFactionBySymbolName(const CCharString& name)
    {
        CFactionMap::SEntry* it = m_factions.Find(Crc(name.GetChars(), name.GetLength()));
        if (it == m_factions.end()) return 0;
        return &it->second;
    }
};
int main()
{
    const char* names[3] = { "FACTION_BANDITS", "FACTION_GUARDS", "FACTION_HERO" };
    CFactionMap::SEntry e[3];
    for (int i = 0; i < 3; ++i) { e[i].key = Crc(names[i], (unsigned int)std::strlen(names[i])); e[i].second.faction = i + 1; }
    for (int i = 0; i < 3; ++i) for (int j = i + 1; j < 3; ++j) if (e[j].key < e[i].key) { CFactionMap::SEntry t = e[i]; e[i] = e[j]; e[j] = t; }
    CFactionDatabase db; db.m_factions.first = e; db.m_factions.last = e + 3;
    CCharStringData d = { "FACTION_GUARDS", 14 }; CCharString s = { &d };
    CCharStringData d2 = { "FACTION_NONE", 12 }; CCharString s2 = { &d2 };
    CCharString empty = { 0 };
    CCountedPointer_CFaction* f = db.FindFactionBySymbolName(s);
    bool ok = f && f->faction == 2 && db.FindFactionBySymbolName(s2) == 0 && db.FindFactionBySymbolName(empty) == 0;
    if (!ok) { std::printf("009ad2e0_TEST FAIL\n"); return 1; }
    std::printf("009ad2e0_TEST PASS\n");
    return 0;
}
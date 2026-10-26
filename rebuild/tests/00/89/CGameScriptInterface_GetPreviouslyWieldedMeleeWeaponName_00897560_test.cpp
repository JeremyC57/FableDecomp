// Behaviour test for 0x00897560 CGameScriptInterface::GetPreviouslyWieldedMeleeWeaponName: hero/TC/index guards,
// def-name lookup through the definition manager and string table, empty-name result.
#include <cstdio>
#include <cstring>
class CCharString {
public:
    char text[32];
    CCharString() { text[0] = 0; }
    CCharString(const char* t) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
    CCharString& operator=(const CCharString& o) { std::strcpy(text, o.text); return *this; }
    bool operator!=(const char* t) const { return std::strcmp(text, t) != 0; }
};
struct CDefStringTable {
    CCharString GetString(long index) { return CCharString(index == 3 ? "OBJECT_IRON_LONGSWORD" : ""); }
};
static CDefStringTable g_DefStringTable;
class CDefString { public:
    long Index;
    CCharString GetString() const { return g_DefStringTable.GetString(Index); }
};
class CDefinitionManager { public:
    CDefString GetDefNameFromGlobalIndex(long index) { CDefString d; d.Index = index == 77 ? 3 : 4; return d; }
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    virtual ~CThing() {}
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_dead : 1;
    unsigned char m_91_1 : 7;
    CInterfaceMap::SEntry m_entries[8];
    CThing() : m_dead(0), m_91_1(0) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = m_entries; }
    void Add(long type, void* tc)   // keep sorted
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type; it->component = (CTCBase*)tc; ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> T* GetTC(long interfaceType)
    {
        T* tc = 0;
        GetInterface(&tc, interfaceType);
        return tc;
    }
};
struct CTCHeroWeaponHistory { long PreviouslyWieldedMeleeWeapon; };
class CPlayer { public: CThing* hero; CThing* GetHeroThing() { return hero; } };
class CPlayerManager { public: CPlayer* player; CPlayer* GetMainPlayer() { return player; } };
class CGameScriptInterface { public:
    CDefinitionManager* DefinitionManager;
    CPlayerManager* PlayerManager;
    bool GetPreviouslyWieldedMeleeWeaponName(CCharString& name) const;
};
bool CGameScriptInterface::GetPreviouslyWieldedMeleeWeaponName(CCharString& name) const
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroThing();
    CTCHeroWeaponHistory* history;
    if (hero && !hero->m_dead && hero->GetInterface(&history, 0x13)) {
        long weapon = history->PreviouslyWieldedMeleeWeapon;
        if (weapon > 0) {
            name = DefinitionManager->GetDefNameFromGlobalIndex(weapon).GetString();
            if (name != "")
                return true;
        }
    }
    return false;
}
int main()
{
    CThing hero; CTCHeroWeaponHistory hist = { 0 };
    CPlayer player = { 0 }; CPlayerManager pm = { &player }; CDefinitionManager dm;
    CGameScriptInterface gsi = { &dm, &pm };
    CCharString name("unchanged");
    int fail = 0;
    if (gsi.GetPreviouslyWieldedMeleeWeaponName(name)) fail |= 1;             // no hero
    player.hero = &hero;
    if (gsi.GetPreviouslyWieldedMeleeWeaponName(name)) fail |= 2;             // no TC 0x13
    hero.Add(0x13, &hist); hero.Add(0x46, 0);
    if (gsi.GetPreviouslyWieldedMeleeWeaponName(name) || std::strcmp(name.text, "unchanged")) fail |= 4;   // index 0
    hist.PreviouslyWieldedMeleeWeapon = 12;                                   // a def with an empty name
    if (gsi.GetPreviouslyWieldedMeleeWeaponName(name) || name.text[0]) fail |= 8;
    hist.PreviouslyWieldedMeleeWeapon = 77;
    if (!gsi.GetPreviouslyWieldedMeleeWeaponName(name) || std::strcmp(name.text, "OBJECT_IRON_LONGSWORD")) fail |= 16;
    hero.m_dead = 1;
    if (gsi.GetPreviouslyWieldedMeleeWeaponName(name)) fail |= 32;
    if (fail) { std::printf("00897560_TEST FAIL\n"); return 1; }
    std::printf("00897560_TEST PASS\n");
    return 0;
}
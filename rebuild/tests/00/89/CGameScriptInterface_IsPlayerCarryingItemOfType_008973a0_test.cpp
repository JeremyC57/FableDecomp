#include <stdio.h>
#include <string.h>
// Behaviour test for CGameScriptInterface::IsPlayerCarryingItemOfType (0x008973A0): false without a live hero that has
// the carrying component; an empty name (unassigned or "") asks only whether anything is carried; otherwise the carried
// thing's definition name must match.
#pragma pack(push, 1)
struct CBasicStringChar {
    char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1;
    bool operator==(const char* text) const { return strcmp(PData, text) == 0; }
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
class CCharString { public: CCharStringData* PStringData; };
struct CDefString {
    long Index;
    bool operator==(const CCharString& name) const;
};
static const char* g_defNames[] = { "OBJECT_APPLE", "OBJECT_SWORD" };
bool CDefString::operator==(const CCharString& name) const { return name.PStringData != 0 && strcmp(g_defNames[Index], name.PStringData->Data.PData) == 0; }
struct CTCBase { };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing;
struct CTCCarrying : CTCBase { CThing* Carried; CThing* GetThingInPrimarySlot() { return Carried; } };
struct CThing {
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x45];
    unsigned char m_dead : 1;
    unsigned char m_91_1 : 7;
    long DefIndex;
    CDefString GetDefName() const { CDefString d; d.Index = DefIndex; return d; }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
class CPlayer { public: CThing* Hero; CThing* GetHeroThing() { return Hero; } };
struct CPlayerManager { CPlayer Main; CPlayer& GetMainPlayer() { return Main; } };
struct CGameScriptInterface {
    void* World; void* Component; void* DisplayEngine; void* DefinitionManager; CPlayerManager* PlayerManager;
    bool IsPlayerCarryingItemOfType(const CCharString& defName) const;
};
bool CGameScriptInterface::IsPlayerCarryingItemOfType(const CCharString& defName) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetHeroThing();
    if (hero && !hero->m_dead && hero->HasTC(0x46)) {
        CThing* item = hero->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (defName.PStringData == 0) {
            if (strcmp("", "") == 0)
                goto empty;
        } else if (defName.PStringData->Data == "")
            goto empty;
        return item && item->GetDefName() == defName;
empty:
        return item != 0;
    }
    return false;
}
int main()
{
    int ok = 1;
    CThing hero; memset(&hero, 0, sizeof(hero)); CThing apple; memset(&apple, 0, sizeof(apple)); apple.DefIndex = 0;
    CTCCarrying carrying; carrying.Carried = 0;
    CInterfaceMap::SEntry entries[2]; entries[0].type = 0x10; entries[0].component = 0; entries[1].type = 0x46; entries[1].component = &carrying;
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    CPlayerManager pm; pm.Main.Hero = &hero;
    CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    char appleText[] = "OBJECT_APPLE", swordText[] = "OBJECT_SWORD", emptyText[] = "";
    CCharStringData appleData, swordData, emptyData; appleData.Data.PData = appleText; swordData.Data.PData = swordText; emptyData.Data.PData = emptyText;
    CCharString none; none.PStringData = 0;
    CCharString appleName; appleName.PStringData = &appleData;
    CCharString swordName; swordName.PStringData = &swordData;
    CCharString emptyName; emptyName.PStringData = &emptyData;
    if (gsi.IsPlayerCarryingItemOfType(none)) ok = 0;                         // no carrying component
    hero.m_presentBits[2] = 1UL << 6;
    if (gsi.IsPlayerCarryingItemOfType(none) || gsi.IsPlayerCarryingItemOfType(appleName)) ok = 0;   // carrying nothing
    carrying.Carried = &apple;
    if (!gsi.IsPlayerCarryingItemOfType(none) || !gsi.IsPlayerCarryingItemOfType(emptyName)) ok = 0;  // anything carried
    if (!gsi.IsPlayerCarryingItemOfType(appleName) || gsi.IsPlayerCarryingItemOfType(swordName)) ok = 0;
    hero.m_dead = 1;
    if (gsi.IsPlayerCarryingItemOfType(none) || gsi.IsPlayerCarryingItemOfType(appleName)) ok = 0;
    hero.m_dead = 0; pm.Main.Hero = 0;
    if (gsi.IsPlayerCarryingItemOfType(none)) ok = 0;
    if (ok) { printf("008973a0_TEST PASS\n"); return 0; }
    printf("008973a0_TEST FAIL\n"); return 1;
}
// Behaviour test for 0x008CB620 CCreatureAction_SummonerUnsheatheStrike::Initialise: runs the reconstructed body over stubbed
// callees; checks the base Initialise, the SUMMONER_ATTACK_STYLE lookup, the combat setter receiving the def and the release.
#include <cstdio>
#include <cstring>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
struct CThing {
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
static int g_strings = 0;
struct CCharString { const char* m_pData; CCharString(const char* t, long = -1) : m_pData(t) { ++g_strings; } ~CCharString() { --g_strings; } };
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
static int g_destroyed = 0;
struct CDefVirtual { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } };
struct CAttackStyleDef : CDefVirtual, CRefCounted { void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); } };
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};
static CAttackStyleDef g_def; static char g_name[32];
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer<CAttackStyleDef>* def) { std::strcpy(g_name, name.m_pData); def->m_p = &g_def; ++g_def.m_count; return true; }
};
static CAttackStyleDef* g_set = 0; static long g_countAtSet = 0;
struct CTCCombat { void SetAttackStyleDef(const CDefPointer<CAttackStyleDef>* def) { g_set = def->m_p; g_countAtSet = def->m_p->m_count; } };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static CDefinitionManager g_manager; static int g_base = 0;
class CCreatureAction_UnsheatheWeapons {
public:
    virtual void Initialise() { ++g_base; }
    CBaseIntelligentPointer PCreature;
    CDefinitionManager* GetPDefinitionManager() const { return &g_manager; }
    CTCCombat* GetCombat() const { CTCCombat* combat = 0; PCreature.GetPItem()->GetInterface(&combat, 0x30); return combat; }
};
class CCreatureAction_SummonerUnsheatheStrike : public CCreatureAction_UnsheatheWeapons { public: virtual void Initialise(); };
void CCreatureAction_SummonerUnsheatheStrike::Initialise()
{
    CCreatureAction_UnsheatheWeapons::Initialise();
    CTCCombat* combat = GetCombat();
    CDefPointer<CAttackStyleDef> def;
    {
        CCharString name("SUMMONER_ATTACK_STYLE", -1);
        GetPDefinitionManager()->GetDef(name, &def);
    }
    combat->SetAttackStyleDef(&def);
}
int main()
{
    CTCCombat combat; CInterfaceMap::SEntry e[1] = { { 0x30, (CTCBase*)&combat } };
    CThing owner = CThing(); owner.m_presentBits[1] = 1UL << (0x30 & 31); owner.m_interfaces.first = e; owner.m_interfaces.last = e + 1;
    CCreatureAction_SummonerUnsheatheStrike a; a.PCreature.m_pItem = &owner;
    g_def.m_count = 0;
    a.Initialise();
    bool ok = g_base == 1 && std::strcmp(g_name, "SUMMONER_ATTACK_STYLE") == 0 && g_strings == 0 && g_set == &g_def &&
              g_countAtSet == 1 && g_def.m_count == 0 && g_destroyed == 1;
    if (!ok) { std::printf("008cb620_TEST FAIL\n"); return 1; }
    std::printf("008cb620_TEST PASS\n");
    return 0;
}

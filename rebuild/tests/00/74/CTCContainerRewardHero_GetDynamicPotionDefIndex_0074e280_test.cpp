// Behaviour test for 0074e280: src/0074e280.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCContainerRewardHero::GetDynamicPotionDefIndex()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCContainerRewardHero::GetDynamicPotionDefIndex() -- pick the potion a reward container should give the living hero: the health or mana potion, by whichever stat is relatively lower, else by which kind he carries fewer of
// Retail 0x0074E280, 480 bytes (plain `ret`, `long` result: 0 without a living hero carrying an inventory).
// Callees: CTCBase::GetPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
// CPlayer::GetHeroCreature @ 0x00487DC0, CVectorMap LowerBound const @ 0x004365B0 (inlined const GetTC: inventory 0x11,
// hero stats 4), CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CGameDefinitionManager::Get
// @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410,
// CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08. Float constant 0x0129BA3C = 0.0001f.
// Layout: retail CThing MaxHealth +0xb0 / Health +0xb4 (as the landed CQuestInfoElementBar::Update 0x006463FF);
// CTCHeroStats retail +0x58/+0x5c are the PDB's RewardMoney/RentMoney slots -- read here as the will (mana) pair
// (named WillPower / MaxWillPower; Ego_r.pdb CTCHeroStats is not used: its early layout differs from retail).
// Shape: each def index comes from a `const char*` converted to a CCharString temporary (built before Get()); a super
// potion counts double; a hero-stats ratio difference under 0.0001 falls back to the potion counts, written as its own
// `if (fabsf(...) < 0.0001f) { counts }` block inside the stats branch (VC7.1 tail-merges it with the no-stats fallback and
// lays the counts path out first, which also gives retail's frame-slot assignment; a negated test lays the ratio first).
#include <math.h>
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                         // +0x91 bit 0
    unsigned char _pad92[0xb0 - 0x92];
    float         MaxHealth;                        // +0xb0
    float         Health;                           // +0xb4
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CPlayer { public:
    CThing* GetHeroCreature();                      // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;                 // 0x00449960
};
struct CTCInventoryBase {
    long GetNumberOfItemsOfTypeInInventory(long defIndex);   // 0x005BDF08
};
class CTCHeroStats { public:
    unsigned char _pad00[0x58];
    long          WillPower;                        // +0x58
    long          MaxWillPower;                     // +0x5c
};

class CTCContainerRewardHero { public:
    void*   __vftable;                              // +0x00
    CThing* Thing;                                  // +0x04 CTCBase parent thing
    const CPlayerManager* GetPlayerManager() const; // 0x00686D50
    long GetDynamicPotionDefIndex();
};

long CTCContainerRewardHero::GetDynamicPotionDefIndex()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    CTCInventoryBase* inventory;
    if (!hero || hero->Dead || !hero->GetInterface(&inventory, 0x11))
        return 0;
    long health = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_HEALTH_POTION");
    long healthCount = inventory->GetNumberOfItemsOfTypeInInventory(health);
    long superHealth = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_SUPER_HEALTH_POTION");
    healthCount += 2 * inventory->GetNumberOfItemsOfTypeInInventory(superHealth);
    long mana = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_MANA_POTION");
    long manaCount = inventory->GetNumberOfItemsOfTypeInInventory(mana);
    long superMana = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_SUPER_MANA_POTION");
    manaCount += 2 * inventory->GetNumberOfItemsOfTypeInInventory(superMana);
    const CTCHeroStats* stats;
    if (hero->GetInterface(&stats, 4)) {
        float healthRatio = hero->Health / hero->MaxHealth;
        float willRatio = (float)stats->WillPower / stats->MaxWillPower;
        if (fabsf(healthRatio - willRatio) < 0.0001f) {
            if (healthCount > manaCount)
                return mana;
            return health;
        }
        if (healthRatio < willRatio)
            return health;
        return mana;
    }
    if (healthCount > manaCount)
        return mana;
    return health;
}

// ---- stubs ----
static int g_strings = 0, g_strDtors = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; ++g_strings; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtors; }
static CGameDefinitionManager g_defs;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_defs; }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name)
{
    const char* n = (const char*)name.PStringData;
    if (strcmp(n, "OBJECT_HEALTH_POTION") == 0) return 10;
    if (strcmp(n, "OBJECT_SUPER_HEALTH_POTION") == 0) return 11;
    if (strcmp(n, "OBJECT_MANA_POTION") == 0) return 20;
    if (strcmp(n, "OBJECT_SUPER_MANA_POTION") == 0) return 21;
    return -1;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* e = first;
    while (e != last && e->type < *type) ++e;
    return e;
}
static long g_counts[32];
long CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long defIndex) { return (defIndex >= 0 && defIndex < 32) ? g_counts[defIndex] : 0; }
static CThing* g_hero = 0;
CThing* CPlayer::GetHeroCreature() { return g_hero; }
static CPlayer g_player;
CPlayer* CPlayerManager::GetMainPlayer() const { return &g_player; }
static CPlayerManager g_pm;
const CPlayerManager* CTCContainerRewardHero::GetPlayerManager() const { return &g_pm; }
int main()
{
    static long herobuf[0x40]; CThing& hero = *(CThing*)herobuf;
    static CTCInventoryBase inv; static long statsbuf[0x20]; CTCHeroStats& stats = *(CTCHeroStats*)statsbuf;
    static CInterfaceMap::SEntry entries[3];
    entries[0].type = 4; entries[0].component = (CTCBase*)&stats;
    entries[1].type = 0x11; entries[1].component = (CTCBase*)&inv;
    entries[2].type = 0x30; entries[2].component = 0;
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 3;
    CTCContainerRewardHero c; c.Thing = 0;
    // no hero / dead hero / no inventory: 0
    g_hero = 0; CHECK(c.GetDynamicPotionDefIndex() == 0);
    g_hero = &hero; hero.Dead = 1; CHECK(c.GetDynamicPotionDefIndex() == 0);
    hero.Dead = 0; hero.m_presentBits[0] = 0; CHECK(c.GetDynamicPotionDefIndex() == 0);
    // inventory, no stats: by counts (health 1 + 2*1 = 3 v mana 2 + 2*0 = 2 -> mana)
    hero.m_presentBits[0] = 1UL << 0x11;
    g_counts[10] = 1; g_counts[11] = 1; g_counts[20] = 2; g_counts[21] = 0;
    g_strings = g_strDtors = 0;
    CHECK(c.GetDynamicPotionDefIndex() == 20); CHECK(g_strings == 4 && g_strDtors == 4);
    g_counts[20] = 3; CHECK(c.GetDynamicPotionDefIndex() == 10);   // equal counts -> health
    g_counts[21] = 1; CHECK(c.GetDynamicPotionDefIndex() == 10);   // health 3 is not more than mana 5 -> health
    // with stats: the lower relative stat wins
    hero.m_presentBits[0] |= 1UL << 4;
    hero.MaxHealth = 100.0f; hero.Health = 50.0f; stats.WillPower = 80; stats.MaxWillPower = 100;
    CHECK(c.GetDynamicPotionDefIndex() == 10);
    hero.Health = 90.0f; stats.WillPower = 30;
    CHECK(c.GetDynamicPotionDefIndex() == 20);
    // equal ratios fall back to the counts: health 3 v mana 3 + 2 = 5 -> health
    hero.Health = 50.0f; stats.WillPower = 50;
    CHECK(c.GetDynamicPotionDefIndex() == 10);
    g_counts[21] = 0; g_counts[20] = 1; CHECK(c.GetDynamicPotionDefIndex() == 20);   // 3 > 1 -> mana
    if (fails == 0) printf("0074e280_TEST PASS\n"); else printf("0074e280_TEST FAIL\n");
    return fails;
}
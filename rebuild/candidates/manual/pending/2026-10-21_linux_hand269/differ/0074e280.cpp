// Grade: DIFFER(469v469) -- frame-slot assignment only: retail puts the inventory out at esp+0x14/key at +0x18 and the name temporaries/indices in other words (key11 L3, health L1, str3 L2); code and registers match
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
// potion counts double; a hero-stats ratio difference under 0.0001 falls back to the potion counts.
#include <math.h>
struct CTCBase;
struct CCharString {
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
struct CThing {
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
struct CPlayer {
    CThing* GetHeroCreature();                      // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                 // 0x00449960
};
struct CTCInventoryBase {
    long GetNumberOfItemsOfTypeInInventory(long defIndex);   // 0x005BDF08
};
struct CTCHeroStats {
    unsigned char _pad00[0x58];
    long          WillPower;                        // +0x58
    long          MaxWillPower;                     // +0x5c
};

struct CTCContainerRewardHero {
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
        if (!(fabsf(healthRatio - willRatio) < 0.0001f)) {
            if (healthRatio < willRatio)
                return health;
            return mana;
        }
    }
    if (healthCount > manaCount)
        return mana;
    return health;
}

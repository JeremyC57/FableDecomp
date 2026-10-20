// Behaviour test for CTCActionUseBuyHouse::OnActionUse (0x0072ED10): which purchase flow runs, with which money.
#include <cstdio>

class CTCBase {};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
    const SEntry* LowerBound(const long* type) const { const SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_stateFlags;
    CInterfaceMap::SEntry entries[8];
    bool HasTC(long t) const { if (m_presentBits[t >> 5] & (1UL << (t & 31))) return true; return false; }
    template <class T> T* GetTC(long t)
    {
        long type = t; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component;
    }
    template<class T> bool GetInterface(T** ppOut, long t)
    {
        if (m_presentBits[t >> 5] & (1UL << (t & 31))) {
            long type = t; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || t < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = entries; m_stateFlags = 0; }
    void Add(long t, CTCBase* c)   // types added in increasing order
    {
        m_presentBits[t >> 5] |= 1UL << (t & 31);
        m_interfaces.last->type = t; m_interfaces.last->component = c; ++m_interfaces.last;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CTCOwnedEntity : CTCBase { CBaseIntelligentPointer POwner; };
struct CTCBuyableHouse : CTCBase { int id; };
struct CTCHeroStats : CTCBase { long Money; };
struct CPlayer { CThing* hero; CThing* GetHeroThing() { return hero; } };
struct CPlayerManager { CPlayer* main; CPlayer* GetMainPlayer() { return main; } };
struct CBuyHouseDef { bool Main; };
template <class T> struct CDefPointer { T* Object; T* operator->() const { return Object; } };
static int g_base = 0, g_main = 0, g_dress = 0; static long g_money = 0; static CTCBuyableHouse* g_house = 0;
class CTCActionUseBase {
public:
    CThing* ParentThing;
    CPlayerManager* m_players;
    CPlayerManager* GetPlayerManager() { return m_players; }
    void OnActionUse(CThing&) { ++g_base; }
};
class CTCActionUseBuyHouse : public CTCActionUseBase {
public:
    CDefPointer<const CBuyHouseDef> PDef;
    void OnActionUseMain(CTCBuyableHouse* h, long m) { ++g_main; g_house = h; g_money = m; }
    void OnActionUseDress(CTCBuyableHouse* h, long m) { ++g_dress; g_house = h; g_money = m; }
    void OnActionUse(CThing& user)
    {
        CTCActionUseBase::OnActionUse(user);
        CTCOwnedEntity* owned;
        if (ParentThing->GetInterface(&owned, 0x33)) {
            CBaseIntelligentPointer& owner = owned->POwner;
            CThing* thing = owner.GetPItem();
            if (thing && !(thing->m_stateFlags & 1)) {
                thing = owner.GetPItem();
                if (thing->HasTC(0xd5)) {
                    CTCBuyableHouse* house = thing->GetTC<CTCBuyableHouse>(0xd5);
                    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
                    if (hero && hero->HasTC(4)) {
                        CTCHeroStats* stats = hero->GetTC<CTCHeroStats>(4);
                        if (PDef->Main)
                            OnActionUseMain(house, stats->Money);
                        else
                            OnActionUseDress(house, stats->Money);
                    }
                }
            }
        }
    }
};

int main()
{
    int ok = 1;
    CTCHeroStats stats; stats.Money = 1234;
    CThing hero; hero.Init(); hero.Add(4, &stats);
    CPlayer player; player.hero = &hero;
    CPlayerManager pm; pm.main = &player;
    CTCBuyableHouse buyable; buyable.id = 1;
    CThing house; house.Init(); house.Add(0xd5, &buyable);
    CTCOwnedEntity owned; owned.POwner.m_pItem = &house;
    CThing sign; sign.Init(); sign.Add(0x33, &owned);
    CBuyHouseDef def; def.Main = true;
    CTCActionUseBuyHouse use; use.ParentThing = &sign; use.m_players = &pm; use.PDef.Object = &def;
    use.OnActionUse(hero);
    if (g_base != 1 || g_main != 1 || g_dress != 0 || g_money != 1234 || g_house != &buyable) ok = 0;
    def.Main = false;
    use.OnActionUse(hero);
    if (g_base != 2 || g_main != 1 || g_dress != 1) ok = 0;
    player.hero = 0;
    use.OnActionUse(hero);
    if (g_base != 3 || g_main + g_dress != 2) ok = 0;            // no hero: nothing
    player.hero = &hero;
    house.m_stateFlags = 1;
    use.OnActionUse(hero);
    if (g_main + g_dress != 2) ok = 0;                          // dead house thing
    if (!ok) { std::printf("0072ed10_TEST FAIL\n"); return 1; }
    std::printf("0072ed10_TEST PASS\n");
    return 0;
}

#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CThingAICreature : public CThing {
public:
    unsigned char _pad95[0x168 - 0x95];
    CBaseIntelligentPointer HomeBuilding;       // +0x168 CIntelligentPointer<CThingBuilding>
};
class CPlayer { public:
    CThing* GetHeroThing() const;               // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
class CMainGameComponent { public:
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;
class CTCHeroStats { public:
    void SetMarriageState(CThingAICreature* spouse, long state);   // 0x0057BC6E
};
class CTCBuyableHouse { public:                        // interface type 0xD5
    void SetOccupant(CThing* occupant);         // 0x006D3C40 (name not recovered)
    void SetRented(bool rented);                // 0x006C1150
};
class CTCWife { public:
    virtual void v00();
    CThingAICreature* ParentThing;              // +0x04
    unsigned char _pad_0x08[0x20 - 0x08];
    long FrameGotMarriedToThePlayer;            // +0x20
    bool DivorcedHero;                          // +0x24
    bool JustMarried;                           // +0x25
    bool NeedsToChangeBrain;                    // +0x26
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= CWorld::GetWorldFrame(); }
    void SetAttractionIcon(bool show, float scale, unsigned char alpha);   // 0x006CFC90
    void Unmarry(bool keepHouseUnrented);
};

void CTCWife::Unmarry(bool keepHouseUnrented)
{
    if (IsMarriedToThePlayer()) {
        CThingAICreature* creature = ParentThing;
        if (creature->HomeBuilding.GetPItem()) {
            CTCBuyableHouse* house = 0;
            creature->HomeBuilding.GetPItem()->GetInterface(&house, 0xd5);
            house->SetOccupant(0);
            house->SetRented(true);
            if (keepHouseUnrented)
                house->SetRented(false);
        }
        SetAttractionIcon(false, 1.0f, 255);
        FrameGotMarriedToThePlayer = 0x7fffffff;
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        if (hero && !(hero->Flags91 & 1)) {
            CTCHeroStats* stats;
            hero->GetInterface(&stats, 4);
            stats->SetMarriageState(creature, 0);
        }
        NeedsToChangeBrain = true;
    }
}
// ---- test doubles ----
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
static long g_frame = 100;
long CWorld::GetWorldFrame() { return g_frame; }
static CThing* g_hero = 0; static CPlayer g_player; static CPlayerManager g_pm;
CThing* CPlayer::GetHeroThing() const { return g_hero; }
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
static CMainGameComponent g_mgc; CMainGameComponent* g_pMainGameComponent = &g_mgc;
static int g_marriageCalls = 0; static long g_lastState = -1; static CThingAICreature* g_lastSpouse = 0;
void CTCHeroStats::SetMarriageState(CThingAICreature* s, long st) { ++g_marriageCalls; g_lastState = st; g_lastSpouse = s; }
static int g_occupantCalls = 0, g_rentCalls = 0; static bool g_rented = false;
void CTCBuyableHouse::SetOccupant(CThing* o) { if (o == 0) ++g_occupantCalls; }
void CTCBuyableHouse::SetRented(bool r) { ++g_rentCalls; g_rented = r; }
void CTCWife::v00() {}
static int g_iconCalls = 0; static bool g_iconShow = true; static float g_iconScale = 0; static unsigned char g_iconAlpha = 0;
void CTCWife::SetAttractionIcon(bool s, float sc, unsigned char a) { ++g_iconCalls; g_iconShow = s; g_iconScale = sc; g_iconAlpha = a; }
int main()
{
    g_mgc.PlayerManager = &g_pm;
    CTCBuyableHouse house; CTCHeroStats stats;
    CThing building; std::memset(&building, 0, sizeof building);
    CInterfaceMap::SEntry be[1] = { { 0xd5, (CTCBase*)&house } };
    building.m_presentBits[0xd5 >> 5] = 1UL << (0xd5 & 31); building.m_interfaces.first = be; building.m_interfaces.last = be + 1;
    CThing hero; std::memset(&hero, 0, sizeof hero);
    CInterfaceMap::SEntry he[1] = { { 4, (CTCBase*)&stats } };
    hero.m_presentBits[0] = 1UL << 4; hero.m_interfaces.first = he; hero.m_interfaces.last = he + 1; g_hero = &hero;
    CThingAICreature creature; std::memset((CThing*)&creature, 0, sizeof(CThing)); creature.HomeBuilding.PItem = &building;
    CTCWife wife; wife.ParentThing = &creature; wife.FrameGotMarriedToThePlayer = 50; wife.NeedsToChangeBrain = false;
    wife.Unmarry(true);
    bool ok = g_occupantCalls == 1 && g_rentCalls == 2 && !g_rented && g_iconCalls == 1 && !g_iconShow && g_iconScale == 1.0f
        && g_iconAlpha == 255 && wife.FrameGotMarriedToThePlayer == 0x7fffffff && g_marriageCalls == 1 && g_lastState == 0
        && g_lastSpouse == &creature && wife.NeedsToChangeBrain;
    // not married any more: nothing happens
    wife.NeedsToChangeBrain = false; wife.Unmarry(true);
    ok = ok && g_iconCalls == 1 && !wife.NeedsToChangeBrain;
    // married, keep the house rented, no home, dead hero
    wife.FrameGotMarriedToThePlayer = 100; creature.HomeBuilding.PItem = 0; hero.Flags91 = 1;
    wife.Unmarry(false);
    ok = ok && g_rentCalls == 2 && g_marriageCalls == 1 && wife.NeedsToChangeBrain && g_iconCalls == 2;
    creature.HomeBuilding.PItem = &building; wife.FrameGotMarriedToThePlayer = 100; g_hero = 0;
    wife.Unmarry(false);
    ok = ok && g_rentCalls == 3 && g_rented && g_marriageCalls == 1;
    if (!ok) { std::printf("006cff10_TEST FAIL\n"); return 1; }
    std::printf("006cff10_TEST PASS\n");
    return 0;
}
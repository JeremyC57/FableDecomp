// Behaviour test for 006c0580: src/006c0580.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCBuyableHouse::DestroyMoneyBags(bool)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCBuyableHouse::DestroyMoneyBags(bool giveToHero) -- kill every live money bag (interface 0xCC) in the house, first crediting its gold to the live hero's stats when asked
// Leaf: DestroyMoneyBags
// Retail 0x006C0580, 336 bytes, `ret 4`.
// Callees: CVectorMap::LowerBound 0x0040F020 (interfaces 0x21 thing list, 0xCC gold, 4 hero stats), CBaseIntelligentPointer::
// GetPItem 0x00A01B50, the TC's player-manager accessor 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970,
// CPlayer::GetHeroThing 0x00487DC0, CTCGold::GetGold 0x007CBDC0, CTCHeroStats::AddMoney(long, bool) 0x0057B327,
// CThing::Kill(bool) 0x004C9B80.
// Layout: CTCBase ParentThing +4; interface-0x21 component list header +0x0C (landed CountMoneyBags 0x006C04D0 model);
// CThing bitset +0x20, map +0x44, dead flag +0x91.
// Shape: the CountMoneyBags walk (three GetPItem calls for the alive and interface tests) with HasTC + GetTC lookups;
// `this` is homed in the frame across the loop; AddMoney's `true` is pushed before GetGold runs.
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
struct CTCBase;
struct CBaseObject;
struct CCharStringData {
    char* m_pData;                                      // +0x00
    long  m_length;                                     // +0x04
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }   // "" = 0x0129AAF4
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    char operator[](long i) const { return GetPString()[i]; }
    CCharString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099EC70
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
struct CThing;
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct SThingNode {
    SThingNode*                 next;
    SThingNode*                 prev;
    CIntelligentPointer<CThing> thing;                  // +0x08
};
struct CTCThingList_0x21 {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                               // +0x0c
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x78 - 0x4c];
    CCharString   ScriptName;                           // +0x78
    unsigned char _pad_0x7c[0x91 - 0x7c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0 = dead)
    void Kill(bool immediately);                        // 0x004C9B80
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCGold {
    long GetGold();                                     // 0x007CBDC0
};
struct CTCHeroStats {
    void AddMoney(long amount, bool b);                 // 0x0057B327
};
struct CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !(p.Get()->m_stateFlags & 1);
}
class CTCBuyableHouse {
public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    void DestroyMoneyBags(bool giveToHero);
};

void CTCBuyableHouse::DestroyMoneyBags(bool giveToHero)
{
    CTCThingList_0x21* list;
    if (ParentThing->GetInterface(&list, 0x21)) {
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next) {
            CTCGold* gold;
            if (IsThingAlive(node->thing) && node->thing.Get()->GetInterface(&gold, 0xcc)) {
                CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
                if (hero && !(hero->m_stateFlags & 1) && giveToHero && hero->HasInterface(4))
                    hero->GetTC<CTCHeroStats>(4)->AddMoney(gold->GetGold(), true);
                node->thing.Get()->Kill(true);
            }
        }
    }
}
// ---- stubs ----
void CBaseIntelligentPointer::v00() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CTCBuyableHouse::v00() {}
static CPlayerManager g_pm; static CPlayer g_player; static CThing* g_hero;
CPlayerManager* CTCBuyableHouse::GetPlayerManager() { return &g_pm; }
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
CThing* CPlayer::GetHeroThing() { return g_hero; }
static CTCGold g_gold[4];
long CTCGold::GetGold() { return 100 * (long)(this - g_gold + 1); }
static int g_adds; static long g_added; static bool g_addFlag; static CTCHeroStats* g_addStats;
void CTCHeroStats::AddMoney(long amount, bool b) { ++g_adds; g_added += amount; g_addFlag = b; g_addStats = this; }
static int g_kills; static CThing* g_killed[8]; static bool g_killFlag;
void CThing::Kill(bool immediately) { g_killed[g_kills++] = this; g_killFlag = immediately; }
static void Reset() { g_adds = 0; g_added = 0; g_addFlag = false; g_addStats = 0; g_kills = 0; memset(g_killed, 0, sizeof g_killed); g_killFlag = false; }
static long g_thingBuf[8][sizeof(CThing) / 4 + 1];
static CThing* NewThing(int i) { memset(g_thingBuf[i], 0, sizeof g_thingBuf[i]); return (CThing*)g_thingBuf[i]; }
static void SetInterfaces(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
}
int main()
{
    CThing* house = NewThing(0); CThing* bag1 = NewThing(1); CThing* bag2 = NewThing(2); CThing* plain = NewThing(3);
    CThing* bag3 = NewThing(4); CThing* hero = NewThing(5);
    static CTCThingList_0x21 list; static CTCHeroStats stats;
    CInterfaceMap::SEntry he[] = { { 0x21, (CTCBase*)&list } }; SetInterfaces(house, he, 1);
    CInterfaceMap::SEntry b1[] = { { 3, 0 }, { 0xcc, (CTCBase*)&g_gold[0] }, { 0xd0, 0 } }; SetInterfaces(bag1, b1, 3);
    CInterfaceMap::SEntry b2[] = { { 0xcc, (CTCBase*)&g_gold[1] } }; SetInterfaces(bag2, b2, 1); bag2->m_stateFlags = 1;
    CInterfaceMap::SEntry pl[] = { { 4, 0 } }; SetInterfaces(plain, pl, 1);
    CInterfaceMap::SEntry b3[] = { { 0xcc, (CTCBase*)&g_gold[2] } }; SetInterfaces(bag3, b3, 1);
    CInterfaceMap::SEntry hr[] = { { 1, 0 }, { 4, (CTCBase*)&stats }, { 9, 0 } }; SetInterfaces(hero, hr, 3);
    static SThingNode head, nodes[5];
    CThing* things[5] = { bag1, bag2, plain, 0, bag3 };
    SThingNode* prev = &head;
    for (int i = 0; i < 5; ++i) { nodes[i].thing.m_pItem = (CBaseObject*)things[i]; prev->next = &nodes[i]; nodes[i].prev = prev; prev = &nodes[i]; }
    prev->next = &head; head.prev = prev; list.m_head = &head;
    CTCBuyableHouse h; h.ParentThing = house;
    // live hero with stats, credited: both live bags pay 100 + 300 and die
    Reset(); g_hero = hero;
    h.DestroyMoneyBags(true);
    CHECK(g_adds == 2 && g_added == 400 && g_addFlag && g_addStats == &stats);
    CHECK(g_kills == 2 && g_killed[0] == bag1 && g_killed[1] == bag3 && g_killFlag);
    // not credited: bags still die
    Reset();
    h.DestroyMoneyBags(false);
    CHECK(g_adds == 0 && g_kills == 2);
    // dead hero, or no hero: no credit
    Reset(); hero->m_stateFlags = 1;
    h.DestroyMoneyBags(true);
    CHECK(g_adds == 0 && g_kills == 2);
    Reset(); hero->m_stateFlags = 0; g_hero = 0;
    h.DestroyMoneyBags(true);
    CHECK(g_adds == 0 && g_kills == 2);
    // hero without stats interface: no credit
    Reset(); g_hero = plain; plain->m_presentBits[0] = 0;
    h.DestroyMoneyBags(true);
    CHECK(g_adds == 0 && g_kills == 2);
    // house without the thing list: nothing happens
    Reset(); g_hero = hero; house->m_presentBits[1] = 0;
    h.DestroyMoneyBags(true);
    CHECK(g_adds == 0 && g_kills == 0);
    if (fails == 0) printf("006c0580_TEST PASS\n"); else printf("006c0580_TEST FAIL\n");
    return fails;
}